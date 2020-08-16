#include <CefBaseIPCExec.h>
#include <CefBaseThread.h>
#include <CefBaseApp.h>
#include <CefBaseIPCVia.h>

namespace  {
    static const char* const MESSAGE_NAME = "CefBaseIPCExec";
    static const char* const RESPONSE_MESSAGE_NAME = "CefBaseIPCExec_Reply";

    class Ping: public CefBaseIPCExec::IIPC_TriggeredFunction {
    public:
        static constexpr const char* NAME = "CefBaseIPCExec_PING";
        std::string Execute(CefBaseApp& app, std::string args) override {
            return args;
        }
    };

    class Result: public CefBaseIPCExec::IIPC_AsyncResult {
    public:
        Result(int h,
                CefRefPtr<CefBrowser> b,
               cef_process_id_t pid,
               CefBaseIPCExec& ipc)

            : browser(b)
            , hdl(h)
            , targetId(pid)
            , exec(ipc)
        {
        }

        void Dispatch(std::string result) override {
            exec.SetResult(browser, hdl, targetId, std::move(result));
        }

    private:
        CefRefPtr<CefBrowser> browser;
        int hdl;
        cef_process_id_t targetId;
        CefBaseIPCExec& exec;
    };

    class LocalResult: public CefBaseIPCExec::IIPC_AsyncResult {
    public:
        LocalResult(cef_thread_id_t tid, CefBaseIPCExec::Callback cb)
                : tid(tid)
                , cb(std::move(cb))
        {
        }

        void Dispatch(std::string result) override {
            CefBaseThread::PostToCEFThread(tid, [=, cb=std::move(cb)] () -> void
            {
                cb(std::move(result));
            });
        }

    private:
        cef_thread_id_t tid;
        CefBaseIPCExec::Callback cb;
    };

}

CefBaseIPCExec::CefBaseIPCExec(CefBaseApp& app)
   : app(app)
{
    Install(Ping::NAME, std::make_shared<::Ping>());
    Install(CefBaseIPCVia::NAME, std::make_shared<CefBaseIPCVia>());
}

std::shared_ptr<CefBaseIPCExec> CefBaseIPCExec::Install(CefBaseApp& app)
{
    auto hdlr = std::make_shared<CefBaseIPCExec>(app);
    app.GetClient()->InstallMessagerHandler(hdlr);
    app.Renderer().InstallHandler(hdlr);

    return hdlr;
}

bool CefBaseIPCExec::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message) {
    bool handled = false;
    if (message->GetName() == MESSAGE_NAME) {
        int hdl = message->GetArgumentList()->GetInt(0);
        std::string trigger = message->GetArgumentList()->GetString(1);
        std::string farg = message->GetArgumentList()->GetString(2);
        auto result =
            std::make_unique<Result>(hdl, browser, source_process, *this);
        handled = triggers.Trigger(browser,
                                   std::move(result),
                                   app,
                                   std::move(trigger),
                                   std::move(farg));
    } else if(message->GetName() == RESPONSE_MESSAGE_NAME) {
        cbs.Callback(
            message->GetArgumentList()->GetInt(0),
            message->GetArgumentList()->GetString(1));

        handled = true;
    }

    return handled;
}

void CefBaseIPCExec::Ping(
        const cef_process_id_t& target,
        CefRefPtr<CefBrowser> browser,
        std::function<void(std::string)> cb)
{
    Execute(target, browser, ::Ping::NAME, "PONG", std::move(cb));
}

void CefBaseIPCExec::Install(
        std::string name,
        std::shared_ptr<CefBaseIPCExec::IIPC_TriggeredAsyncFunction> trigger)
{
    triggers.Store(std::move(name), std::move(trigger));
}

void CefBaseIPCExec::Execute(const cef_process_id_t& target,
                             CefRefPtr<CefBrowser> browser,
                             std::string trigger,
                             std::string arg,
                             CefBaseIPCExec::Callback cb)
{
    bool localExec = false;
    cef_thread_id_t postThread = TID_RENDERER;
    switch (app.CurrentProcess()) {
        case CefBaseApp::ProcessId::BROWSER:
            if (target == PID_BROWSER) {
                localExec = true;
            }
            postThread = TID_UI;
            break;
        case CefBaseApp::ProcessId::RENDERER:
            postThread = TID_RENDERER;
            if (target == PID_RENDERER) {
                localExec = true;
            }
            break;
        case CefBaseApp::ProcessId::UNKNOWN:
            throw UnknownSourcePID{};
    }

    if (localExec) {
        CefBaseThread::PostToCEFThread(postThread, [=, cb=std::move(cb)] () {
            auto result = std::make_unique<LocalResult>(postThread, cb);
            triggers.Trigger(browser, std::move(result), app, trigger, arg);
        });
    } else {
        auto hdl = cbs.Store(cb);
        CefRefPtr<CefProcessMessage> message =
                CefProcessMessage::Create(MESSAGE_NAME);
        auto args = message->GetArgumentList();
        args->SetSize(3);
        args->SetInt(0, hdl);
        args->SetString(1, std::move(trigger));
        args->SetString(2, std::move(arg));

        // On the renderer process, only TID_RENDERER may invoke
        // SendProcessMessage
        if (postThread == TID_RENDERER) {
            CefBaseThread::PostToCEFThread(TID_RENDERER, [=] () {
                browser->GetMainFrame()->SendProcessMessage(target, message);
            });
        } else {
            browser->GetMainFrame()->SendProcessMessage(target, message);
        }
    }
}

void CefBaseIPCExec::SetResult(CefRefPtr<CefBrowser> browser,
                               int hdl,
                               cef_process_id_t target,
                               std::string result)
{

    CefRefPtr<CefProcessMessage> reply =
            CefProcessMessage::Create(RESPONSE_MESSAGE_NAME);
    auto args = reply->GetArgumentList();
    args->SetSize(2);
    args->SetInt(0, hdl);
    args->SetString(1, std::move(result));
    browser->GetMainFrame()->SendProcessMessage(target, reply);
}

CefBaseIPCExec::StoredCallbacks::StoredCallbacks()
    : nextIdx(0)
{
}

int CefBaseIPCExec::StoredCallbacks::Store(CefBaseIPCExec::Callback cb) {
    nextIdx++;
    callbacks.emplace(nextIdx, std::move(cb));
    return nextIdx;
}

void CefBaseIPCExec::StoredCallbacks::Callback(int hdl, std::string arg) {
    auto it = callbacks.find(nextIdx);
    if (it != callbacks.end()) {
        it->second(std::move(arg));
        callbacks.erase(it);
    }
}

bool CefBaseIPCExec::StoredTriggers::Trigger(
        CefRefPtr<CefBrowser> browser,
        std::unique_ptr<IIPC_AsyncResult> result,
        CefBaseApp& app,
        const std::string& name,
        std::string arg)
{
    bool found = false;
    auto it = triggers.find(name);

    if (it != triggers.end()) {
        it->second->Execute(browser, std::move(result), app, std::move(arg));
        found = true;
    }

    return found;
}

void CefBaseIPCExec::StoredTriggers::Store(
        std::string name,
        std::shared_ptr<CefBaseIPCExec::IIPC_TriggeredAsyncFunction> trigger)
{
    triggers[name] = trigger;
}


void CefBaseIPCExec::IIPC_TriggeredFunction::Execute(
        CefRefPtr<CefBrowser> browser,
        std::unique_ptr<CefBaseIPCExec::IIPC_AsyncResult> result,
        CefBaseApp& app,
        std::string args)
{
    result->Dispatch(this->Execute(app, args));
}
