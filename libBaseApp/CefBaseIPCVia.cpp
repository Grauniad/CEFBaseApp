#include <CefBaseIPCVia.h>
#include <SimpleJSON.h>
#include <CefBaseApp.h>

namespace {
    NewStringField(targetPID);
    NewStringField(targetProc);
    NewStringField(args);
    using EncodedArgs = SimpleParsedJSON<targetPID, targetProc, args>;

    class JSONEncoder: public CefBaseIPCVia::Encoder {
    public:
        std::string Encode(const RawArgs& args) {
            encoder.Clear();
            switch (args.pid) {
                case PID_RENDERER:
                    encoder.Get<targetPID>() = "RENDERER";
                    break;
                case PID_BROWSER:
                    encoder.Get<targetPID>() = "BROWSER";
                    break;
            }
            encoder.Get<targetProc>() = args.procToTrigger;
            encoder.Get<::args>() = args.args;

            return encoder.GetJSONString();
        }

        bool Decode(std::string encoded, RawArgs& args) {
            encoder.Clear();
            std::string error;
            bool ok = encoder.Parse(encoded.c_str(), error);

            if (ok) {
                args.procToTrigger = encoder.Get<targetProc>();
                args.args = encoder.Get<::args>();
                if (encoder.Get<targetPID>() == "RENDERER") {
                    args.pid = PID_RENDERER;
                } else {
                    args.pid = PID_BROWSER;
                }
            }

            return ok;

        }
    private:
        EncodedArgs encoder;
    };
}

CefBaseIPCVia::CefBaseIPCVia() {
    encoder = NewEncoder();
}

void CefBaseIPCVia::Execute(
        CefRefPtr<CefBrowser> browser,
        std::unique_ptr <CefBaseIPCExec::IIPC_AsyncResult> result,
        CefBaseApp& app,
        std::string args)
{
    Encoder::RawArgs rawArgs;
    if (encoder->Decode(args, rawArgs)) {
        std::shared_ptr<CefBaseIPCExec::IIPC_AsyncResult> sharedResult;
        sharedResult = std::move(result);
        app.IPC().Execute(
                rawArgs.pid,
                browser,
                rawArgs.procToTrigger,
                rawArgs.args,
                [=] (std::string r) -> void {
                    sharedResult->Dispatch(std::move(r));
                });
    } else {
        result->Dispatch("FAILED TO EXECUTE");
    }
}

std::unique_ptr <CefBaseIPCVia::Encoder> CefBaseIPCVia::NewEncoder() {
    return std::make_unique<JSONEncoder>();
}

