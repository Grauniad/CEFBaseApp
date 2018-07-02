/*
 * CefBaseMultiHandler.h
 *
 *  Created on: 14 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEMULTIHANDLER_H_
#define CEFBASEMULTIHANDLER_H_


#include "include/wrapper/cef_helpers.h"

template <class HANDLER>
class CefBaseMultiHandler: public HANDLER {
public:
    typedef std::shared_ptr<HANDLER> PointerType;
    typedef HANDLER BaseType;

    void InstallHandler(PointerType handler) {
        handlers.emplace_back(handler);
    }

    template <class... ARGS>
    void ForwardToHandlers(void (HANDLER::*callback)(ARGS...), ARGS... args) {
        for (PointerType& handler: handlers) {
            ((handler.get())->*callback)(std::forward<ARGS>(args)...);
        }
    }


protected:
    typedef std::vector<PointerType> Container;
    Container handlers;

    IMPLEMENT_REFCOUNTING(CefBaseMultiHandler<HANDLER>);
};

#define CefBaseMultiHandler_Forward(fname) \
    virtual void fname() OVERRIDE { \
        ForwardToHandlers(&BaseType::fname); \
    }

#define CefBaseMultiHandler_Forward1(fname, TYPE1) \
    virtual void fname(TYPE1 arg1) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname,arg1); \
    }

#define CefBaseMultiHandler_Forward2(fname, TYPE1, TYPE2) \
    virtual void fname(TYPE1 arg1, TYPE2 arg2) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname, arg1, arg2); \
    }

#define CefBaseMultiHandler_Forward3(fname, TYPE1, TYPE2, TYPE3) \
    virtual void fname(TYPE1 arg1, TYPE2 arg2, TYPE3 arg3) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname, arg1, arg2, arg3); \
    }

#define CefBaseMultiHandler_Forward4(fname, TYPE1, TYPE2, TYPE3, TYPE4) \
    virtual void fname(TYPE1 arg1, TYPE2 arg2, TYPE2 arg3, TYPE4, arg4) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname, arg1, arg2, arg3, arg4); \
    }

#define CefBaseMultiHandler_Forward1Ref(fname, TYPE1) \
    virtual void fname(CefRefPtr<TYPE1> arg1) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname,arg1); \
    }

#define CefBaseMultiHandler_Forward2Refs(fname, TYPE1, TYPE2) \
    virtual void fname(CefRefPtr<TYPE1> arg1, CefRefPtr<TYPE2> arg2) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname, arg1, arg2); \
    }

#define CefBaseMultiHandler_Forward3Refs(fname, TYPE1, TYPE2, TYPE3) \
    virtual void fname(CefRefPtr<TYPE1> arg1, CefRefPtr<TYPE2> arg2, CefRefPtr<TYPE3> arg3) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname,arg1, arg2, arg3); \
    }

#define CefBaseMultiHandler_Forward4Refs(fname, TYPE1, TYPE2, TYPE3, TYPE4) \
    virtual void fname(CefRefPtr<TYPE1> arg1, CefRefPtr<TYPE2> arg2, CefRefPtr<TYPE3> arg3, CefRefPtr<TYPE4>, arg4) OVERRIDE { \
        ForwardToHandlers(&BaseType::fname, arg1, arg2, arg3, arg4); \
    }

#endif /* CEFBASEMULTIHANDLER_H_ */
