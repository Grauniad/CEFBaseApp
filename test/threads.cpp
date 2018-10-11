#include <CefTestApp.h>
#include <CefTests.h>
#include <gtest/gtest.h>
#include <CefBaseThread.h>
#include <OSTools.h>

CefBaseApp* app;

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
    app = testApp.get();
    testApp->IPC().Install("GET_PROC_NAME", std::make_shared<GetProcessName>());

    DummyCefApp::RunTestsAndExit(testApp);
}

TEST(GET_RESULT_FROM, RENDERER_PROC_NAME) {
    ASSERT_TRUE(
            CefBaseThread::GetResultFromRender<bool>([=] () -> bool {
                return (app->CurrentProcess() == CefBaseApp::ProcessId::RENDERER);
            })
    );
}

TEST(GET_RESULT_FROM, RENDERER) {
    ASSERT_TRUE(
        CefBaseThread::GetResultFromRender<bool>([=] () -> bool {
            return CefCurrentlyOn(TID_RENDERER);
        })
    );
}

TEST(GET_RESULT_FROM, SAME_THREAD) {
    ASSERT_TRUE(
            CefBaseThread::GetResultFromRender<bool>([=] () -> bool {
                return CefBaseThread::GetResultFromRender<bool>([=] () -> bool {
                    return CefCurrentlyOn(TID_RENDERER);
                });
            })
    );
}

TEST(GET_RESULT_FROM, RENDERER_TO_UI_NOT_SUPPORTED) {
    ASSERT_THROW(
        CefBaseThread::GetResultFromUI<bool>([=] () -> bool {
            return CefCurrentlyOn(TID_UI);
        })
    ,
        CefBaseThread::UnsupportedIPCRequested
    );
}

class PostTo: public ::testing::Test {
protected:
    PostTo() : futureResult(result.get_future()) {}

    void SetResult(int val) {
        result.set_value(val);
    }

    void WaitForResult(const int& expected) {
        ASSERT_EQ(expected, futureResult.get());
    }

    std::promise<int> result;
    std::future<int> futureResult;
};

TEST_F(PostTo, RENDERER) {
    CefBaseThread::PostToCEFThread(TID_RENDERER, [&] () -> void {
        SetResult(3);
    });

    WaitForResult(3);
}

TEST_F(PostTo, RENDERER_IN_50MS) {
    Time start;
    CefBaseThread::PostToCEFThread(TID_RENDERER, [&] () -> void {
        SetResult(3);
    }, 50);

    WaitForResult(3);
    Time end;
    ASSERT_GT(end.DiffUSecs(start), 50 * 1000);
    ASSERT_LT(end.DiffUSecs(start), 51 * 1000);
}

TEST_F(PostTo, SAME_THREAD) {
    CefBaseThread::PostToCEFThread(TID_RENDERER, [&] () -> void {
        CefBaseThread::PostToCEFThread(TID_RENDERER, [&] () -> void {
            SetResult(3);
        });
    });

    WaitForResult(3);
}

TEST_F(PostTo, NO_IPC_CALL) {
    ASSERT_THROW(
        CefBaseThread::PostToCEFThread(TID_UI, [&]() -> void { }),
        CefBaseThread::UnsupportedIPCRequested);
}
