//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML SAX parser.
 */

#pragma once

#include <pycpp/misc/heap_pimpl.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/string.h>
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
    xml_sax_handler(const xml_sax_handler&) = default;
    xml_sax_handler& operator=(const xml_sax_handler&) = default;
    xml_sax_handler(xml_sax_handler&&) = default;
    xml_sax_handler& operator=(xml_sax_handler&&) = default;

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
    xml_stream_reader() = default;
    xml_stream_reader(const xml_stream_reader&) = delete;
    xml_stream_reader& operator=(const xml_stream_reader&) = delete;
    xml_stream_reader(xml_stream_reader&&);
    xml_stream_reader& operator=(xml_stream_reader&&);

    // MODIFIERS
    void open(istream&);
    void set_handler(xml_sax_handler&);
    void swap(xml_stream_reader&);

protected:
    istream* stream_ = nullptr;
    xml_sax_handler* handler_ = nullptr;
};



/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct xml_file_reader_base
{
    unique_heap_pimpl<ifstream> file_;
};


/**
 *  \brief Reader for stream-based document.
 */
struct xml_file_reader:
    protected xml_file_reader_base,
    public xml_stream_reader
{
    xml_file_reader() = default;
    xml_file_reader(const xml_file_reader&) = delete;
    xml_file_reader& operator=(const xml_file_reader&) = delete;
    xml_file_reader(xml_file_reader&&);
    xml_file_reader& operator=(xml_file_reader&&);

    // STREAM
    xml_file_reader(const string_view& name);
    void open(const string_view& name);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    xml_file_reader(const wstring_view& name);
    void open(const wstring_view& name);
    xml_file_reader(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS

    // MODIFIERS
    void swap(xml_file_reader&);
};


/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct xml_string_reader_base
{
    unique_heap_pimpl<istringstream> sstream_;
};


/**
 *  \brief Reader for string-based document.
 */
struct xml_string_reader:
    protected xml_string_reader_base,
    public xml_stream_reader
{
    xml_string_reader() = default;
    xml_string_reader(const xml_string_reader&) = delete;
    xml_string_reader& operator=(const xml_string_reader&) = delete;
    xml_string_reader(xml_string_reader&&);
    xml_string_reader& operator=(xml_string_reader&&);

    // STREAM
    xml_string_reader(const string_wrapper& str);
    void open(const string_wrapper& str);

    // MODIFIERS
    void swap(xml_string_reader&);
};

PYCPP_END_NAMESPACE
