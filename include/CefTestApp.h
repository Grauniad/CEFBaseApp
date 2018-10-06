/*
 * CefTestApp.h
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */

#ifndef CEFTESTAPP_H_
#define CEFTESTAPP_H_

#include "include/cef_app.h"
#include "include/internal/cef_linux.h"
#include "include/internal/cef_types_wrappers.h"
#include "include/cef_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"

#include "CefTests.h"
#include "CefTestAppHandlers.h"
#include "CefTestClient.h"
#include <CefTestJSHandler.h>

#include <functional>
#include <memory>
#include <CefBaseApp.h>

class DefaultTestLogger;


class DummyCefApp: public CefBaseApp {
public:
    DummyCefApp(int argc, char** argv, std::string url);

    virtual ~DummyCefApp() = default;

    /*
     * Execute all tests and return a suitable exit code for the application
     */
    static void RunTestsAndExit(CefRefPtr<DummyCefApp> app);

    CefRefPtr<CefTestJSHandler> JSHandler() { return jsHandler_; }

    static void SetTestBrowser(CefRefPtr<CefBrowser> b);
    static CefRefPtr<CefBrowser> GetTestBrowser();
    struct NoTestBrowserConfigured {};
    struct TestBrowserAlreadyConfigured {};
private:
    static CefRefPtr<CefBrowser> testBrowser;

    typedef std::shared_ptr<DummyCefAppHandlers> PointerType;
    PointerType handlers;

    CefRefPtr<CefTestJSHandler> jsHandler_;
    CefMainArgs args;


IMPLEMENT_REFCOUNTING(DummyCefApp);

};


#endif /* CEFTESTAPP_H_ */
