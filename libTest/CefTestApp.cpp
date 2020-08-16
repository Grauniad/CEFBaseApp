/*
 * CefTestApp.cpp
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */

#include <CefTestApp.h>
#include "include/wrapper/cef_helpers.h"
#include <iostream>
#include <iomanip>
#include <logger.h>
#include <unistd.h>
#include <cassert>
using namespace std;

/**********************************************************************
*                         App
**********************************************************************/
CefRefPtr<CefBrowser> DummyCefApp::testBrowser;
CefRefPtr<CefV8Context> DummyCefApp::testContext;

void DummyCefApp::RunTestsAndExit(CefRefPtr<DummyCefApp> app) {

    bool logging = false;
    IF_LOG(LOG_OVERVIEW, logging = true;)
    std::string pid = "";

    /************************************************************
     *                  Sub-process Handling
     ************************************************************/

    if (logging) {
        std::stringstream pidPrefix;
        pidPrefix << "[" << getpid() << "] ";
        pid = pidPrefix.str();
    }

    const auto LogLine = [&] (const std::string& msg) -> void {
        std::string result = pid;
        result += msg;
        result += "\n";

        LOG_FROM(LOG_OVERVIEW, "RunTestsAndExit", result);
    };

    if (logging) {
        std::stringstream args;
        args << pid << "New Process: " << std::endl;
        for (int i = 0; i < app->args.argc; ++i) {
            args << pid << "   ARGV[" << std::setw(2) << std::setfill('0') << i << "] : " << app->args.argv[i]
                 << std::endl;
        }
        LOG_FROM(LOG_OVERVIEW, "RunTestsAndExit", args.str());
    }


    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(app->args, app, NULL);

    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        if(app->handlers->OK()) {
            if (logging) {
                std::stringstream msg;
                msg << "Process shutdown with exit code: " << exit_code;
                LogLine(msg.str());
            }
            exit(exit_code);
        } else {
            if (logging) {
                LogLine("Process shutdown in error");
            }
            exit(1);
        }
    }

    /************************************************************
     *                  Main (Browser) Handling
     ************************************************************/

    CefSettings settings;
    settings.windowless_rendering_enabled = 1;
    settings.no_sandbox = 1;
    if (logging) {
        settings.log_severity = LOGSEVERITY_DEBUG;
    }

    if (logging) {
        LogLine("(MAIN) Initialising Browser Process");
    }

    // Initialize CEF for the browser process.
    CefInitialize(app->args, settings, app, NULL);

    if (logging) {
        LogLine("(MAIN) Running Message Loop");
    }

    CefRunMessageLoop();

    if (logging) {
        LogLine("(MAIN) Messge loop complete - shutting down");
    }

    CefShutdown();

    if (logging) {
        LogLine("(MAIN) CefShutdown complete");
    }

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
void DummyCefApp::ClearTestBrowser() {
    DummyCefApp::testBrowser = nullptr;
}


void DummyCefApp::SetTestContext(CefRefPtr<CefV8Context> c) {
    testContext = c;
}

CefRefPtr<CefV8Context> DummyCefApp::GetTestContext() {
    if (testContext.get()) {
        return testContext;
    } else {
        throw NoTestContextConfigured{};
    }
}

