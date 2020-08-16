#include <CefBaseCookieMgr.h>
#include <CefBaseThread.h>

namespace {
    class StoredCallback {
    public:
        StoredCallback(CefBaseCookies::CookieForEach cb)
           : state(Status::PENDING)
           , storedcb(std::move(cb)) {}

        void Callback(
                const std::string& name,
                const std::string& value,
                const CefBaseCookies::RemainingCookies& status)
        {
            switch (state)
            {
                case Status::PENDING:
                case Status::TRIGGERED:
                    storedcb(name, value, status);
                    state = Status::TRIGGERED;
                    break;
                case Status::TIMEDOUT:
                    break;
            }
        }

        void Timeout() {
            switch (state)
            {
                case Status::TRIGGERED:
                    // Not a problem - we got data
                    break;
                case Status::TIMEDOUT:
                case Status::PENDING:
                    state = Status::TIMEDOUT;
                    storedcb("", "", CefBaseCookies::RemainingCookies::NO_COOKIES);
                    break;
            }
        }
    private:
        enum class Status {
            PENDING,
            TRIGGERED,
            TIMEDOUT,
        };
        Status state;
        CefBaseCookies::CookieForEach storedcb;
    };
}

CefBaseCookies::CefBaseCookies(CefRefPtr<CefCookieManager> mgr)
   : mgr(mgr)
{
}

void CefBaseCookies::ForEachCookie(
        const std::string &url,
        CefBaseCookies::CookieForEach callback,
        int timeoutms)
{
    auto storedCb = std::make_shared<StoredCallback>(std::move(callback));
    class Visitor : public CefCookieVisitor {
    public:
        Visitor(std::shared_ptr<StoredCallback> cb) : cb(cb) {}

        std::shared_ptr<StoredCallback> cb;

        bool Visit(const CefCookie &cookie, int count, int total, bool &deleteCookie) override {
            CefString name(cookie.name.str);
            CefString value(cookie.value.str);

            const RemainingCookies status = (count < (total - 1)) ?
                                            RemainingCookies::MORE_COOKIES :
                                            RemainingCookies::NO_MORE_COOKIES;
            cb->Callback(name, value, status);

            return true;
        }

    IMPLEMENT_REFCOUNTING(Visitor);
    };
    CefRefPtr<Visitor> v = new Visitor(storedCb);

    CefBaseThread::PostToCEFThread(TID_UI, [=] () -> void {
        storedCb->Timeout();
    }, timeoutms);

    if (!mgr->VisitUrlCookies(url, true, v)) {
        storedCb->Callback("", "", RemainingCookies::NO_COOKIES);
    }
}

void CefBaseCookies::GetCookieMap(
        const std::string &url,
        CefBaseCookies::CookieMapCallback cb,
        int timeoutms)
{
    std::shared_ptr<CookieNameValueMap> nv = std::make_shared<CookieNameValueMap>();
    auto consumer = [nv=std::move(nv), cb=std::move(cb)] (
            std::string name,
            std::string value,
            const RemainingCookies status) -> void
    {
        switch (status) {
        case RemainingCookies::NO_MORE_COOKIES:
            nv->emplace(std::move(name), std::move(value));
            // fall thru
        case RemainingCookies::NO_COOKIES:
            cb(nv);
            break;
        case RemainingCookies::MORE_COOKIES:
            nv->emplace(std::move(name), std::move(value));
            // more to come...
            break;
        }
    };
    ForEachCookie(url, std::move(consumer), timeoutms);
}

