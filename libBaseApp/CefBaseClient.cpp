/*
 * CefBaseClient.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseClient.h"

CefBaseClient::CefBaseClient(CefBaseApp* parent)
        :
            parent_(parent),
            lifeSpanHandler_(new CefBaseLifeSpanHandler),
            renderHandler_(new CefBaseRenderHandler),
            requestHandler_(new CefBaseRequestHandler),
            displayHandler_(new CefBaseDisplayHandler),
            loadHandler_ (new CefBaseLoadHandler)
{
}

CefBaseClient::~CefBaseClient() {
    // TODO Auto-generated destructor stub
}

CefRefPtr<CefLifeSpanHandler> CefBaseClient::GetLifeSpanHandler() {
    return lifeSpanHandler_;
}

CefRefPtr<CefRenderHandler> CefBaseClient::GetRenderHandler() {
    return renderHandler_;
}

CefRefPtr<CefLoadHandler> CefBaseClient::GetLoadHandler() {
    return loadHandler_;
}

CefRefPtr<CefRequestHandler> CefBaseClient::GetRequestHandler() {
    return requestHandler_;
}
CefRefPtr<CefDisplayHandler> CefBaseClient::GetDisplayHandler() {
    return displayHandler_;
}


CefBaseLifeSpanHandler& CefBaseClient::LifeSpanHandler() {
    return static_cast<CefBaseLifeSpanHandler&>(*lifeSpanHandler_);
}

bool CefBaseClient::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
    bool handled = false;

    for (MessageHandlerPtrType& handler : messageHandlers_) {
        handled = handler->OnProcessMessageReceived(
            browser,
            source_process,
            message);
    }

    return handled;
}

CefBaseRenderHandler& CefBaseClient::RenderHandler() {
    return static_cast<CefBaseRenderHandler&>(*renderHandler_);
}

CefBaseRequestHandler& CefBaseClient::RequestHandler() {
    return static_cast<CefBaseRequestHandler&>(*requestHandler_);
}

CefBaseDisplayHandler& CefBaseClient::DisplayHandler() {
    return static_cast<CefBaseDisplayHandler&>(*displayHandler_);
}

CefBaseLoadHandler& CefBaseClient::LoadHandler() {
    return static_cast<CefBaseLoadHandler&>(*loadHandler_);
}
