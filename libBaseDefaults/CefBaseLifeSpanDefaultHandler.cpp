/*
 * CefBaseLifeSpanDefaultHandler.cpp
 *
 *  Created on: 8 Feb 2015
 *      Author: lhumphreys
 */

#include "CefBaseLifeSpanDefaultHandler.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"

CefBaseLifeSpanDefaultHandler::CefBaseLifeSpanDefaultHandler()
    : is_closing_(false)
{

}

CefBaseLifeSpanDefaultHandler::~CefBaseLifeSpanDefaultHandler() {
	// TODO Auto-generated destructor stub
}

void CefBaseLifeSpanDefaultHandler::OnAfterCreated(
		CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}

void CefBaseLifeSpanDefaultHandler::OnBeforeClose(
		CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	if (browser_list_.empty()) {
		// All browser windows have closed. Quit the application message loop.
		CefQuitMessageLoop();
	}
}

bool CefBaseLifeSpanDefaultHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void CefBaseLifeSpanDefaultHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
				base::Bind(&CefBaseLifeSpanDefaultHandler::CloseAllBrowsers,
						   this,
						   force_close));
		return;
	}

	if (browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}
