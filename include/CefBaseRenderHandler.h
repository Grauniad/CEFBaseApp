/*
 * CefBaseRenderHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASERENDERHANDLER_H_
#define CEFBASERENDERHANDLER_H_

#include "CefBaseMultiHandler.h"
#include "include/cef_render_handler.h"

class CefBaseRenderHandler: public CefBaseMultiHandler<CefRenderHandler> {
public:
    CefBaseRenderHandler();

    virtual ~CefBaseRenderHandler();

    /************************************************************************
     *                          Windowless Rendering
     ***********************************************************************/

    /**
     * Process: Browser
     *
     * Thread: UI
     *
     * Event: Request for view rectange of the browser
     *
     * Called to retrieve the view rectangle which is relative to screen
     * coordinates. Return true if the rectangle was provided.
     */
    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);

    /*--cef()--*/
    /**
     * Process: Browser
     *
     * Thread: UI
     *
     * Event: An element is to be painted to the screen
     *
     * Called when an element should be painted. |type| indicates whether the
     * element is the view or the popup widget. |buffer| contains the pixel data
     * for the whole image. |dirtyRects| contains the set of rectangles that
     * need to be repainted. |buffer| will be |width|*|height|*4 bytes in size
     * and represents a BGRA image with an upper-left origin.
     */
    virtual void OnPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height);
private:
    IMPLEMENT_REFCOUNTING(CefBaseRenderHandler);
};

#endif /* CEFBASERENDERHANDLER_H_ */
