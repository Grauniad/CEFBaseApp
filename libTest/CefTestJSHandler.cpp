/*
 * CefTestJSHandler.cpp
 *
 *  Created on: 28 Mar 2015
 *      Author: lhumphreys
 */

#include "CefTestJSHandler.h"
#include "logger.h"
#include <util_time.h>
using namespace std;

class TestLogFunc: public CEFJSUtils::JSFunction {
public:
    static void Install(CefRefPtr<CefV8Context> context) {
        CEFJSUtils::JSFunction::Install(
            context,
            std::unique_ptr<TestLogFunc>(new TestLogFunc()));
    }
    TestLogFunc()
            : JSFunction("DEV_TOOLS_Test_Log")
    {
    }

    virtual void Execute(JSFunctionContext& context) {
        static std::string msg;
        msg.clear();

        if (!context.GetArg(0, msg)) {
            context.RaiseError("No value provided for argument: msg");
        }

        PRINT(Time().Timestamp())
        PRINT(msg)
        PRINT("\n")
    }
};

void CefTestJSHandler::RegisterJSFunctions(CefRefPtr<CefV8Context> context) {
    TestLogFunc::Install(context);
}

CefTestJSHandler::~CefTestJSHandler() {
    // TODO Auto-generated destructor stub
}

