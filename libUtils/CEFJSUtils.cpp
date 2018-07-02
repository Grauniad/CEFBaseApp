/*
 * CEFJSUtils.cpp
 *
 *  Created on: 15 Feb 2015
 *      Author: lhumphreys
 */

#include "CEFJSUtils.h"

bool CEFJSUtils::SetGlobalVariable(
    const std::string& name,
    const std::string& value,
    CefRefPtr<CefV8Context> context)
{
    bool ok = false;

    CefRefPtr<CefV8Value> object = context->GetGlobal();

    CefRefPtr<CefV8Value> str = CefV8Value::CreateString(value);

    ok = object->SetValue(name, str, V8_PROPERTY_ATTRIBUTE_NONE);

    return ok;
}

bool CEFJSUtils::SetGlobalVariable(
    const std::string& name,
    const bool& value,
    CefRefPtr<CefV8Context> context)
{
    bool ok = false;

    CefRefPtr<CefV8Value> object = context->GetGlobal();

    CefRefPtr<CefV8Value> js_value = CefV8Value::CreateBool(value);

    ok = object->SetValue(name, js_value, V8_PROPERTY_ATTRIBUTE_NONE);

    return ok;
}

CEFJSUtils::JSFunction::JSFunction(const std::string& fname)
        : name(fname)
{
}

bool CEFJSUtils::JSFunction::Install(
    CefRefPtr<CefV8Context> context,
    std::unique_ptr<JSFunction>&& function)
{
    bool installed = false;

    const std::string& fname = function->Name();

    CefRefPtr<CefV8Handler> handler = new JSFunctionHandler(
        std::move(function));

    CefRefPtr<CefV8Value> jsFunction = CefV8Value::CreateFunction(
        fname,
        handler);

    CefRefPtr<CefV8Value> object = context->GetGlobal();

    installed = object->SetValue(fname, jsFunction, V8_PROPERTY_ATTRIBUTE_NONE);

    return installed;
}

CEFJSUtils::JSFunction::JSFunctionHandler::JSFunctionHandler(
    std::unique_ptr<JSFunction>&& f)
        : func(std::move(f))
{
}

CEFJSUtils::JSFunction::JSFunctionHandler::~JSFunctionHandler() {
}

bool CEFJSUtils::JSFunction::JSFunctionHandler::Execute(
    const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception)
{
    bool executionHandled = false;

    if (name == func->Name()) {
        JSFunctionContext context(retval, arguments);

        try {
            func->Execute(context);
        } catch (JSFunctionContext::JSException& err) {
            exception = err.msg;
        }

        executionHandled = true;
    }

    return executionHandled;
}

CEFJSUtils::JSFunction::JSFunctionContext::JSFunctionContext(
    CefRefPtr<CefV8Value>& _retVal,
    const CefV8ValueList& _arguments)
        : retVal(_retVal), args(_arguments)
{
}

bool CEFJSUtils::JSFunction::JSFunctionContext::GetArg(
    size_t idx,
    std::string& val)
{
    bool found = false;

    if (args.size() > idx) {
        CefRefPtr<CefV8Value> value = args[idx];
        if (value->IsString()) {
            val = value->GetStringValue();
            found = true;
        }
    }

    return found;
}

bool CEFJSUtils::JSFunction::JSFunctionContext::SetRetVal(
    const std::string& retValueToSet)
{
    bool setValue = true;

    retVal = CefV8Value::CreateString(retValueToSet);

    return setValue;
}

void CEFJSUtils::JSFunction::JSFunctionContext::RaiseError(
    const std::string& msg)
{
    throw JSException{msg};
}
