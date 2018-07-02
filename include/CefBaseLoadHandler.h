/*
 * CefBaseLoadHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASELOADHANDLER_H_
#define CEFBASELOADHANDLER_H_

#include "CefBaseMultiHandler.h"
#include "include/cef_load_handler.h"
#include <iostream>
#include <fstream>

class CefBaseLoadHandler: public CefBaseMultiHandler<CefLoadHandler> {
public:
    CefBaseLoadHandler();
    virtual ~CefBaseLoadHandler();

    CefBaseMultiHandler_Forward3(OnLoadEnd, CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, int);


    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                              bool isLoading,
                              bool canGoBack,
                              bool canGoForward) override;

	/**
	 * TODO: CHECK
	 * Process: Browser
	 * Thread: UI
	 *
	 * Event: Load for a navigation fails, or is cancelled.
	 */
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;
};

#endif /* CEFBASELOADHANDLER_H_ */
