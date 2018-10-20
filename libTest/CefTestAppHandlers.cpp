#include "CefTestAppHandlers.h"
#include "CefTestClient.h"
#include "CefTestApp.h"
#include "CefBaseIPCExec.h"

#include "include/wrapper/cef_helpers.h"
#include <env.h>
#include <iostream>

#include <include/cef_cookie.h>

#include "CefBaseThread.h"
#include <gtest/gtest.h>
#include <CefTestAppHandlers.h>


bool DummyCefAppHandlers::exitClean = true;

DummyCefAppHandlers::DummyCefAppHandlers(
        DummyCefApp& app,
        int argc,
        char ** argv,
        std::string url)
    : url(std::move(url))
    , argc(argc)
    , argv(argv)
    , running(false)
    , app(app)
{
    // SimpleHandler implements browser-level callbacks.
    std::shared_ptr<DummyCefClient> handler(new DummyCefClient);
    std::shared_ptr<CefClient> client = handler;
    std::shared_ptr<CefLifeSpanHandler> lifeHandler = handler;
    std::shared_ptr<CefRenderHandler> renderHandler = handler;

    app.Client().InstallMessagerHandler(client);
    app.Client().LifeSpanHandler().InstallHandler(lifeHandler);
    app.Client().RenderHandler().InstallHandler(renderHandler);
}

void DummyCefAppHandlers::OnContextInitialized() {
    // This should be called from the browser process..
    CEF_REQUIRE_UI_THREAD()
    ;

    // Information used when creating the native window.
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    // Create the first browser window.
    CefBrowserHost::CreateBrowser(
        window_info,
        app.GetClient().get(),
        url,
        browser_settings,
        NULL);
}

void DummyCefAppHandlers::Exit() {
    if (CefCurrentlyOn(TID_UI)) {
        DummyCefApp::ClearTestBrowser();
        exitClean = true;
        CefQuitMessageLoop();
    } else {
    }
}

void DummyCefAppHandlers::Abort() {
    if (CefCurrentlyOn(TID_UI)) {
        DummyCefApp::ClearTestBrowser();
        exitClean = false;
        CefQuitMessageLoop();
    } else {
    }
}

void DummyCefAppHandlers::OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    DummyCefApp::SetTestContext(context);
    if (!running) {
        DummyCefApp::SetTestBrowser(browser);
        running = true;
        // JS initialised - run tests
        DoInNewThread([this,browser, context] () {
            std::cout.flush();
            ::testing::InitGoogleTest(&argc, argv);
            int result = RUN_ALL_TESTS();

            if (result != 0) {
                // When done - kill the process...
                CefBaseThread::GetResultFromRender<int>([=] () {
                    CefRefPtr<CefProcessMessage> msg =
                            CefProcessMessage::Create("ABORT");
                    browser->SendProcessMessage(PID_BROWSER,msg);
                    return 0;
                });
            } else {
                // When done - kill the process...
                CefBaseThread::GetResultFromRender<int>([=] () {
                    CefRefPtr<CefProcessMessage> msg =
                            CefProcessMessage::Create("EXIT");
                    browser->SendProcessMessage(PID_BROWSER,msg);
                    return 0;
                });
            }

        });
    }
}

DummyCefAppHandlers::~DummyCefAppHandlers() {

}
