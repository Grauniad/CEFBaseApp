/*
 * CEFJSLogging.h
 *
 *  Created on: 15 Feb 2015
 *      Author: lhumphreys
 */

#ifndef CEFJSLOGGING_H_
#define CEFJSLOGGING_H_

#include <include/cef_v8.h>


namespace CEFJSLogging {
    /*
     * Add a log formatter function to the global
     * namespace
     *
     * @param  context the JS context to add the formatter to
     *
     * @returns true if the function as added, false otherwise
     */
    bool AddFormater(CefRefPtr<CefV8Context> context);
}




#endif /* CEFJSLOGGING_H_ */
