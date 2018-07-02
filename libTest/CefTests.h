#ifndef CEFTESTAPP_TESTS_H_
#define CEFTESTAPP_TESTS_H_

#include <tester.h>
#include "CefBaseJSMessageRouter.h"
#include <CefBaseApp.h>
#include <util_time.h>
#include <iostream>
#include <thread>
#include <chrono>

class CefTestContext {
public:
    CefTestContext(CefRefPtr<CefV8Context> context, testLogger& log);

    struct CefTestBaseException {
        virtual ~CefTestBaseException() { } ;
        virtual std::string Dump() const = 0;
    };

    struct CefTestGenericException: public CefTestBaseException {
        CefTestGenericException(const std::string& msg): msg(msg) { }

        virtual std::string Dump() const { return msg; }
        std::string msg;
    };

    struct CefTestUnexpectedJSException: public CefTestBaseException {
        CefTestUnexpectedJSException(CefRefPtr<CefV8Exception> jsError);

        ~CefTestUnexpectedJSException () { }

        virtual std::string Dump() const;

        std::string msg;
        std::string trace;
    };

    struct CefTestWrongResult: public CefTestBaseException {
        CefTestWrongResult (const std::string& exp, const std::string& act)
            : expected(exp), actual(act) { }

        ~CefTestWrongResult() { }

        virtual std::string Dump() const;

        std::string expected;
        std::string actual;
    };

    struct CefTestWaitTimeOutException: public CefTestBaseException {
        CefTestWaitTimeOutException (const std::string& descr, const std::string& error)
            : task(descr), error(error) { }

        ~CefTestWaitTimeOutException() { }

        virtual std::string Dump() const;

        std::string task;
        std::string error;
    };

    /**
     * Execute java script code in the current context, demanding a specific
     * result.
     */
    void ExecuteCleanJS(const std::string& code, const std::string& result);

    /**
     * Keep retrying a function f() until it returns the correct result. If
     * it returns an incorrect result after maxWaitUS has passed an error will
     * be thrown.
     *
     * The timeout does NOT behave as an interrupt and a non-returning f() will
     * lock the test forever.
     *
     * By default, the thread does not sleep between retries, but execution is
     * yielded to give the render an opportunity to perform any actions.
     *
     * If a value for sleepUS is provided, the test will sleep for at least than
     * many micro-seconds, rather than yielding directly, before testing the
     * function again
     */
    template<class F>
    void WaitForResult(
        F f,
        unsigned maxWaitUS,
        const std::string& descr,
        unsigned sleepUS = 0
        );

    /**
     * Return the log object
     */
    testLogger& Log() { return log_; }

private:
    CefRefPtr<CefV8Context> context_;
    testLogger&              log_;
};

class CefTestBase {
public:
    /**
     * Will be invoked on the renderer thread when the test should be started.
     *
     * If the test needs to run in the browser, a call to DoOnUI should be made
     */
    virtual int RunTest(CefTestContext& context) = 0;

    virtual const std::string& Name() const = 0;

    virtual ~CefTestBase() { } 
protected:

private:
};

class SimpleCefTest: public CefTestBase {
public:
    typedef int (TestFun) (CefTestContext& context);
    SimpleCefTest(const std::string& testName, TestFun* fptr)
       : name(testName), func(fptr) { }

    virtual int RunTest(CefTestContext& context) {
       return (*func)(context);
    }

    virtual const std::string& Name() const {
        return name;
    }

private:
    std::string name;
    TestFun* func;
};

template<class F>
void CefTestContext::WaitForResult(
    F f,
    unsigned maxWaitUS,
    const std::string& descr,
    unsigned sleepUS)
{
    long unsigned i = 0;
    Time start;
    Time stop;

    std::string errorMsg = "";


    bool ok = false;

    for (Time stop; !ok && stop.DiffUSecs(start) < maxWaitUS; stop.SetNow())
    {
        Log() << stop.Timestamp() << ", waiting for: " << descr << endl;
        errorMsg = "";
        ++i;
        try {
            ok = f();
        } catch (const CefTestBaseException& e) {
            ok = false;
            errorMsg = e.Dump();
            Log() << "Caught exception whilst executing: "
                  << "   " << descr << endl;
            Log() << "Error was: " << endl
                  << errorMsg;
        }

        if ( !ok) {
            Log() << ", Still waiting for: " << descr << endl;
            if ( sleepUS > 0 ) {
                Log() << "I will now sleep..." << endl;
                std::this_thread::sleep_for(std::chrono::microseconds(sleepUS));

            } else {
                Log() << "I will now yield..." << endl;
                std::this_thread::yield();
            }
        }
    }

    if (ok)
    {
        Log() << stop.Timestamp() << ": OK" << descr << endl;
    }
    else
    {
        Log() << "***********************************************" << endl;
        throw CefTestWaitTimeOutException(descr,errorMsg);
    }
}

#endif
