/*
 * CefBaseApp.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseApp.h"

CefBaseApp::CefBaseApp()
    : browser_(new CefBaseBrowserProcess),
      renderer_(new CefBaseRendererProcess(*this)),
      client_(new CefBaseClient(this))
{
    // TODO Auto-generated constructor stub
}

CefBaseApp::~CefBaseApp() {
    // TODO Auto-generated destructor stub
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
