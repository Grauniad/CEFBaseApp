/*
 * GCGVMessageRouter.cpp
 *
 *  Created on: 22 Feb 2015
 *      Author: lhumphreys
 */

#include "CefBaseJSMessageRouter.h"

class PingHandler: public CefBaseJSRequestReply {
    virtual std::string OnRequest(RequestContext& context) {
        return context.request;
    }
};

CefBaseJSMessageRouter::CefBaseJSMessageRouter(
    std::string queryFn,
    std::string cancelFn)
    : reqReps_(nullptr)
{
    CefMessageRouterConfig config;
    config.js_query_function = queryFn;
    config.js_cancel_function = cancelFn;

    browserSideRouter_ = CefMessageRouterBrowserSide::Create(config);
    rendererSideRouter_ = CefMessageRouterRendererSide::Create(config);

    reqReps_.reset(new CefBaseJSRequestReplyHandler);
}

void CefBaseJSMessageRouter::OnContextInitialized() {
    reqReps_->Install<PingHandler>("DEV_TOOLS_Ping");
    browserSideRouter_->AddHandler(reqReps_.get(),false);
}


CefBaseJSMessageRouter::~CefBaseJSMessageRouter() {
}

CefRefPtr<CefMessageRouterBrowserSide> CefBaseJSMessageRouter::GetBrowserSideRouter()
{
    return browserSideRouter_;
}

CefRefPtr<CefMessageRouterRendererSide> CefBaseJSMessageRouter::GetRendererSideRouter()
{
    return rendererSideRouter_;
}
