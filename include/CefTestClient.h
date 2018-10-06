#ifndef CEFTESTAPP_CLIENT_H_
#define CEFTESTAPP_CLIENT_H_

#include "include/cef_client.h"

class DummyCefClient: public CefClient,
                      public CefLifeSpanHandler,
                      public CefRenderHandler
{
public:
    DummyCefClient();

    /************************************************************************
     *                          LIFE SPAN HANDLING
     ***********************************************************************/

	/**
	 * The browser has been created
	 */
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> _browser) OVERRIDE;

	/**
	 * A browser is about to be destroyed - stop tracking it.
	 *
	 * In addition, we should be cleaning up any references we still have to it.
	 */
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    /************************************************************************
     *                          Message handling
     ***********************************************************************/

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                            CefProcessId source_process,
                                            CefRefPtr<CefProcessMessage> message);

    /************************************************************************
     *                          Windowless Rendering
     ***********************************************************************/

    ///
    // Called to retrieve the view rectangle which is relative to screen
    // coordinates. Return true if the rectangle was provided.
    ///
    /*--cef()--*/
    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);

    ///
    // Called when an element should be painted. |type| indicates whether the
    // element is the view or the popup widget. |buffer| contains the pixel data
    // for the whole image. |dirtyRects| contains the set of rectangles that need
    // to be repainted. |buffer| will be |width|*|height|*4 bytes in size and
    // represents a BGRA image with an upper-left origin.
    ///
    /*--cef()--*/
    virtual void OnPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height) { }

private:
	CefRefPtr<CefBrowser> browser;
    IMPLEMENT_REFCOUNTING(DummyCefClient);
};

#endif
