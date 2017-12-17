//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML SAX parser.
 */

#pragma once

#include <pycpp/stl/sstream.h>
#include <pycpp/string/string.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/xml/core.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief SAX handler for an XML document.
 */
struct xml_sax_handler
{
public:
    xml_sax_handler(bool use_namespaces = false);

    // SAX EVENTS
    virtual void start_document();
    virtual void end_document();
    virtual void start_element(const string_wrapper&, xml_attr_t&&);
    virtual void end_element(const string_wrapper&);
    virtual void characters(const string_wrapper&);
    virtual void start_element_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&, xml_attr_t&&);
    virtual void end_element_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&);
    virtual void ignorable_whitespace(const string_wrapper&);
    virtual void processing_instruction(const string_wrapper&, const string_wrapper&);
    virtual void skipped_entity(const string_wrapper&);

    // PROPERTIES
    bool use_namespaces() const;

private:
    bool use_namespaces_ = false;
};


/**
 *  \brief Reader for stream-based document.
 */
struct xml_stream_reader
{
public:
    xml_stream_reader();

    void open(istream&);
    void set_handler(xml_sax_handler&);

private:
    istream* stream_ = nullptr;
    xml_sax_handler* handler_ = nullptr;
};


/**
 *  \brief Reader for stream-based document.
 */
struct xml_file_reader: xml_stream_reader
{
public:
    xml_file_reader();
    xml_file_reader(const string_view& name);
    void open(const string_view& name);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    xml_file_reader(const wstring_view& name);
    void open(const wstring_view& name);
    xml_file_reader(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS

private:
    ifstream file_;
};


/**
 *  \brief Reader for string-based document.
 */
struct xml_string_reader: xml_stream_reader
{
public:
    xml_string_reader();
    xml_string_reader(const string_wrapper& str);
    void open(const string_wrapper& str);

private:
    istringstream sstream_;
};

PYCPP_END_NAMESPACE
