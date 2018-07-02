/*
 * CefBaseBrowserProcess.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEBROWSERPROCESS_H_
#define CEFBASEBROWSERPROCESS_H_

#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"
#include "CefBaseMultiHandler.h"

class CefBaseBrowserProcess:
          public CefBaseMultiHandler<CefBrowserProcessHandler>
{
public:
    CefBaseBrowserProcess();

    /**
     * PROCESS: Browser
     * THREAD:  UI
     *
     * Event: The CEF context (a new CEF application) has been created.
     *
     * CEF has been initialised by the library - now do any application level
     * startup actions (create initial windows etc...)
     */
    CefBaseMultiHandler_Forward(OnContextInitialized);

    virtual ~CefBaseBrowserProcess();
private:
};

#endif /* CEFBASEBROWSERPROCESS_H_ */
