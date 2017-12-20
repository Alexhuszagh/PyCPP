//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON generic writer.
 */

#pragma once

#include <pycpp/json/core.h>
#include <pycpp/misc/heap_pimpl.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Generic writer for a JSON document.
 */
struct json_writer
{
    // SAX EVENTS
    virtual void start_object();
    virtual void end_object();
    virtual void start_array();
    virtual void end_array();
    virtual void key(const string_wrapper&);
    virtual void null();
    virtual void boolean(bool);
    virtual void number(double);
    virtual void string(const string_wrapper&);
    virtual void flush() const;
};


/**
 *  \brief Writer for stream-based document.
 */
struct json_stream_writer: json_writer
{
public:
    json_stream_writer(char = ' ', int = 4);
    json_stream_writer(ostream&, char = ' ', int = 4);
    json_stream_writer(const json_stream_writer&) = delete;
    json_stream_writer& operator=(const json_stream_writer&) = delete;
    json_stream_writer(json_stream_writer&&);
    json_stream_writer& operator=(json_stream_writer&&);
    ~json_stream_writer();

    // MODIFIERS/PROPERTIES
    void open(ostream&);
    void set_indent(char = ' ', int = 4);
    bool is_pretty() const;
    void swap(json_stream_writer&);

    // SAX EVENTS
    virtual void start_object() override;
    virtual void end_object() override;
    virtual void start_array() override;
    virtual void end_array() override;
    virtual void key(const string_wrapper&) override;
    virtual void null() override;
    virtual void boolean(bool) override;
    virtual void number(double) override;
    virtual void string(const string_wrapper&) override;
    virtual void flush() const override;

private:
    char indent_character_ = ' ';
    int indent_width_ = 0;
    void* stream_ = nullptr;
    void* writer_ = nullptr;
};


/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct json_file_writer_base
{
    mutable unique_heap_pimpl<ofstream> file_;
};


/**
 *  \brief Writer for file-based document.
 */
struct json_file_writer:
    protected json_file_writer_base,
    public json_stream_writer
{
    json_file_writer() = default;
    json_file_writer(const json_file_writer&) = delete;
    json_file_writer& operator=(const json_file_writer&) = delete;
    json_file_writer(json_file_writer&&);
    json_file_writer& operator=(json_file_writer&&);

    // STREAM
    json_file_writer(const string_view& name);
    void open(const string_view& name);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    json_file_writer(const wstring_view& name);
    void open(const wstring_view& name);
    json_file_writer(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS
    virtual void flush() const override;

    // MODIFIERS
    void swap(json_file_writer&);
};


/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct json_string_writer_base
{
    mutable unique_heap_pimpl<ostringstream> sstream_;
};


/**
 *  \brief Writer for string-based document.
 */
struct json_string_writer:
    protected json_string_writer_base,
    public json_stream_writer
{
    json_string_writer();
    json_string_writer(const json_string_writer&) = delete;
    json_string_writer& operator=(const json_string_writer&) = delete;
    json_string_writer(json_string_writer&&);
    json_string_writer& operator=(json_string_writer&&);
    std::string str() const;

    // MODIFIERS
    void swap(json_string_writer&);
};

PYCPP_END_NAMESPACE
