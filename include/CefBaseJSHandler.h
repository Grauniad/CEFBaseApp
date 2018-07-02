/*
 * GCGVJSHandler.h
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#ifndef GCGVJSHANDLER_H_
#define GCGVJSHANDLER_H_

#include "CefBaseJSMessageRouter.h"
#include "CefBaseApp.h"
#include <CEFJSUtils.h>
#include "include/cef_browser.h"
#include "include/cef_base.h"

/*
 * External interface for the java-script library.
 *
 * All callbacks from the browser / renderer handlers will go via here, and then
 * fan out into any child-classes as required.
 */
class CefBaseJSHandler: public virtual CefBaseRefCounted {
public:

    /**
     * Install this java-script hander into an application
     */
    void InstallHandler(CefBaseApp& app);

    /**
     * Accessor to the Req-Rep object (Browser only) which dynamic installation
     * of new req-rep handlers at run time
     */
    CefBaseJSRequestReplyHandler& ReqReps() { return router_->ReqReps(); }

    /**
     * @param queryFn   The JS query function to start async requests
     * @param cancelFn  The JS function  to cancel ongoing async requsts.
     */
    CefBaseJSHandler(const std::string& queryFn, const std::string& cancelFn);

    virtual ~CefBaseJSHandler();


    /****************************************************************************
     *                        Common Callbacks
     ****************************************************************************/
    /*
     * Handles the receipt of messages from another process. This callback is
     * implemented on the CefClient, and may be invoked from either the renderer
     * or the browser.
     *
     * This method worries about who to route the message to, allowing the
     * CefClient to not worry about the correct handler to call
     */
    bool OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message);

    /****************************************************************************
     *                        Browser Callbacks
     ****************************************************************************/

    ///
    // Call from CefLifeSpanHandler::OnBeforeClose. Any pending queries associated
    // with |browser| will be canceled and Handler::OnQueryCanceled will be called.
    // No JavaScript callbacks will be executed since this indicates destruction
    // of the browser.
    ///
    void OnBeforeClose(CefRefPtr<CefBrowser> browser);

    ///
    // Call from CefRequestHandler::OnRenderProcessTerminated. Any pending queries
    // associated with |browser| will be canceled and Handler::OnQueryCanceled
    // will be called. No JavaScript callbacks will be executed since this
    // indicates destruction of the context.
    ///
    void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser);

    ///
    // Call from CefRequestHandler::OnBeforeBrowse only if the navigation is
    // allowed to proceed. If |frame| is the main frame then any pending queries
    // associated with |browser| will be canceled and Handler::OnQueryCanceled
    // will be called. No JavaScript callbacks will be executed since this
    // indicates destruction of the context.
    ///
    void OnBeforeBrowse(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame);


    /****************************************************************************
     *                        Renderer Callbacks
     ****************************************************************************/

    // The below methods should be called from other CEF handlers. They must be
    // called exactly as documented for the router to function correctly.
    ///
    // Call from CefRenderProcessHandler::OnContextCreated. Registers the
    // JavaScripts functions with the new context.
    ///
    void OnContextCreated(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context);

    ///
    // Call from CefRenderProcessHandler::OnContextReleased. Any pending queries
    // associated with the released context will be canceled and
    // Handler::OnQueryCanceled will be called in the browser process.
    ///
    void OnContextReleased(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context);

protected:
    /*
     * Register custom JS functions
     *
     * @param context   The JS context to add the functions to
     */
    virtual void RegisterJSFunctions(CefRefPtr<CefV8Context> context);

private:
    ///
    // Call from CefClient::OnProcessMessageReceived. Returns true if the message
    // is handled by this router or false otherwise.
    ///
    bool OnBrowserProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message);

    ///
    // Call from CefRenderProcessHandler::OnProcessMessageReceived. Returns true
    // if the message is handled by this router or false otherwise.
    ///
    bool OnRendererProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message);


    /****************************************************************************
     *                        Data
     ****************************************************************************/

    std::shared_ptr<CefBaseJSMessageRouter> router_;

    IMPLEMENT_REFCOUNTING(CefBaseJSHandler);
};

#endif /* GCGVJSHANDLER_H_ */
