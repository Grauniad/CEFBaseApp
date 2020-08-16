/*
 * GCGVJSHandler.cpp
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#include "CefBaseJSHandler.h"

#include <CEFJSLogging.h>
#include "include/wrapper/cef_helpers.h"

CefBaseJSHandler::CefBaseJSHandler(
    const std::string& queryFn,
    const std::string& cancelFn)
   : router_(new CefBaseJSMessageRouter(queryFn,cancelFn))
{
}

CefBaseJSHandler::~CefBaseJSHandler() {
}


void CefBaseJSHandler::RegisterJSFunctions(CefRefPtr<CefV8Context> context) {
	CEFJSLogging::AddFormater(context);
}

void CefBaseJSHandler::InstallHandler(CefBaseApp& app) {
    class JSLifeSpanInstaller: public CefLifeSpanHandler {
    public:
        JSLifeSpanInstaller(CefRefPtr<CefBaseJSHandler> _jsHandler)
           : jsHandler(_jsHandler) { }

        void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE {
            jsHandler->OnBeforeClose(browser);
        }
    private:
        CefRefPtr<CefBaseJSHandler> jsHandler;
        IMPLEMENT_REFCOUNTING(JSLifeSpanInstaller);
    };

    class JSRequestHandler: public CefRequestHandler {
    public:
        JSRequestHandler(CefRefPtr<CefBaseJSHandler> _jsHandler)
           : jsHandler(_jsHandler) { }

        void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                       TerminationStatus status) OVERRIDE
        {
            jsHandler->OnRenderProcessTerminated(browser);
        }

        bool OnBeforeBrowse(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            bool user_gesture,
            bool is_redirect) OVERRIDE
        {
            jsHandler->OnBeforeBrowse(browser,frame);

            return false;
        }
    private:
        CefRefPtr<CefBaseJSHandler> jsHandler;
        IMPLEMENT_REFCOUNTING(JSRequestHandler);
    };

    class JSRendererProcess: public CefRenderProcessHandler {
    public:
        JSRendererProcess(CefRefPtr<CefBaseJSHandler> _jsHandler)
           : jsHandler(_jsHandler) { }

        void OnContextCreated(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) OVERRIDE
        {
            jsHandler->OnContextCreated(browser,frame,context);
        }

        void OnContextReleased(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) OVERRIDE
        {
            jsHandler->OnContextReleased(browser,frame,context);
        }

        bool OnProcessMessageReceived(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) OVERRIDE
        {
            return jsHandler->OnProcessMessageReceived(
                browser,
                frame,
                source_process,
                message);
        }

    private:
        CefRefPtr<CefBaseJSHandler> jsHandler;
        IMPLEMENT_REFCOUNTING(JSRendererProcess);
    };

    class JSCefClient: public CefClient {
    public:
        JSCefClient(CefRefPtr<CefBaseJSHandler> _jsHandler)
           : jsHandler(_jsHandler) { }

        bool OnProcessMessageReceived(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) OVERRIDE
        {
            return jsHandler->OnProcessMessageReceived(
                browser,
                frame,
                source_process,
                message);
        }

    private:
        CefRefPtr<CefBaseJSHandler> jsHandler;
        IMPLEMENT_REFCOUNTING(JSCefClient);
    };

    app.Client().InstallMessagerHandler(
        std::shared_ptr<CefClient>(new JSCefClient(this)));
    app.Renderer().InstallHandler(
        std::shared_ptr<CefRenderProcessHandler>(new JSRendererProcess(this)));
    app.Client().RequestHandler().InstallHandler(
        std::shared_ptr<CefRequestHandler>(new JSRequestHandler(this)));
    app.Client().LifeSpanHandler().InstallHandler(
        std::shared_ptr<CefLifeSpanHandler>(new JSLifeSpanInstaller(this)));

    app.Browser().InstallHandler(router_);
}

/****************************************************************************
 *                        Browser Events
 ****************************************************************************/

void CefBaseJSHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

    router_->GetBrowserSideRouter()->OnBeforeClose(browser);
}

void CefBaseJSHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

    router_->GetBrowserSideRouter()->OnRenderProcessTerminated(browser);
}

void CefBaseJSHandler::OnBeforeBrowse(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame)
{
	CEF_REQUIRE_UI_THREAD();

    router_->GetBrowserSideRouter()->OnBeforeBrowse(browser,frame);
}

bool CefBaseJSHandler::OnBrowserProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
	CEF_REQUIRE_UI_THREAD();
    bool handled = false;

    if (!handled)
    {
        handled = router_->GetBrowserSideRouter()->OnProcessMessageReceived(
            browser,
            frame,
            source_process,
            message);
    }

    return handled;
}

/****************************************************************************
 *                        Renderer Events
 ****************************************************************************/

void CefBaseJSHandler::OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    RegisterJSFunctions(context);

    router_->GetRendererSideRouter()->OnContextCreated(browser,frame,context);
}

void CefBaseJSHandler::OnContextReleased(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    router_->GetRendererSideRouter()->OnContextReleased(
        browser,
        frame,
        context);
}

bool CefBaseJSHandler::OnRendererProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
    bool handled = false;

    if (!handled)
    {
        handled = router_->GetRendererSideRouter()->OnProcessMessageReceived(
            browser,
            frame,
            source_process,
            message);
    }

    return handled;
}

bool CefBaseJSHandler::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
    bool handled = false;
    if (CefCurrentlyOn(TID_RENDERER)) {
        handled = OnRendererProcessMessageReceived(
            browser,
            frame,
            source_process,
            message);
    } else if (CefCurrentlyOn(TID_UI)) {
        handled = OnBrowserProcessMessageReceived(
            browser,
            frame,
            source_process,
            message);
    }

    return handled;
}
