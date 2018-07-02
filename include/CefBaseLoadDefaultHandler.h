/*
 * GCGV_Callbacks_Loading.h
 *
 *  Created on: 8 Feb 2015
 *      Author: lhumphreys
 */

#ifndef CEF_BASE_DEFAULTS_LOADING_H_
#define CEF_BASE_DEFAULTS_LOADING_H_

#include <include/cef_load_handler.h>

/**
 * Handle events related to page loading ( although note that this could be
 * individual gets / files within the page).
 *
 */
class CefBaseLoadDefaultHandler: public CefLoadHandler {
public:
	/**
	 * TODO: Verify below
	 * Process: Browser
	 * Thread:  UI
	 *
	 * Event: A load has failed on the on page.
	 *
	 * If the load was the main page ( not some internal get) put some useful error
	 * text in the frame.
	 */
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;
private:

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CefBaseLoadDefaultHandler);
};

#endif /* GCGV_CALLBACKS_LOADING_H_ */
