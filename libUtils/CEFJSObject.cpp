/*
 * CEFJSObject.cpp
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */

#include "CEFJSObject.h"

//TODO: Investigate performance of default getters / setters to see if we want
//      to provide a custom one
CEFJSObject::CEFJSObject()
    : object(CefV8Value::CreateObject(nullptr, nullptr))
{
}

CEFJSObject::~CEFJSObject() {
}

bool CEFJSObject::Add(const std::string& name, const std::string& value) {
    CefRefPtr<CefV8Value> str = CefV8Value::CreateString(value);

    return object->SetValue(name, str, V8_PROPERTY_ATTRIBUTE_NONE);
}

bool CEFJSObject::Add(const std::string& name, const int32_t& value) {
    CefRefPtr<CefV8Value> str = CefV8Value::CreateInt(value);

    return object->SetValue(name, str, V8_PROPERTY_ATTRIBUTE_NONE);
}

std::string CEFJSObject::JSON() {
    static std::stringstream result;
    result.clear();

    result << "{" << std::endl;

    std::vector<CefString> keys;
    object->GetKeys(keys);

    bool first = true;
    for (const CefString& key: keys) {
        if (first) {
            first = false;
        } else {
            result << "," << std::endl;
        }
        CefRefPtr<CefV8Value> value = object->GetValue(key);
        result << key.ToString() << ": " << value->GetStringValue().ToString();
    }

    result << std::endl << "}" << std::endl;
    return result.str();
}
