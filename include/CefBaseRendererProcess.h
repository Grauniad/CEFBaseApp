/*
 * CefBaseRendererProcess.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASERENDERERPROCESS_H_
#define CEFBASERENDERERPROCESS_H_

#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"

#include "CefBaseClient.h"
#include "CefBaseMultiHandler.h"

class CefBaseRendererProcess:
        public CefBaseMultiHandler<CefRenderProcessHandler>
{
public:
    CefBaseRendererProcess(CefBaseApp& app);

    virtual ~CefBaseRendererProcess();
    /**
     * PROCESS: Renderer
     * THREAD:  RENDERER
     *
     * Event: The render process main thread has been created...
     *
     */
    void OnRenderThreadCreated(CefRefPtr<CefListValue>) override;

    /**
     * PROCESS: Renderer
     * THREAD:  RENDERER
     *
     * Event: The java script (v8) context has been created
     *
     * This callback is called immediately after a new java script context has
     * been created, and can be used to set up any variables or install new
     * handlers.
     *
     * @param CefBrowser    The browser in which the js context was created
     * @param CefFrame      The frame (within the browser) in which the js
     *                      context was created
     * @param CefV8Context  The newly created JS context.
     */
    CefBaseMultiHandler_Forward3Refs(
        OnContextCreated, CefBrowser, CefFrame, CefV8Context);

    /**
     * PROCESS: Renderer
     * THREAD:  RENDERER
     *
     * Event: The java script (v8) context is about to be cleaned up
     *
     * This callback is called immediately before an existing java script
     * context is to be released. Any references to the context should now be
     * cleaned up.
     *
     * @param CefBrowser    The browser in which the js context was created
     * @param CefFrame      The frame (within the browser) in which the js
     *                      context was created
     * @param CefV8Context  The newly created JS context.
     */
  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,

                                 CefRefPtr<CefV8Context> context);


  ///
  // Called when a new message is received from a different process. Return true
  // if the message was handled or false otherwise. Do not keep a reference to
  // or attempt to access the message outside of this callback.
  ///
  /*--cef()--*/
    /**
     * PROCESS: Renderer
     * THREAD:  RENDERER
     *
     * Event: The renderer process has received an IPC message
     *
     * This callback is called on receipt of a new message. Handlers should
     * return true if they have dealt with the message, or false if it should be
     * forwarded on to others.
     *
     * As per the CEF documentation:
     *     Do not keep a reference to or attempt to access the message outside
     *     of this callback.
     *
     * @param CefBrowser      The browser receiving the message
     * @param source_process  The sender
     * @param message         The message content
     */
  virtual bool OnProcessMessageReceived(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefProcessId source_process,
      CefRefPtr<CefProcessMessage> message) OVERRIDE;

  CefRefPtr<CefLoadHandler> GetLoadHandler() override;
private:
    CefBaseApp& app_;
};
#endif /* CEFBASERENDERERPROCESS_H_ */
