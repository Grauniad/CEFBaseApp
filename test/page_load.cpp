#include <CefTestApp.h>
#include <CefTests.h>
#include <gtest/gtest.h>
#include <CefBaseThread.h>
#include <OSTools.h>
#include <PipePublisher.h>
#include <CefTestJSBaseTest.h>

class PageLoadHook: public CefLoadHandler
{
public:
    void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode) override
   {
        if (CefCurrentlyOn(TID_RENDERER)) {
            urlPub.Publish(frame->GetURL());
        }
   }

   std::shared_ptr<PipeSubscriber<std::string>> NewClient() {
        return urlPub.NewClient(1000);
    }
private:
    PipePublisher<std::string> urlPub;
    IMPLEMENT_REFCOUNTING(PageLoadHook);
};

// Hacky global vars to ensure tests have acess...
CefBaseApp* app;
std::shared_ptr<PipeSubscriber<std::string>> urls;
std::string rootPath;


int main (int argc, char** argv) {
    rootPath = OS::Dirname(OS::GetExe());
    const std::string url = "file://" + rootPath + "/index.html";

    // Create a new test app to run the tests
    CefRefPtr<DummyCefApp> testApp(new DummyCefApp(argc, argv, url));
    app = testApp.get();

    // Install a hook to capture each new page...
    auto urlHdlr = std::make_shared<PageLoadHook>();
    urls = urlHdlr->NewClient();

    app->GetClient()->LoadHandler().InstallHandler(std::move(urlHdlr));

    DummyCefApp::RunTestsAndExit(testApp);
}

class NavigateTest: public JSTestBase {
public:
    /***********************************************************
     *  JSTestBase required API
     ***********************************************************/
    CefBaseApp& App() override {
        return *app;
    }

    CefRefPtr<CefBrowser> TestBrowser() override {
        return DummyCefApp::GetTestBrowser();
    }

    CefRefPtr<CefV8Context> TestContext() override {
        return DummyCefApp::GetTestContext();
    }

    /***********************************************************
     *  Test Utilities
     ***********************************************************/

    /**
     * Build up a file URL based on the test HTML file
     *
     * @param file The file to get the URL for
     *
     * @return The url
     */
    static std::string BuildURL(const std::string& file) {
        return "file://" + rootPath + "/" + file;
    }

    void RequestNavigate(std::string url) {
        CefBaseThread::PostToCEFThread(TID_RENDERER, [=] () -> void {
            TestBrowser()->GetMainFrame()->LoadURL(url);
        });
    }

    // Wait until we've seen another url
    static std::string WaitForPageLoad() {
        std::string url;
        urls->WaitForMessage(url);

        return url;
    }

    /***********************************************************
     *  Test Setup / Tear down
     ***********************************************************/

    static  void SetUpTestCase() {
        auto url = BuildURL("index.html");
        std::string current;
        while (current != url) {
            current = WaitForPageLoad();
        }
    }


    void SetUp() override {
        auto url = BuildURL("index.html");
        RequestNavigate(url);
        std::string current;
        while (current != url) {
            current = WaitForPageLoad();
        }
    }

    void TearDown() override {
        JSTestBase::TearDown();
        std::string url;
        // Check that we exhausted the published pages...
        ASSERT_FALSE(urls->GetNextMessage(url)) << url;
    }
};

TEST_F(NavigateTest, NoChange) {
    // Nothing to do...
}

TEST_F(NavigateTest, PageLoad) {
    auto url = BuildURL("dummy.html");
    RequestNavigate(url);

    std::string current = WaitForPageLoad();

    ASSERT_EQ(current, url);
}

TEST_F(NavigateTest, HtmlRedirect) {
    std::string url = BuildURL("redirect.html");
    std::string final = BuildURL("index.html");
    RequestNavigate(url);

    std::string first = WaitForPageLoad();
    ASSERT_EQ(first, url);

    std::string current = WaitForPageLoad();
    ASSERT_EQ(current, final);
}
