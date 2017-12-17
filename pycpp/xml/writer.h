//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML generic writer.
 */

#pragma once

#include <pycpp/stl/fstream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/string.h>
#include <pycpp/xml/core.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Generic writer for an XML document.
 */
struct xml_writer
{
    // SAX EVENTS
    virtual void start_element(const string_wrapper&);
    virtual void end_element();
    virtual void write_attribute(const string_wrapper&, const string_wrapper&);
    virtual void write_text(const string_wrapper&);
    virtual void write_comment(const string_wrapper&);
    virtual void start_element_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&);
    virtual void end_element_ns();
    virtual void write_attribute_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&, const string_wrapper&);
    virtual void flush() const;
};


/**
 *  \brief Writer for stream-based document.
 */
struct xml_stream_writer: xml_writer
{
public:
    xml_stream_writer(char = ' ', int = 4);
    xml_stream_writer(ostream&, char = ' ', int = 4);
    xml_stream_writer(const xml_stream_writer&) = delete;
    xml_stream_writer& operator=(const xml_stream_writer&) = delete;
    xml_stream_writer(xml_stream_writer&&);
    xml_stream_writer& operator=(xml_stream_writer&&);
    ~xml_stream_writer();

    // MODIFIERS/PROPERTIES
    void open(ostream&);
    void set_indent(char = ' ', int = 4);
    bool is_pretty() const;
    void swap(xml_stream_writer&);

    // SAX EVENTS
    virtual void start_element(const string_wrapper&) override;
    virtual void end_element() override;
    virtual void write_attribute(const string_wrapper&, const string_wrapper&) override;
    virtual void write_text(const string_wrapper&) override;
    virtual void write_comment(const string_wrapper&) override;
    virtual void start_element_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&) override;
    virtual void end_element_ns() override;
    virtual void write_attribute_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&, const string_wrapper&) override;
    virtual void flush() const override;

private:
    char indent_character_ = ' ';
    int indent_width_ = 0;
    void* stream_ = nullptr;
    void* writer_ = nullptr;
};


/**
 *  \brief Writer for stream-based document.
 */
struct xml_file_writer: xml_stream_writer
{
public:
    xml_file_writer() = default;
    xml_file_writer(const xml_file_writer&) = delete;
    xml_file_writer& operator=(const xml_file_writer&) = delete;
    xml_file_writer(xml_file_writer&&);
    xml_file_writer& operator=(xml_file_writer&&);

    // STREAM
    xml_file_writer(const string_view& name);
    void open(const string_view& name);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    xml_file_writer(const wstring_view& name);
    void open(const wstring_view& name);
    xml_file_writer(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS
    virtual void flush() const override;

    // MODIFIERS
    void swap(xml_string_writer&);

private:
    mutable ofstream file_;
};


/**
 *  \brief Writer for string-based document.
 */
struct xml_string_writer: xml_stream_writer
{
public:
    xml_string_writer();
    xml_string_writer(const xml_string_writer&) = delete;
    xml_string_writer& operator=(const xml_string_writer&) = delete;
    xml_string_writer(xml_string_writer&&);
    xml_string_writer& operator=(xml_string_writer&&);
    std::string str() const;

    // MODIFIERS
    void swap(xml_string_writer&);

private:
    ostringstream sstream_;
};

PYCPP_END_NAMESPACE
