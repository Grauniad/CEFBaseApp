/*
 * CEFDOMSnapshot.cpp
 *
 *  Created on: 25 Feb 2018
 *      Author: lhumphreys
 */

#include "CEFDOMSnapshot.h"
#include <include/wrapper/cef_helpers.h>

namespace
{
    class DOMVisitor: public CefDOMVisitor
    {
    public:
        DOMVisitor(std::string& dest): dest_(dest) {}

        void Visit(CefRefPtr<CefDOMDocument> document) override {
            dest_ = document->GetDocument()->GetAsMarkup().ToString();
        }

        std::string& dest_;
        IMPLEMENT_REFCOUNTING(DOMVisitor);
    };


}

std::unique_ptr<CEFDOMSnapshot> CEFDOMSnapshot::GetSnapshot(
        CefRefPtr<CefFrame> frame)
{
    // Visit DOM is only permitted from the RENDERER
    CEF_REQUIRE_RENDERER_THREAD();

    std::string source;

    CefRefPtr<DOMVisitor> visitor = new DOMVisitor(source);

    frame->VisitDOM(visitor);

    return std::make_unique<CEFDOMSnapshot>(std::move(source));
}

CEFDOMSnapshot::CEFDOMSnapshot(std::string source)
    : source_(source)
{
}
