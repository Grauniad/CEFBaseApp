/*
 * CefBaseRequestReplies.h
 *
 *  Created on: 29 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEREQUESTREPLIES_H_
#define CEFBASEREQUESTREPLIES_H_


#include <string>
#include <include/wrapper/cef_message_router.h>
#include <include/cef_browser_process_handler.h>

class CefBaseJSRequestReply {
public:
    virtual ~CefBaseJSRequestReply() { }

    struct RequestContext {
        const std::string& request;
        CefRefPtr<CefBrowser> browser;
    };

    /**
     * Process: Browser
     * Thread:  UI
     *
     * Event: A new OnQuery callback has been triggered from a js context
     *
     * To be implemented by the handler.
     *
     * In the event of an error, an instance of CefBaseInvalidRequestException
     * should be thrown.
     *
     * @param request   The request message (with the header removed)
     *
     * @returns The string response
     */
    virtual std::string OnRequest(RequestContext& context) = 0;


    struct CefBaseInvalidRequestException {
        int code;
        std::string errMsg;
    };

    struct CefBaseAbdandonRequest { };
protected:
    /**
     * WARNING: Only valid during an ongoing call to OnRequest
     */
    CefRefPtr<CefBrowser> Browser();
};

class CefBaseJSRequestReplyHandler: public CefMessageRouterBrowserSide::Handler, public CefBaseRefCounted
{
public:
    CefBaseJSRequestReplyHandler() { }

    typedef std::shared_ptr<CefBaseJSRequestReply> HandlerPointer;

    /**
     * Attempt to install a new handler with name reqName. If a handler for this
     * request has already been installed the installation will fail.
     *
     * Requests with a name of REQ_NAME can be triggered by sending a request
     * where the first (space separated) word is  REQUEST_REQ_NAME.
     */
    bool InstallHandler(const std::string& name, HandlerPointer handler );

    template<class HANDLER>
    bool Install(const std::string& name) {
        HandlerPointer handler(new HANDLER);
        return InstallHandler(name,handler);
    }


    /**************************************************************************
     *                      RAW CEF Interface
     *************************************************************************/

    ///
    // Executed when a new query is received. |query_id| uniquely identifies the
    // query for the life span of the router. Return true to handle the query
    // or false to propagate the query to other registered handlers, if any. If
    // no handlers return true from this method then the query will be
    // automatically canceled with an error code of -1 delivered to the
    // JavaScript onFailure callback. If this method returns true then a
    // Callback method must be executed either in this method or asynchronously
    // to complete the query.
    ///
    virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         int64 query_id,
                         const CefString& request,
                         bool persistent,
                         CefRefPtr<Callback> callback);

    ///
    // Executed when a query has been cancelled either explicitly using the
    // JavaScript cancel function or implicitly due to browser destruction,
    // navigation or renderer process termination. It will only be called for
    // the single handler that returned true from OnQuery for the same
    // |query_id|. No references to the associated Callback object should be
    // kept after this method is called, nor should any Callback methods be
    // executed.
    ///
    virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 int64 query_id);
private:
    CefBaseJSRequestReplyHandler(const CefBaseJSRequestReplyHandler&) = delete;

    /**
     * Find the handler for reqString, and get the response
     */
    bool GetResponse(
        const std::string& reqString,
        const CefRefPtr<Callback>& callback,
        CefRefPtr<CefBrowser> browser);

    /**
     * Trigger the provided handler for a response to callback, getting the
     * args from request
     */
    bool GetResponse(
        const std::string& request,
        const std::string& name,
        const CefRefPtr<Callback> callback,
        CefRefPtr<CefBrowser> browser,
        CefBaseJSRequestReply& handler);


    typedef std::map<std::string ,HandlerPointer> HandlerMap;
    HandlerMap handlers;

    IMPLEMENT_REFCOUNTING(CefBaseJSRequestReplyHandler);
};



#endif /* CEFBASEREQUESTREPLIES_H_ */
