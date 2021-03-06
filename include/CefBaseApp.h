/*
 * CefBaseApp.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEAPP_H_
#define CEFBASEAPP_H_

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "CefBaseBrowserProcess.h"
#include "CefBaseRendererProcess.h"
#include "CefBaseClient.h"
#include <CefBaseIPCExec.h>
#include <atomic>

/**
 * @title CEF Application
 *
 * The base-application in the single access point created by all processes.
 *
 * Application wishing to hook into CEF events should install handlers before
 * calling RunAndExit().
 */
class CefBaseApp: public CefApp {
public:
    CefBaseApp();
    virtual ~CefBaseApp() = default;

    /**********************************************************************
     *                         CEF APP
     **********************************************************************/
    /**
     * PROCESS: Browser
     * THREAD:  ANY
     *
     * Return a reference to the object responsible for handling browser level
     * events
     */
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
            OVERRIDE;

    /**
     * PROCESS: Renderer
     * THREAD:  ANY
     *
     * Return a reference to the object responsible for handling browser level
     * events
     */
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
            OVERRIDE;

    CefRefPtr<CefBaseClient> GetClient() {
        return client_;
    }

    /**********************************************************************
     *                         Accessors
     **********************************************************************/

    CefBaseRendererProcess& Renderer();
    CefBaseBrowserProcess& Browser();

    CefBaseClient& Client();

    CefBaseIPCExec& IPC();

    enum class ProcessId {
        UNKNOWN,
        BROWSER,
        RENDERER
    };
    ProcessId CurrentProcess() const;
    void SetProcessTypeRenderer();
    void SetProcessTypeBrowser();

private:
    std::atomic<ProcessId>              procType_;
    CefRefPtr<CefBrowserProcessHandler> browser_;
    CefRefPtr<CefRenderProcessHandler>  renderer_;
    CefRefPtr<CefBaseClient>            client_;
    std::shared_ptr<CefBaseIPCExec>     ipc_;
    IMPLEMENT_REFCOUNTING(CefBaseApp);
};

#endif /* CEFBASEAPP_H_ */
