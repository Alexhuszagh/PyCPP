//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON SAX parser.
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
 *  \brief SAX handler for a JSON document.
 */
struct json_sax_handler
{
    json_sax_handler() = default;
    json_sax_handler(const json_sax_handler&) = default;
    json_sax_handler& operator=(const json_sax_handler&) = default;
    json_sax_handler(json_sax_handler&&) = default;
    json_sax_handler& operator=(json_sax_handler&&) = default;

    // SAX EVENTS
    virtual void start_document();
    virtual void end_document();
    virtual void start_object();
    virtual void end_object(size_t);
    virtual void start_array();
    virtual void end_array(size_t);
    virtual void key(const string_wrapper&);
    virtual void null();
    virtual void boolean(bool);
    virtual void number(double);
    virtual void string(const string_wrapper&);
};


/**
 *  \brief Reader for stream-based document.
 */
struct json_stream_reader
{
public:
    json_stream_reader() noexcept = default;
    json_stream_reader(const json_stream_reader&) = delete;
    json_stream_reader& operator=(const json_stream_reader&) = delete;
    json_stream_reader(json_stream_reader&&) noexcept;
    json_stream_reader& operator=(json_stream_reader&&) noexcept;

    // MODIFIERS
    void open(istream&);
    void set_handler(json_sax_handler&) noexcept;
    void swap(json_stream_reader&) noexcept;

private:
    istream* stream_ = nullptr;
    json_sax_handler* handler_ = nullptr;
};


/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct json_file_reader_base
{
    json_file_reader_base();
    unique_heap_pimpl<ifstream> file_;
};


/**
 *  \brief Reader for file-based document.
 */
struct json_file_reader:
    protected json_file_reader_base,
    public json_stream_reader
{
    json_file_reader() = default;
    json_file_reader(const json_file_reader&) = delete;
    json_file_reader& operator=(const json_file_reader&) = delete;
    json_file_reader(json_file_reader&&) noexcept;
    json_file_reader& operator=(json_file_reader&&) noexcept;

    // STREAM
    json_file_reader(const string_view& name);
    void open(const string_view& name);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    json_file_reader(const wstring_view& name);
    void open(const wstring_view& name);
    json_file_reader(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS

    // MODIFIERS
    void swap(json_file_reader&) noexcept;
};


/**
 *  \brief Base to ensure file_ cleanup occurs after stream cleanup.
 */
struct json_string_reader_base
{
    json_string_reader_base();
    unique_heap_pimpl<json_istringstream_t> sstream_;
};


/**
 *  \brief Reader for string-based document.
 */
struct json_string_reader:
    protected json_string_reader_base,
    public json_stream_reader
{
    json_string_reader() = default;
    json_string_reader(const json_string_reader&) = delete;
    json_string_reader& operator=(const json_string_reader&) = delete;
    json_string_reader(json_string_reader&&) noexcept;
    json_string_reader& operator=(json_string_reader&&) noexcept;

    // STREAM
    json_string_reader(const string_wrapper& str);
    void open(const string_wrapper& str);

    // MODIFIERS
    void swap(json_string_reader&) noexcept;
};

PYCPP_END_NAMESPACE
