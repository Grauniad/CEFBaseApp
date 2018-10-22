#include <CefTestApp.h>
#include <CefTests.h>
#include <gtest/gtest.h>
#include <CefBaseThread.h>
#include <OSTools.h>
#include <PipePublisher.h>
#include <CefTestJSBaseTest.h>
#include <ReqServer.h>
#include <OpenConnections.h>
#include <WorkerThread.h>

/**
 * The Load Handler receives callbacks on both the Render and the UI process.
 *
 * This allows us to provdie two publishers:
 *    - A PipePublisher publishing to a ring buffer in the tests' local process
 *    - A subscription server, publishing from the browser process
 *
 *  Crucially the root from the browser process to the test thread doesn't pass
 *  through the  TID_RENDERER at all.
 */
class PageLoadHook: public CefLoadHandler
{
public:
    static constexpr int port = 1250;
    static constexpr const char* url = "ws://localhost:1250";

    PageLoadHook()
    {
        if (CefCurrentlyOn(TID_UI)) {
            server.AddHandler(LoadedServer::SUB_NAME, std::make_shared<LoadedServer>());
            serverThread.PostTask([&] () -> void {
                server.HandleRequests(port);
            });
            serverThread.Start();
            server.WaitUntilRunning();
        }
    }

    /**
     * CEF Hook that will be triggered on both the browser and the render process
     */
    void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   int httpStatusCode) override
    {
        if (CefCurrentlyOn(TID_RENDERER)) {
            urlPub.Publish(frame->GetURL());
        } else if (CefCurrentlyOn(TID_UI)) {
            urlSubPub->Publish(frame->GetURL());
        }
    }

    /**
     * Subscription server publishing the urls from the browser process
     */
    class LoadedServer: public SubscriptionHandler {
    public:
        static constexpr const char* SUB_NAME = "SUB_LOADS";
        void OnRequest(RequestHandle hdl) override {
            clients.Add(hdl);
        }
        void Publish (const std::string& url) {
            clients.Publish(url);
        }
    private:
        OpenConnectionsList clients;
    };

    /**
     * Creeat a new client ring buffer for receiving urls on the client thread
     */
   std::shared_ptr<PipeSubscriber<std::string>> NewClient() {
        return urlPub.NewClient(1000);
    }
private:
    PipePublisher<std::string> urlPub;
    WorkerThread serverThread;
    RequestServer server;
    std::shared_ptr<LoadedServer> urlSubPub;
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

};

class RendererNavTest: public NavigateTest {
public:
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

TEST_F(RendererNavTest, NoChange) {
    // Nothing to do...
}

TEST_F(RendererNavTest, PageLoad) {
    auto url = BuildURL("dummy.html");
    RequestNavigate(url);

    std::string current = WaitForPageLoad();

    ASSERT_EQ(current, url);
}

TEST_F(RendererNavTest, HtmlRedirect) {
    std::string url = BuildURL("redirect.html");
    std::string final = BuildURL("index.html");
    RequestNavigate(url);

    std::string first = WaitForPageLoad();
    ASSERT_EQ(first, url);

    std::string current = WaitForPageLoad();
    ASSERT_EQ(current, final);
}
