//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML DOM parser.
 */

#pragma once

#include <pycpp/config.h>
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
    virtual void start_element(const string_view& content, xml_attr_t &attrs) override;
    virtual void end_element(const string_view& content) override;
    virtual void characters(const string_view& content) override;

private:
    xml_node_t* root_ = nullptr;
    std::deque<xml_node_t*> levels_;
};


/**
 *  \brief XML document type.
 */
struct xml_document_t: xml_node_t
{
    void loads(const std::string&);
    void load(std::istream&);
    void load(const std::string&);
#if defined(PYCPP_HAVE_WFOPEN)
    void load(const std::wstring&);
#endif

    std::string dumps(char = ' ', int = 4);
    void dump(std::ostream&, char = ' ', int = 4);
    void dump(const std::string&, char = ' ', int = 4);
#if defined(PYCPP_HAVE_WFOPEN)
    void dump(const std::wstring&, char = ' ', int = 4);
#endif
};

PYCPP_END_NAMESPACE
