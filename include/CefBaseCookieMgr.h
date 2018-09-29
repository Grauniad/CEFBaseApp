#ifndef CEFBASEAPP_CEFBASECOOKIEMGR_H
#define CEFBASEAPP_CEFBASECOOKIEMGR_H

#include <include/cef_cookie.h>
#include <functional>
#include <map>

class CefBaseCookies {
public:
    CefBaseCookies(CefRefPtr<CefCookieManager> mgr);

    enum class RemainingCookies: int8_t {
        NO_MORE_COOKIES,
        MORE_COOKIES
    };
    using CookieForEach =
            std::function<void (const std::string& name, const std::string& value, const RemainingCookies& status)>;

    // Callback is triggered on TID_UI...
    void ForEachCookie(const std::string& url, CookieForEach callback);

    // May only be invoked from the Browser process...
    using CookieNameValueMap = std::map<std::string, std::string>;
    using CookieMapCallback = std::function<void (std::shared_ptr<const CookieNameValueMap>)>;
    void  GetCookieMap(const std::string& url, CookieMapCallback);

private:
    CefRefPtr<CefCookieManager> mgr;

};

#endif //CEFBASEAPP_CEFBASECOOKIEMGR_H
