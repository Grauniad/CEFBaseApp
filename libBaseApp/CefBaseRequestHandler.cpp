/*
 * CefBaseRequestHandler.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseRequestHandler.h"

CefBaseRequestHandler::CefBaseRequestHandler() {
    // TODO Auto-generated constructor stub

}

bool CefBaseRequestHandler::OnBeforeBrowse(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool user_gesture,
    bool is_redirect)
{
    bool cancelNavigation = false;

    for (PointerType& handler: handlers) {
        cancelNavigation |= handler->OnBeforeBrowse(
            browser,
            frame,
            request,
            user_gesture,
            is_redirect);
    }

    return cancelNavigation;
}

void CefBaseRequestHandler::OnRenderProcessTerminated(
    CefRefPtr<CefBrowser> browser,
    TerminationStatus status)
{
    for (PointerType& handler: handlers ) {
        handler->OnRenderProcessTerminated(browser,status);
    }
}

CefBaseRequestHandler::~CefBaseRequestHandler() {
    // TODO Auto-generated destructor stub
}

