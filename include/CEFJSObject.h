/*
 * CEFJSObject.h
 *
 *  Created on: 28 Feb 2015
 *      Author: lhumphreys
 */

#ifndef CEFJSOBJECT_H_
#define CEFJSOBJECT_H_

#include <include/cef_v8.h>
#include <cstdint>

/**
 * Represents a java script object.
 */
class CEFJSObject {
public:
    CEFJSObject();

    virtual ~CEFJSObject();

    bool Add(const std::string& name, const std::string& value);

    bool Add(const std::string& name, const int32_t& value);

    /**
     * Return the V8 representation of the obeject
     */
    CefRefPtr<CefV8Value> GetObject() { return object; };

    /**
     * Return the JSON representation of the object
     */
    std::string JSON();
private:
    CefRefPtr<CefV8Value> object;
};

#endif /* CEFJSOBJECT_H_ */
