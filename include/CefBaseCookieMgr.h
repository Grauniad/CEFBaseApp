#ifndef CEFBASEAPP_CEFBASECOOKIEMGR_H
#define CEFBASEAPP_CEFBASECOOKIEMGR_H

#include <include/cef_cookie.h>
#include <functional>
#include <map>
#include <memory>

class CefBaseCookies {
public:
    CefBaseCookies(CefRefPtr<CefCookieManager> mgr);

    static constexpr const char* DUMMY_COOKIE_NAME = "CefBaseCookies::DUMMY_COOKIE_NAME";

    enum class RemainingCookies: int8_t {
        NO_COOKIES,
        NO_MORE_COOKIES,
        MORE_COOKIES
    };
    using CookieForEach =
            std::function<void (const std::string& name, const std::string& value, const RemainingCookies& status)>;

    // Callback is triggered on TID_UI...
    /**
     * PROCESS:          PID_UI
     * THREAD:           ANY
     * CALLBACK THREAD:  TID_UI
     *
     * Trigger the specified callback method for each matching cookie.
     *
     * NOTE: If there are no matching cookie we never get a callback from the underlying CEF API. In this case the
     *       configured timeoutms will trigger an abort after the configured wait time.
     *
     * @param url       The URL to get cookies for
     * @param callback  The callback method triggered for each cookie
     * @param timeoutms How long to wait before giving up, and aborting the request if no cookies have yet been found.
     */
    void ForEachCookie(
            const std::string& url,
            CookieForEach callback,
            const int timeoutms = 50);

    /**
     * PROCESS:          PID_UI
     * THREAD:           ANY
     * CALLBACK THREAD:  TID_UI
     *
     * Wraps the ForEachCookie method to return a map continaing all found cookies.
     *
     * NOTE: If there are no matching cookie we never get a callback from the underlying CEF API. In this case the
     *       configured timeoutms will trigger an abort after the configured wait time.
     *
     * @param url       The URL to get cookies for
     * @param callback  The callback method triggered once all cookies have been returned.
     * @param timeoutms How long to wait before giving up, and aborting the request if no cookies have yet been found.
     */
    using CookieNameValueMap = std::map<std::string, std::string>;
    using CookieMapCallback = std::function<void (std::shared_ptr<const CookieNameValueMap>)>;
    void  GetCookieMap(const std::string& url, CookieMapCallback, int timeoutms = 50);

private:
    CefRefPtr<CefCookieManager> mgr;

};

#endif //CEFBASEAPP_CEFBASECOOKIEMGR_H
