//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML DOM parser.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/view/string.h>
#include <pycpp/xml/sax.h>
#include <pycpp/xml/writer.h>
#include <deque>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Handler for the XML DOM.
 */
struct xml_dom_handler: xml_sax_handler
{
public:
    xml_dom_handler(xml_node_t&);
    virtual void start_document() override;
    virtual void end_document() override;
    virtual void start_element(const string_view&, xml_attr_t&&) override;
    virtual void end_element(const string_view&) override;
    virtual void characters(const string_view&) override;

private:
    xml_node_t* root_ = nullptr;
    std::deque<xml_node_t*> levels_;
};


/**
 *  \brief XML document type.
 */
struct xml_document_t: xml_node_t
{
    void loads(const string_view&);
    void load(std::istream&);
    void load(const string_view&);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void load(const wstring_view&);
    void load(const u16string_view&);
#endif                                          // WINDOWS

    std::string dumps(char = ' ', int = 4);
    void dump(std::ostream&, char = ' ', int = 4);
    void dump(const string_view&, char = ' ', int = 4);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void dump(const wstring_view&, char = ' ', int = 4);
    void dump(const u16string_view&, char = ' ', int = 4);
#endif                                          // WINDOWS
};

PYCPP_END_NAMESPACE
