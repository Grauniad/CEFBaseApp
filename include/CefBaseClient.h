/*
 * CefBaseClient.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASECLIENT_H_
#define CEFBASECLIENT_H_

#include "include/cef_client.h"
#include "CefBaseLifeSpanHandler.h"
#include "CefBaseRenderHandler.h"
#include "CefBaseRequestHandler.h"
#include "CefBaseDisplayHandler.h"
#include "CefBaseLoadHandler.h"

class CefBaseApp;

#include <vector>

class CefBaseClient: public CefClient {
public:
    typedef std::shared_ptr<CefClient> MessageHandlerPtrType;

    CefBaseClient(CefBaseApp* parent);

    virtual ~CefBaseClient();

    /**********************************************************************
    *                       CEF Client
    **********************************************************************/

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;

    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE;

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;

    /**********************************************************************
    *                       Message Handling
    **********************************************************************/

    void InstallMessagerHandler(MessageHandlerPtrType handler) {
        messageHandlers_.emplace_back(handler);
    }

    /**
     * TODO:
     * Process: Browser
     * THREAD:  UI?
     *
     * Event: A new message is recieved from a different process
     *
     * @param browser         The browser which received the message
     * @param source_process  The PID of the sending process
     * @param message         The message we received
     *
     * Handlers will be notified in the order in which they were installed.
     *
     * @returns TRUE if the message was handled by at least one of the handlers,
     *          false otherwise.
     */
    virtual bool OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message);


    /**********************************************************************
    *                       Accessors
    **********************************************************************/

    CefBaseLifeSpanHandler& LifeSpanHandler();

    CefBaseRenderHandler& RenderHandler();

    CefBaseRequestHandler& RequestHandler();

    CefBaseDisplayHandler& DisplayHandler();

    CefBaseLoadHandler& LoadHandler();

    CefBaseApp& Parent() {
        return *parent_;
    }

private:
    CefBaseApp*                   parent_;
    CefRefPtr<CefLifeSpanHandler> lifeSpanHandler_;
    CefRefPtr<CefRenderHandler>   renderHandler_;
    CefRefPtr<CefRequestHandler>  requestHandler_;
    CefRefPtr<CefDisplayHandler>  displayHandler_;
    CefRefPtr<CefLoadHandler>     loadHandler_;
    typedef std::vector<MessageHandlerPtrType> MessageHandlerContainer;
    MessageHandlerContainer  messageHandlers_;

    IMPLEMENT_REFCOUNTING(CefBaseClient);
};

#endif /* CEFBASECLIENT_H_ */
