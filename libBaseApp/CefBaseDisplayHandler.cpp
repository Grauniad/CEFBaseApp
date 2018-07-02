/*
 * CefBaseDisplayHandler.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseDisplayHandler.h"

CefBaseDisplayHandler::CefBaseDisplayHandler() {
    // TODO Auto-generated constructor stub

}

CefBaseDisplayHandler::~CefBaseDisplayHandler() {
    // TODO Auto-generated destructor stub
}

void CefBaseDisplayHandler::OnTitleChange(
    CefRefPtr<CefBrowser> browser,
    const CefString& title)
{
    for (PointerType& handler: handlers) {
        handler->OnTitleChange(browser,title);
    }
}
