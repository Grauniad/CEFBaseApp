#ifndef CEFBASEAPP_CEFBASEJSBASETEST_H
#define CEFBASEAPP_CEFBASEJSBASETEST_H

#include <gtest/gtest.h>
#include <include/cef_v8.h>

#include <CefBaseApp.h>

class JSTestBase: public ::testing::Test {
public:
    //API Accessors required to be implemented by the individual test case
    virtual CefRefPtr<CefV8Context> TestContext() = 0;

    virtual CefRefPtr<CefBrowser> TestBrowser() = 0;

    virtual CefBaseApp& App() = 0;

    void ClearCookies();

    void ExecuteCleanJS(std::string code, std::string expected);
};
#endif //CEFBASEAPP_CEFBASEJSBASETEST_H
