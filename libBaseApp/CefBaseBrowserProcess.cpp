/*
 * CefBaseBrowserProcess.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include <CefBaseBrowserProcess.h>
#include <CefBaseApp.h>
#include <CefBaseCookieMgr.h>
#include <include/cef_cookie.h>

CefBaseBrowserProcess::CefBaseBrowserProcess(CefBaseApp& app)
   : app_(app)
{
}

CefBaseBrowserProcess::~CefBaseBrowserProcess() {
}

void CefBaseBrowserProcess::OnContextInitialized() {
    app_.SetProcessTypeBrowser();
    ForwardToHandlers(&CefBrowserProcessHandler::OnContextInitialized);
}
