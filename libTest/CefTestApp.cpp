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
#include <tester.h>
using namespace std;

/**********************************************************************
*                         App
**********************************************************************/

void DummyCefApp::RunTestsAndExit(int argc, char **argv) {
    // Provide CEF with command-line arguments.
    CefMainArgs main_args(argc, argv);

    // SimpleApp implements application-level callbacks. It will create the first
    // browser instance in OnContextInitialized() after CEF has initialized.
    CefRefPtr<DummyCefApp> This(this);

    /************************************************************
     *                  Sub-process Handling
     ************************************************************/

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, This, NULL);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        if(handlers->OK()) {
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

    // Initialize CEF for the browser process.
    CefInitialize(main_args, settings, This, NULL);

    CefRunMessageLoop();

    CefShutdown();

    if ( handlers->OK()) {
        exit(0);
    } else {
        exit(1);
    }
}

DummyCefApp::DummyCefApp()
   : handlers(new DummyCefAppHandlers),
     jsHandler_(new CefTestJSHandler("testQuery","testQueryCancel"))
{
    jsHandler_->InstallHandler(*this);
    Renderer().InstallHandler(handlers);
    Browser().InstallHandler(handlers);
}

DummyCefApp& DummyCefApp::Instance() {
    static DummyCefApp app;
    return app;
}

void DummyCefApp::AddTest(const std::string& name, SimpleCefTest::TestFun* fptr)
{
    std::unique_ptr<CefTestBase> test(new SimpleCefTest(name,fptr));
    handlers->AddTest(std::move(test));
}

