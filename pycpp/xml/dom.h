//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML DOM parser.
 */

#pragma once

#include <pycpp/stl/deque.h>
#include <pycpp/string/string.h>
#include <pycpp/xml/sax.h>
#include <pycpp/xml/writer.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Handler for the XML DOM.
 */
struct xml_dom_handler: xml_sax_handler
{
public:
    xml_dom_handler() = delete;
    xml_dom_handler(const xml_dom_handler&) = delete;
    xml_dom_handler& operator=(const xml_dom_handler&) = delete;
    xml_dom_handler(xml_dom_handler&&);
    xml_dom_handler& operator=(xml_dom_handler&&);
    xml_dom_handler(xml_node_t&);

    // SAX EVENTS
    virtual void start_document() override;
    virtual void end_document() override;
    virtual void start_element(const string_wrapper&, xml_attr_t&&) override;
    virtual void end_element(const string_wrapper&) override;
    virtual void characters(const string_wrapper&) override;

        // MODIFIERS
    void swap(xml_dom_handler&);

private:
    xml_node_t* root_ = nullptr;
    deque<xml_node_t*> levels_;
};


/**
 *  \brief XML document type.
 */
struct xml_document_t: xml_node_t
{
    xml_document_t() = default;
    xml_document_t(const xml_document_t&) = delete;
    xml_document_t& operator=(const xml_document_t&) = delete;
    xml_document_t(xml_document_t&&);
    xml_document_t& operator=(xml_document_t&&);

    // READERS
    void loads(const string_wrapper&);
    void load(istream&);
    void load(const string_view&);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void load(const wstring_view&);
    void load(const u16string_view&);
#endif                                          // WINDOWS

    // WRITERS
    xml_string_t dumps(char = ' ', int = 4);
    void dump(ostream&, char = ' ', int = 4);
    void dump(const string_view&, char = ' ', int = 4);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    void dump(const wstring_view&, char = ' ', int = 4);
    void dump(const u16string_view&, char = ' ', int = 4);
#endif                                          // WINDOWS
};

PYCPP_END_NAMESPACE
