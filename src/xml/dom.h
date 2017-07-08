//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML DOM parser.
 */

#pragma once

#include <xml/sax.h>
#include <xml/writer.h>

// OBJECTS
// -------


/**
 *  \brief Handler for the XML DOM.
 */
struct xml_dom_handler: xml_sax_handler
{
public:
// TODO: need to implement...
//    xml_dom_handler(xml_node_t&);
//    virtual void start_document() override;
//    virtual void end_document() override;

private:
//    xml_node_t* root_ = nullptr;
//    bool has_key_ = false;
//    xml_string_t key_;
//    std::vector<xml_node_t*> levels_;
};


/**
 *  \brief XML document type.
 */
struct xml_document_t: xml_node_t
{
    void loads(const std::string&);
    void load(std::istream&);
    void load(const std::string&);
#if defined(HAVE_WFOPEN)
    void load(const std::wstring&);
#endif

    std::string dumps(char = ' ', int = 4);
    void dump(std::ostream&, char = ' ', int = 4);
    void dump(const std::string&, char = ' ', int = 4);
#if defined(HAVE_WFOPEN)
    void dump(const std::wstring&, char = ' ', int = 4);
#endif
};
