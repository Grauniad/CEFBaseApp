#ifndef CEFBASEAPP_CEFBASEIPCEXEC_H
#define CEFBASEAPP_CEFBASEIPCEXEC_H

#include <CefBaseClient.h>

#include <functional>
#include <CefBaseRendererProcess.h>

/**
 * When installed correctly into the relevant processes, the
 * OnProcessMessageReceived will be triggered by the underlying
 * framework on receipt of the message.
 *
 * Note that this callback is triggered on different interfaces
 * depending on the process:
 *    CefClient:               Triggered on the UI process
 *    CefRenderProcessHandler: Triggered on the Renderer process
 *
 * And so we must install ourselves into both base handlers. The Install()
 * method should be used to ensure this is done correctly
 */
class CefBaseIPCExec: public CefClient,
                      public CefRenderProcessHandler
{
public:
    using Callback = std::function<void (std::string)>;
    using TriggerFunctionPtr = std::string (*)(std::string);

    CefBaseIPCExec(CefBaseApp& app);
    virtual ~CefBaseIPCExec() = default;

    /**
     * Register for message callbacks
     */
    static std::shared_ptr<CefBaseIPCExec> Install(CefBaseApp& app);

    class IIPC_AsyncResult {
    public:
        virtual ~IIPC_AsyncResult() = default;
        virtual void Dispatch(std::string result) = 0;
    };

    class IIPC_TriggeredAsyncFunction {
    public:
        virtual ~IIPC_TriggeredAsyncFunction() = default;
        virtual void Execute(
                CefRefPtr<CefBrowser> browser,
                std::unique_ptr<IIPC_AsyncResult> result,
                CefBaseApp& app,
                std::string args) = 0;
    };

    class IIPC_TriggeredFunction: public IIPC_TriggeredAsyncFunction {
    public:
        virtual ~IIPC_TriggeredFunction() = default;
        virtual std::string Execute(
                CefBaseApp& app,
                std::string args) = 0;
    private:
        virtual void Execute(
                CefRefPtr<CefBrowser> b,
                std::unique_ptr<IIPC_AsyncResult> result,
                CefBaseApp& app,
                std::string args) override;
    };
    void Install(std::string name,
                 std::shared_ptr<IIPC_TriggeredAsyncFunction> trigger);

    /**
     * Ping a message out to the specified process, receiving a callback
     * once the process has responded.
     *
     * @param target   The process to ping
     * @param browser  The browser instance to use for the message routing
     * @param cb       The onComplete callback to trigger, in this process - on
     *                 the message receiving thread.
     */
    void Ping( const cef_process_id_t& target,
               CefRefPtr<CefBrowser> browser,
               Callback cb);

    /**
     * Ping a message out to the specified process, receiving a callback
     * once the process has responded.
     *
     * @param target    The process to execute on
     * @param browser   The browser instance to use for the message routing
     * @param trigger   The trigger to execute
     * @param arg       The argument to pass to the trigger
     * @param cb        The callback to trigger, in this process - on the
     *                  message receiving thread.
     */
    void Execute(
            const cef_process_id_t& target,
            CefRefPtr<CefBrowser> browser,
            std::string trigger,
            std::string arg,
            Callback cb);

    void SetResult(CefRefPtr<CefBrowser>,
                   int hdl,
                   cef_process_id_t target,
                   std::string result);

    struct UnknownSourcePID {};

private:
    bool OnProcessMessageReceived(
            CefRefPtr<CefBrowser> browser,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) override;

    class StoredCallbacks {
    public:
        StoredCallbacks();
        int Store(CefBaseIPCExec::Callback cb);

        void Callback(int hdl, std::string arg);
    private:
        int nextIdx;
        std::map<size_t, CefBaseIPCExec::Callback> callbacks;
    };

    class StoredTriggers {
    public:
        bool Trigger(
                CefRefPtr<CefBrowser> browser,
                std::unique_ptr<IIPC_AsyncResult> result,
                CefBaseApp& app,
                const std::string& name,
                std::string arg);

        void Store(
                std::string name,
                std::shared_ptr<IIPC_TriggeredAsyncFunction> trigger);
    private:
        std::map<std::string, std::shared_ptr<IIPC_TriggeredAsyncFunction>> triggers;
    };
    StoredCallbacks cbs;
    StoredTriggers  triggers;
    CefBaseApp&     app;

    IMPLEMENT_REFCOUNTING(CefBaseIPCExec)
};

#endif
