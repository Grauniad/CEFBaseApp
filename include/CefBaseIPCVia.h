#ifndef CEFBASEAPP_CEFBASEIPCVIA_H
#define CEFBASEAPP_CEFBASEIPCVIA_H

#include <CefBaseIPCExec.h>

class CefBaseIPCVia: public CefBaseIPCExec::IIPC_TriggeredAsyncFunction {
public:
    CefBaseIPCVia();
    static constexpr const char* NAME = "CefBaseIPCExec_VIA";
    /**
     * The argument should be a JSON string the onwards function
     * call:
     *   {
     *       targetPID: "BROWSER | RENDERER",
     *       targetPROC: "PROC_TO_TRIGGER",
     *       args:  "string argument to trigger..."
     *   }
     */
    void Execute(
            CefRefPtr<CefBrowser> browser,
            std::unique_ptr<CefBaseIPCExec::IIPC_AsyncResult> result,
             CefBaseApp& app,
             std::string args) override;

    class Encoder {
    public:
        struct RawArgs {
            cef_process_id_t pid;
            std::string procToTrigger;
            std::string args;
        };
        virtual std::string Encode(const RawArgs& args) = 0;
        virtual bool Decode(std::string encoded, RawArgs& args) = 0;
    };


    static std::unique_ptr<Encoder> NewEncoder();

private:
    std::unique_ptr<Encoder> encoder;
};


#endif //CEFBASEAPP_CEFBASEIPCVIA_H
