#include <CefTestApp.h>
#include <CefTests.h>
#include <gtest/gtest.h>
#include <OSTools.h>

int main (int argc, char** argv) {
    const std::string rootPath = OS::Dirname(OS::GetExe());
    const std::string url = "file://" + rootPath + "/index.html";

    CefRefPtr<DummyCefApp> testApp(new DummyCefApp(argc, argv, url));

    DummyCefApp::RunTestsAndExit(testApp);
}

TEST(HELLO_TESTS, TRUE) {
    ASSERT_TRUE(true);
}
