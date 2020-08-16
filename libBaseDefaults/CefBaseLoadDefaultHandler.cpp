/*
 * GCGV_Callbacks_Loading.cpp
 *
 *  Created on: 8 Feb 2015
 *      Author: lhumphreys
 */


#include "CefBaseLoadDefaultHandler.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_parser.h"

void CefBaseLoadDefaultHandler::OnLoadError(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl)
{
    if (CefCurrentlyOn(TID_UI))
    {
        // Don't display an error for downloaded files.
        if (errorCode == ERR_ABORTED) {
            return;
        }

        // Display a load error message.
        std::stringstream ss;
        ss << "<html><body bgcolor=\"white\">"
                "<h2>Failed to load URL " << std::string(failedUrl) <<
                " with error " << std::string(errorText) << " (" << errorCode <<
                ").</h2></body></html>";
        CefString raw = ss.str();
        frame->LoadURL(CefURIEncode(raw, false));
    }
}
