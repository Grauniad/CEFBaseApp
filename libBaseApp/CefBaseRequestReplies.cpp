/*
 * CefBaseRequestReplies.cpp
 *
 *  Created on: 29 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseRequestReplies.h"

#include <logger.h>


bool CefBaseJSRequestReplyHandler::InstallHandler(
    const std::string& name,
    HandlerPointer handler)
{
    auto it = handlers.find(name);
    bool ok = false;

    if (it == handlers.end()) {
        handlers.insert(HandlerMap::value_type(name, std::move(handler)));
    }

    SLOG_FROM(
        LOG_VERBOSE,
        "CefBaseJSRequestReplyHandler::InstallHandler",
        "Installed handler: " << name
        )

    return ok;
}


bool CefBaseJSRequestReplyHandler::OnQuery(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id,
    const CefString& request,
    bool persistent,
    CefRefPtr<Callback> callback)
{
    std::string reqString = request;

    bool handled = false;

    if ( strncmp("REQUEST_",reqString.c_str(),8) == 0 ) {
        handled = GetResponse(reqString, callback, browser);
    }

    if (handled) {
        SLOG_FROM(
            LOG_VERBOSE,
            "CefBaseJSRequestReplyHandler::OnQuery",
            "Handled request: " << reqString
            )
    } else {
        SLOG_FROM(
            LOG_VERBOSE,
            "CefBaseJSRequestReplyHandler::OnQuery",
            "Failed to handle request: " << reqString
            )

    }


    return handled;
}

bool CefBaseJSRequestReplyHandler::GetResponse(
    const std::string& reqString,
    const CefRefPtr<Callback>& callback,
    CefRefPtr<CefBrowser> browser)
{
    bool handled = false;

    std::string name;
    std::stringstream reqSStream(reqString);
    reqSStream >> name;

    const size_t prefixLen = 8; // "REQUEST_"

    const char* reqName = "";
    if (name.length() > prefixLen) {
        reqName = name.c_str() + prefixLen;
    }

    auto it = handlers.find(reqName);

    if (it != handlers.end()) {
        CefBaseJSRequestReply& handler = *it->second;
        handled = GetResponse(reqString, name, callback, browser, handler);
    }

    return handled;
}

bool CefBaseJSRequestReplyHandler::GetResponse(
    const std::string&       request,
    const std::string&       name,
    const CefRefPtr<Callback> callback,
    CefRefPtr<CefBrowser> browser,
    CefBaseJSRequestReply& handler)
{
    bool handled = false;

    try {
        const size_t prefix = name.length() + 1; // include the space
        CefBaseJSRequestReply::RequestContext context
            {
                request.length() > prefix ? request.substr(prefix): "",
                browser
            };

        std::string response = handler.OnRequest(context);

        handled = true;
        callback->Success(std::move(response));
    } catch (CefBaseJSRequestReply::CefBaseInvalidRequestException& e) {
        handled = true;
        callback->Failure(e.code, e.errMsg);
    } catch (CefBaseJSRequestReply::CefBaseAbdandonRequest& e) {
        // silently drop the request
        handled = true;
    }
    return handled;
}

void CefBaseJSRequestReplyHandler::OnQueryCanceled(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id)
{
    // Nothing to do...
}
