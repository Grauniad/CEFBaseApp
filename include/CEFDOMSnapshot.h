/*
 * CEFDOMSnapshot.h
 *
 *  Created on: 25 Feb 2018
 *      Author: lhumphreys
 */

#ifndef DEV_TOOLS_CPP_LIBRARIES_LIBCEF_LIBUTILS_CEFDOMSNAPSHOT_H_
#define DEV_TOOLS_CPP_LIBRARIES_LIBCEF_LIBUTILS_CEFDOMSNAPSHOT_H_

#include <include/cef_dom.h>
#include <include/cef_frame.h>

class CEFDOMSnapshot  {
public:
    CEFDOMSnapshot(std::string source);

    /**
     * Snapshot the DOM in the specified frame...
     *
     * *MUST* be invoked from the RENDERED thread
     */
    static std::unique_ptr<CEFDOMSnapshot> GetSnapshot(
            CefRefPtr<CefFrame> frame);

    virtual ~CEFDOMSnapshot() = default;

    const std::string& GetDOMSource() const { return source_; }
    std::string& GetDOMSource() { return source_; }
private:

    std::string source_;

};

#endif /* DEV_TOOLS_CPP_LIBRARIES_LIBCEF_LIBUTILS_CEFDOMSNAPSHOT_H_ */
