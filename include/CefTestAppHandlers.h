#ifndef CEFTESTAPP_HANDLERS_H_
#define CEFTESTAPP_HANDLERS_H_

#include "include/cef_app.h"
#include "CefTests.h"

class DummyCefApp;

class DummyCefAppHandlers:
        public CefBrowserProcessHandler,
        public CefRenderProcessHandler
{
public:
    DummyCefAppHandlers(
            DummyCefApp& app,
            int argc,
            char** argv,
            std::string error);

    virtual ~DummyCefAppHandlers();


    /**********************************************************************
     *                         Application Control
     **********************************************************************/

    static void Exit();

    static void Abort();

    /**********************************************************************
     *                         Browser App
     **********************************************************************/

    /**
     * The CEF context has been created. Create a new window and display it.
     *
     */
    virtual void OnContextInitialized() OVERRIDE;

    /**********************************************************************
     *                         Renderer App
     **********************************************************************/

    void OnContextCreated(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefV8Context> context) override;

    bool OK() const { return exitClean; }

private:
    const std::string url;
    int argc;
    char ** argv;
    static bool exitClean;
    bool running;
    DummyCefApp& app;

	IMPLEMENT_REFCOUNTING(DummyCefAppHandlers);
};

#endif
