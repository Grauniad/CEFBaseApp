/*
 * CefBaseApp.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include <CefBaseApp.h>

#include "CefBaseApp.h"

CefBaseApp::CefBaseApp()
    : procType_(ProcessId::UNKNOWN),
      browser_(new CefBaseBrowserProcess(*this)),
      renderer_(new CefBaseRendererProcess(*this)),
      client_(new CefBaseClient(this))
{
    ipc_ = CefBaseIPCExec::Install(*this);
}

CefRefPtr<CefBrowserProcessHandler> CefBaseApp::GetBrowserProcessHandler() {
    return browser_;
}

CefRefPtr<CefRenderProcessHandler> CefBaseApp::GetRenderProcessHandler() {
    return renderer_;
}

CefBaseRendererProcess& CefBaseApp::Renderer() {
    return static_cast<CefBaseRendererProcess&>(*renderer_.get());
}

CefBaseBrowserProcess& CefBaseApp::Browser() {
    return static_cast<CefBaseBrowserProcess&>(*browser_.get());
}

CefBaseClient& CefBaseApp::Client() {
    return static_cast<CefBaseClient&>(*client_.get());
}

CefBaseIPCExec& CefBaseApp::IPC() {
    return *ipc_;
}

CefBaseApp::ProcessId CefBaseApp::CurrentProcess() const {
    return procType_;
}

void CefBaseApp::SetProcessTypeRenderer() {
    procType_= ProcessId::RENDERER;
}

void CefBaseApp::SetProcessTypeBrowser() {
    procType_= ProcessId::BROWSER;
}
