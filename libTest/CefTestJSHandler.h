/*
 * CefTestJSHandler.h
 *
 *  Created on: 28 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFTESTJSHANDLER_H_
#define CEFTESTJSHANDLER_H_

#include "CefBaseJSHandler.h"

class CefTestJSHandler: public CefBaseJSHandler {
public:
    CefTestJSHandler(const std::string& queryFn, const std::string& cancelFn)
        : CefBaseJSHandler(queryFn, cancelFn) { }

    /*
     * Register custom JS functions
     *
     * @param context   The JS context to add the functions to
     */
    virtual void RegisterJSFunctions(CefRefPtr<CefV8Context> context);

    virtual ~CefTestJSHandler();
};

#endif /* CEFTESTJSHANDLER_H_ */
