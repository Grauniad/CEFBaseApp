/*
 * CefBaseRendererProcess.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseRendererProcess.h"

#include <CefBaseApp.h>
#include <CefBaseRendererProcess.h>


CefBaseRendererProcess::CefBaseRendererProcess(CefBaseApp& app)
   : app_(app)
{
}

CefBaseRendererProcess::~CefBaseRendererProcess() {
    // TODO Auto-generated destructor stub
}

void CefBaseRendererProcess::OnContextReleased(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    for (PointerType& handler: handlers) {
        handler->OnContextReleased(browser,frame,context);
    }
}

bool CefBaseRendererProcess::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
    bool handled = false;
    for ( auto it = handlers.begin();
          !handled && it != handlers.end();
          ++it)
    {
        auto handler = *it;
        handler.get()->OnProcessMessageReceived(browser,source_process,message);
    }

    return handled;
}

CefRefPtr<CefLoadHandler> CefBaseRendererProcess::GetLoadHandler() {
    return app_.GetClient()->GetLoadHandler();
}

void CefBaseRendererProcess::OnRenderThreadCreated(CefRefPtr<CefListValue> v) {
    app_.SetProcessTypeRenderer();
    for (auto& hdlr: handlers) {
        hdlr->OnRenderThreadCreated(v);
    }
}
