/*
 * CefBaseDisplayHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEDISPLAYHANDLER_H_
#define CEFBASEDISPLAYHANDLER_H_

#include "CefBaseMultiHandler.h"
#include "include/cef_display_handler.h"

class CefBaseDisplayHandler: public CefBaseMultiHandler<CefDisplayHandler> {
public:
    CefBaseDisplayHandler();
    virtual ~CefBaseDisplayHandler();

	/**
	 * Process: Browser
	 * Thread:  UI
	 *
	 * Event: The title of the window has changed, must notify native framework
	 *        GUI...
	 *
	 * @param browser   The browser whose title has changed
	 * @param title     The updated title text
	 */
    void OnTitleChange(CefRefPtr<CefBrowser>, const CefString& title);
};

#endif /* CEFBASEDISPLAYHANDLER_H_ */
