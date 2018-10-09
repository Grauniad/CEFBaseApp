#include <CefTestJSBaseTest.h>

#include <CefBaseThread.h>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

void JSTestBase::ClearCookies() {
    std::string code = R"JS(
    (function () {
        let cookies = document.cookie.split(';');
        for (let i = 0; i < cookies.length; i +=1) {
            let cookie = cookies[i];
            document.cookie = cookie + '=; expires=Thu, 01 Jan 1970 00:00:01 GMT';
        }
    })();

    document.cookie.toString()
    )JS";
    ExecuteCleanJS(code, "");
}

void JSTestBase::ExecuteCleanJS(std::string code, std::string expected)
{
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

                bool ok = TestContext()->Eval(js,"", 0, jsResult,jsError);

                if ( ok )
                {
                    result.result = jsResult->GetStringValue();
                }
                else
                {
                    std::string msg = jsError->GetMessage();

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

                    result.error = msg + "\n\n" + strbuf.str();
                }

                return result;
            });

    ASSERT_EQ(result.error, "");
    ASSERT_EQ(result.result, expected);
}

