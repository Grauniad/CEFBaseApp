/*
 * CefBaseLifeSpanHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASELIFESPANHANDLER_H_
#define CEFBASELIFESPANHANDLER_H_

#include "include/cef_life_span_handler.h"
#include "CefBaseMultiHandler.h"

class CefBaseLifeSpanHandler: public CefBaseMultiHandler<CefLifeSpanHandler> {
public:
    CefBaseLifeSpanHandler();

    /**
     *  Process: Browser
     *  Thread:  UI
     *
     *  Event: A new browser has been created.
     *
     *  Trigger any actions caused by the creation of a new browser (e.g
     *  tracking of open windows etc.)
     */
    CefBaseMultiHandler_Forward1Ref(OnAfterCreated,CefBrowser);

    /**
     *  Process: Browser
     *  Thread:  UI
     *
     *  Event: An existing browser is about to closed down
     *
     *  Trigger any actions caused by the creation of a new browser (e.g
     *  tracking of open windows etc.)
     */
    CefBaseMultiHandler_Forward1Ref(OnBeforeClose,CefBrowser);

    /**
     * Process: Browser
     * Thread: UI
     *
     * Event: The framework wants to close a window, and must determine how
     *
     * Each handler is called, in order they were installed. If any of the
     * callbacks return true the function returns true, otherwise it returns
     * false.
     */
    bool virtual DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;


    virtual ~CefBaseLifeSpanHandler();
private:
};

#endif /* CEFBASELIFESPANHANDLER_H_ */
