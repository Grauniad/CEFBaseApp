/*
 * CefTest.cpp
 *
 *  Created on: 24 Mar 2015
 *      Author: lhumphreys
 */

#include "CefTests.h"
/*
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_callback.h"
#include "CefBaseThread.h"

#include <iostream>
*/
//#include "CefTestApp.h"

#include "include/wrapper/cef_helpers.h"
#include <env.h>
#include <iostream>
#include <iomanip>

#include "CefBaseThread.h"

CefTestContext::CefTestContext(CefRefPtr<CefV8Context> context, testLogger& log)
        : context_(context), log_(log)
{
}


void CefTestContext::ExecuteCleanJS(
    const std::string& code,
    const std::string& exp_result)
{
    Log() << "Executing code: " << endl;
    Log() << "  >" << code << "<" <<endl;
    Log() << "Expected result: " << endl;
    Log() << "  >" << exp_result << endl;

    struct JS_RESULT{
        JS_RESULT() : error("") { }

        std::string result;
        std::string error;
    };

    /**
     * We should only execute code in the java-script thread, and CefV8Values
     * may only be accessed from the thread they were created in.
     */
    JS_RESULT result = CefBaseThread::GetResultFromRender<JS_RESULT>(
    [&] () -> JS_RESULT {
        JS_RESULT result;

        CefString js(code);
        CefRefPtr<CefV8Value> jsResult;
        CefRefPtr<CefV8Exception> jsError;

        bool ok = context_->Eval(js,"", 0, jsResult,jsError);

        if ( ok )
        {
            result.result = jsResult->GetStringValue();
        }
        else
        {
            result.error = CefTestUnexpectedJSException(jsError).Dump();
        }

        return result;
    });

    if (result.error == "") {
        if (result.result == exp_result) {
            Log() << "Execution OK!" << endl;
        } else {
            throw CefTestWrongResult(exp_result, result.result);
        }
    } else {
        throw CefTestGenericException(result.error);
    }
}

CefTestContext::CefTestUnexpectedJSException::CefTestUnexpectedJSException(
    CefRefPtr<CefV8Exception> jsError)
{
    msg = jsError->GetMessage();

    stringstream strbuf;
    strbuf << "Error at "
            << std::string(jsError->GetScriptResourceName())
            << ":" << jsError->GetLineNumber() << endl;
    strbuf << endl;

    strbuf << std::string(jsError->GetSourceLine()) << endl;

    strbuf << setw(jsError->GetStartColumn()) << "^";

    int distance = jsError->GetEndColumn() - jsError->GetStartColumn() -1;

    if ( distance > 0 ) {
        strbuf << setw(distance) << setfill('-') << "-";
    }

    strbuf << "^" << endl;

    this->trace = strbuf.str();
}

std::string CefTestContext::CefTestUnexpectedJSException::Dump() const {
    static std::stringstream buf;

    buf.str("");
    buf.clear();

    buf << "Received unexpected Java-script exception: " << endl;
    buf << "   " << msg << endl;
    buf << "Trace: " << endl << endl;
    buf << trace;

    return buf.str();
}

std::string CefTestContext::CefTestWrongResult::Dump() const {
    static std::stringstream buf;

    buf.str("");
    buf.clear();

    buf << "Wrong value received when executing: " << endl;
    buf << "   Expected: >" << expected << "<" << endl;
    buf << "   Actual:   >" << actual   << "<" << endl;

    return buf.str();
}

std::string CefTestContext::CefTestWaitTimeOutException::Dump() const {
    static std::stringstream buf;

    buf.str("");
    buf.clear();

    buf << "Task failed to return true:" << endl;
    buf << "   Task: >" << task << "<" << endl;
    buf << "   error: " << endl
        << error << endl;

    return buf.str();
}
