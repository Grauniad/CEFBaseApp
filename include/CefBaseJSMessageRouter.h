/*
 * GCGVMessageRouter.h
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#ifndef GCGVMESSAGEROUTER_H_
#define GCGVMESSAGEROUTER_H_

#include <include/wrapper/cef_message_router.h>
#include <include/cef_browser_process_handler.h>

#include "CefBaseRequestReplies.h"


class CefBaseJSMessageRouter: public CefBrowserProcessHandler
{
public:
    /*
     * Create a new message router
     */
    CefBaseJSMessageRouter(std::string queryFn, std::string cancelFn);

    virtual ~CefBaseJSMessageRouter();

    CefRefPtr<CefMessageRouterBrowserSide> GetBrowserSideRouter();

    CefRefPtr<CefMessageRouterRendererSide> GetRendererSideRouter();

    /**************************************************************************
     *                      Trigger to install handlers
     *************************************************************************/

    virtual void OnContextInitialized();

    /**
     * Process: Browser
     * Thread:  UI
     *
     * Provides access to the reqReps object.
     *
     * Note that this is only valid on the the browser process.
     *
     */
    CefBaseJSRequestReplyHandler& ReqReps() { return *reqReps_; }


private:
    CefRefPtr<CefMessageRouterBrowserSide>  browserSideRouter_;
    CefRefPtr<CefMessageRouterRendererSide> rendererSideRouter_;

    std::shared_ptr<CefBaseJSRequestReplyHandler>  reqReps_;

	IMPLEMENT_REFCOUNTING(CefBaseJSMessageRouter);
};

#endif /* GCGVMESSAGEROUTER_H_ */
