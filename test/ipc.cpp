#include <CefTestApp.h>
#include <CefTests.h>
#include <gtest/gtest.h>
#include <CefBaseThread.h>
#include <CefBaseIPCExec.h>
#include <CefBaseIPCVia.h>
#include <OSTools.h>

CefBaseApp* app = nullptr;

class GetProcessName: public CefBaseIPCExec::IIPC_TriggeredFunction {
public:
    std::string Execute(CefBaseApp& app, std::string arg) override {
        switch (app.CurrentProcess())
        {
            case CefBaseApp::ProcessId::UNKNOWN:
                return "UNKNOWN";
            case CefBaseApp::ProcessId::BROWSER:
                return "BROWSER";
            case CefBaseApp::ProcessId::RENDERER:
                return "RENDERER";
        }
        return "";
    }
};

int main (int argc, char** argv) {
    const std::string rootPath = OS::Dirname(OS::GetExe());
    const std::string url = "file://" + rootPath + "/index.html";
    CefRefPtr<DummyCefApp> testApp(new DummyCefApp(argc, argv, url));
    testApp->IPC().Install("GET_PROC_NAME", std::make_shared<GetProcessName>());

    app = testApp;

    DummyCefApp::RunTestsAndExit(testApp);
}


class IPC: public ::testing::Test {
protected:
    IPC() : futureResult(result.get_future()) {}

    CefRefPtr<CefBrowser> Browser() {
        return DummyCefApp::GetTestBrowser();
    }

    CefBaseApp& App() {
        return *app;
    }

    void SetResult(std::string val) {
        result.set_value(val);
    }

    void WaitForResult(const std::string& expected) {
        ASSERT_EQ(expected, futureResult.get());
    }

    std::promise<std::string> result;
    std::future<std::string> futureResult;
};

TEST_F(IPC, Ping) {
    App().IPC().Ping(PID_BROWSER, Browser(), [&] (std::string r) -> void {
        SetResult(r);
    });
    WaitForResult("PONG");
}

TEST_F(IPC, GET_BROWSER_PID) {
    App().IPC().Execute(
            PID_BROWSER,
            Browser(),
            "GET_PROC_NAME",
            "",
            [&] (std::string r) -> void
            {
                SetResult(r);
            } );
    WaitForResult("BROWSER");
}

TEST_F(IPC, GET_RENDERER_PID) {
    App().IPC().Execute(
            PID_RENDERER,
            Browser(),
            "GET_PROC_NAME",
            "",
            [&] (std::string r) -> void
            {
                SetResult(r);
            } );
    WaitForResult("RENDERER");
}

TEST_F(IPC, GET_RENDERER_PID_VIA_BROWSER) {
    auto encoder = CefBaseIPCVia::NewEncoder();
    App().IPC().Execute(
            PID_BROWSER,
            Browser(),
            CefBaseIPCVia::NAME,
            encoder->Encode({PID_RENDERER, "GET_PROC_NAME", ""}),
            [&] (std::string r) -> void
            {
                SetResult(r);
            });
    WaitForResult("RENDERER");
}

TEST_F(IPC, GET_BROWSER_PID_VIA_BROWSER) {
    auto encoder = CefBaseIPCVia::NewEncoder();
    App().IPC().Execute(
            PID_BROWSER,
            Browser(),
            CefBaseIPCVia::NAME,
            encoder->Encode({PID_BROWSER, "GET_PROC_NAME", ""}),
            [&] (std::string r) -> void
            {
                SetResult(r);
            });
    WaitForResult("BROWSER");
}

