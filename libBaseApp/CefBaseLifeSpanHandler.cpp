/*
 * CefBaseLifeSpanHandler.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseLifeSpanHandler.h"

CefBaseLifeSpanHandler::CefBaseLifeSpanHandler() {
    // TODO Auto-generated constructor stub

}

CefBaseLifeSpanHandler::~CefBaseLifeSpanHandler() {
    // TODO Auto-generated destructor stub
}

bool CefBaseLifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser) {
    bool retVal = false;

    for (PointerType& handler: handlers) {
        retVal |= handler->DoClose(browser);
    }

    return retVal;
}
