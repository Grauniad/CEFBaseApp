/*
 * CefBaseRenderHandler.cpp
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#include "CefBaseRenderHandler.h"

CefBaseRenderHandler::CefBaseRenderHandler() {
    // TODO Auto-generated constructor stub

}

CefBaseRenderHandler::~CefBaseRenderHandler() {
    // TODO Auto-generated destructor stub
}

bool CefBaseRenderHandler::GetViewRect(
    CefRefPtr<CefBrowser> browser,
    CefRect& rect)
{
    bool found = false;
    for (PointerType& handler: handlers) {
        found |= handler->GetViewRect(browser,rect);
    }
    return found;
}

void CefBaseRenderHandler::OnPaint(
    CefRefPtr<CefBrowser> browser,
    PaintElementType type,
    const RectList& dirtyRects,
    const void* buffer,
    int width,
    int height)
{
    for (PointerType& handler: handlers) {
        handler->OnPaint(browser,type,dirtyRects,buffer,width,height);
    }
}
