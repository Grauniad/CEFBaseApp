/*
 * CefTestApp.cpp
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */

#include <CefTestApp.h>
#include "include/wrapper/cef_helpers.h"
#include <iostream>
#include <cassert>
using namespace std;

/**********************************************************************
*                         App
**********************************************************************/
CefRefPtr<CefBrowser> DummyCefApp::testBrowser;
CefRefPtr<CefV8Context> DummyCefApp::testContext;

void DummyCefApp::RunTestsAndExit(CefRefPtr<DummyCefApp> app) {

    /************************************************************
     *                  Sub-process Handling
     ************************************************************/

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(app->args, app, NULL);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        if(app->handlers->OK()) {
            exit(exit_code);
        } else {
            exit(1);
        }
    }

    /************************************************************
     *                  Main (Browser) Handling
     ************************************************************/

    CefSettings settings;
    settings.windowless_rendering_enabled = 1;
    settings.no_sandbox = 1;

    // Initialize CEF for the browser process.
    CefInitialize(app->args, settings, app, NULL);

    CefRunMessageLoop();

    CefShutdown();

    if ( app->handlers->OK()) {
        exit(0);
    } else {
        exit(1);
    }
}

DummyCefApp::DummyCefApp(int argc, char ** argv, std::string url)
   : CefBaseApp()
   , handlers(new DummyCefAppHandlers(*this, argc, argv, std::move(url)))
   , jsHandler_(new CefTestJSHandler("testQuery","testQueryCancel"))
   , args(argc, argv)
{
    jsHandler_->InstallHandler(*this);
    Renderer().InstallHandler(handlers);
    Browser().InstallHandler(handlers);
}

CefRefPtr<CefBrowser> DummyCefApp::GetTestBrowser() {
    if (DummyCefApp::testBrowser.get()) {
        return DummyCefApp::testBrowser;
    } else {
        throw NoTestBrowserConfigured{};
    }
}

void DummyCefApp::SetTestBrowser(CefRefPtr<CefBrowser> b) {
    if (DummyCefApp::testBrowser.get()) {
        throw TestBrowserAlreadyConfigured{};
    } else {
        DummyCefApp::testBrowser = b;
    }
}

void DummyCefApp::SetTestContext(CefRefPtr<CefV8Context> c) {
    if (testContext.get()) {
        throw TestContextAlreadyConfigured{};
    } else {
        testContext = c;
    }
}

CefRefPtr<CefV8Context> DummyCefApp::GetTestContext() {
    if (testContext.get()) {
        return testContext;
    } else {
        throw NoTestContextConfigured{};
    }
}

