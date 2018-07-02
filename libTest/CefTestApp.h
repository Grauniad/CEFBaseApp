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
    static DummyCefApp& Instance();

    virtual ~DummyCefApp() {}

    /*
     * Create a test to execute a standard function and add it to
     * the internal list.
     *
     * IMPORTANT: for this to work this must point to a function loaded into
     *            the same address in all (sub)processes...
     *
     */
    void AddTest(const std::string& name, SimpleCefTest::TestFun* fptr);

    /*
     * Execute all tests and return a suitable exit code for the application
     */
    void RunTestsAndExit(int argc, char** argv);

    CefRefPtr<CefTestJSHandler> JSHandler() { return jsHandler_; }

private:
    DummyCefApp();

    typedef std::shared_ptr<DummyCefAppHandlers> PointerType;
    PointerType handlers;

    CefRefPtr<CefTestJSHandler> jsHandler_;


IMPLEMENT_REFCOUNTING(DummyCefApp);

};


#endif /* CEFTESTAPP_H_ */
