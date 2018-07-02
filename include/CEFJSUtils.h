/*
 * CEFJSUtils.h
 *
 *  Created on: 15 Feb 2015
 *      Author: lhumphreys
 */

#ifndef CEFJSUTILS_H_
#define CEFJSUTILS_H_

#include <include/cef_v8.h>
#include <memory>

namespace CEFJSUtils {
    /*
     * Set the value of the global variable <name> to <value> in the java-script
     * <context>.
     *
     * If the value does not exist it is created, if it does exist it is overriden
     *
     * @returns true if the variable was set, false otherwise
     *
     * @param name     The name of the variable to create
     * @param value    The new value of the variable
     * @param context  The V8 context to create the variable in...
     */
    bool SetGlobalVariable(const std::string& name, const std::string& value,
            CefRefPtr<CefV8Context> context);

    bool SetGlobalVariable(const std::string& name, const bool& value,
            CefRefPtr<CefV8Context> context);

    /*
     * JSFunction simplifies the process of adding a function to the global
     * context.
     *
     * To create a new function derive from this class and implement the
     * virtual Execute method, and provide a name for your function to the
     * c'tor.
     *
     * You will then need to install the function (usually in
     * onContextCreated) by calling Install and providing the context to install
     * it into.
     *
     */
    class JSFunction {
    public:
        JSFunction(const std::string& fname);

        virtual ~JSFunction() { }

        class JSFunctionContext {
        public:
            /*
             * Construct a new context...
             */
            JSFunctionContext(CefRefPtr<CefV8Value>& retVal,
                    const CefV8ValueList& arguments);
            /*
             * Can be called by from the child-class during an Execute()
             * invocation to get the numbered arg.
             *
             * Calling this function at any other time would have undefined
             * results
             *
             * @param idx  The argument number (starting at 0) to retrieve
             * @param val  On success, will be populated with the argument
             *
             * @returns true if the value was found, false otherwise
             */
            bool GetArg(size_t idx, std::string& val);

            /*
             * May be called from thec child-class during an Execute()
             * invocation to set the return value to the provided string.
             *
             * Calling this function at any other time would have undefined
             * results
             *
             * @param retValue  The return value
             *
             * @returns true on success, false otherwise
             */
            bool SetRetVal(const std::string& retValue);

            /*
             * Throw an error.
             *
             * The error will be caught outside of the scope of Execute,
             * and then raised as an exception in the JS context
             */
            void RaiseError(const std::string& msg);

            struct JSException {
                std::string msg;
            };

        private:
            CefRefPtr<CefV8Value>& retVal;
            const CefV8ValueList& args;
        };

        /*
         * To be implemented by the concrete class to do whatever logic they
         * desire
         */
        virtual void Execute(JSFunctionContext& context) = 0;

        /**
         * Installs the function into a context
         *
         * @param context   The context to install the function
         *                  into
         * @param function  The function to install
         *
         * @returns true if the function was installed
         *          Successfully, false otherwise
         */
        static bool Install(CefRefPtr<CefV8Context> context,
                std::unique_ptr<JSFunction>&& function);

        const std::string& Name() const {
            return name;
        }
    private:
        class JSFunctionHandler: public CefV8Handler {
        public:
            /*
             * Construct the handler from a new JSFunction, which we
             * take ownership of
             */
            JSFunctionHandler(std::unique_ptr<JSFunction>&& f);

            virtual ~JSFunctionHandler();

            virtual bool Execute(const CefString& name,
                    CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval, CefString& exception)
                            OVERRIDE;
        private:
            std::unique_ptr<JSFunction> func;

            // Provide the reference counting implementation for this class.
        IMPLEMENT_REFCOUNTING(JSFunctionHandler)
            ;
        };
        std::string name;
    };
}
;

#endif /* CEFJSUTILS_H_ */
