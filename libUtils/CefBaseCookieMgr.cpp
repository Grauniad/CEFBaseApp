#include <CefBaseCookieMgr.h>

CefBaseCookies::CefBaseCookies(CefRefPtr<CefCookieManager> mgr)
   : mgr(mgr)
{
}

void CefBaseCookies::ForEachCookie(const std::string &url, CefBaseCookies::CookieForEach callback) {
    class Visitor: public CefCookieVisitor {
    public:
        Visitor(CookieForEach cb): cb(std::move(cb)) {}
        CookieForEach cb;

        bool Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie) override {
            CefString name(cookie.name.str);
            CefString value(cookie.value.str);
            const RemainingCookies status = (count < (total-1))?
                                 RemainingCookies::MORE_COOKIES:
                                 RemainingCookies::NO_MORE_COOKIES;
            cb(name, value, status);
        }

        IMPLEMENT_REFCOUNTING(Visitor);
    };
    CefRefPtr<Visitor> v = new Visitor(callback);

    mgr->VisitUrlCookies(url,false, v);
}

void CefBaseCookies::GetCookieMap(const std::string &url, CefBaseCookies::CookieMapCallback cb) {
    std::shared_ptr<CookieNameValueMap> nv = std::make_shared<CookieNameValueMap>();
    auto f = [nv=std::move(nv), cb=std::move(cb)] (
            std::string name,
            std::string value,
            const RemainingCookies status) -> void
    {
        nv->emplace(std::move(name), std::move(value));
        switch (status) {
        case RemainingCookies::NO_MORE_COOKIES:
            cb(nv);
            break;
        case RemainingCookies::MORE_COOKIES:
            // more to come...
            break;
        }
    };
}

