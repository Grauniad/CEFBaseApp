#ifndef CEFTESTAPP_HANDLERS_H_
#define CEFTESTAPP_HANDLERS_H_

#include "include/cef_app.h"
#include "CefTests.h"

class DummyCefAppHandlers:
        public CefBrowserProcessHandler,
        public CefRenderProcessHandler
{
public:
    DummyCefAppHandlers();

    /**********************************************************************
     *                         Setup
     **********************************************************************/
    /*
     * Add a test to the internal list
     */
    void AddTest(std::unique_ptr<CefTestBase>&& test);

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

    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefV8Context> context);

    bool OK() const { return exitClean; }

private:
    std::vector<std::unique_ptr<CefTestBase>> tests;
    static bool exitClean;

	IMPLEMENT_REFCOUNTING(DummyCefAppHandlers);
};

#endif
