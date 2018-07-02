/*
 * CefBaseRequestHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEREQUESTHANDLER_H_
#define CEFBASEREQUESTHANDLER_H_

#include <include/cef_request_handler.h>
#include "CefBaseMultiHandler.h"

class CefBaseRequestHandler: public CefBaseMultiHandler<CefRequestHandler> {
public:
    CefBaseRequestHandler();

    /**
     * Process: Browser
     * Thread:  UI
     *
     * Event: The browser is about to change page, return true to cancel
     *
     * Each handler is called, in order they were installed. If any return true,
     * the navigation is cancelled.
     */
    virtual bool OnBeforeBrowse(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        bool user_gesture,
        bool is_redirect) override;

    /**
     * Process: Browser
     * Thread:  UI
     *
     * Event: The renderer Process has terminated unexpectedly
     */
    virtual void OnRenderProcessTerminated(
        CefRefPtr<CefBrowser> browser,
        TerminationStatus status) override;

    virtual ~CefBaseRequestHandler();
};

#endif /* CEFBASEREQUESTHANDLER_H_ */
