//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON SAX parser.
 */

#pragma once

#include <pycpp/json/core.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/view/string.h>

#include <sstream>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief SAX handler for a JSON document.
 */
struct json_sax_handler
{
    // SAX EVENTS
    virtual void start_document();
    virtual void end_document();
    virtual void start_object();
    virtual void end_object(size_t);
    virtual void start_array();
    virtual void end_array(size_t);
    virtual void key(const string_view&);
    virtual void null();
    virtual void boolean(bool);
    virtual void number(double);
    virtual void string(const string_view&);
};


/**
 *  \brief Reader for stream-based document.
 */
struct json_stream_reader
{
public:
    json_stream_reader();

    void open(std::istream&);
    void set_handler(json_sax_handler&);

private:
    std::istream* stream_ = nullptr;
    json_sax_handler* handler_ = nullptr;
};


/**
 *  \brief Reader for file-based document.
 */
struct json_file_reader: json_stream_reader
{
public:
    json_file_reader();
    json_file_reader(const std::string &name);
    void open(const std::string &name);

#if defined(PYCPP_HAVE_WFOPEN)
    json_file_reader(const std::wstring &name);
    void open(const std::wstring &name);
#endif

private:
    ifstream file_;
};


/**
 *  \brief Reader for string-based document.
 */
struct json_string_reader: json_stream_reader
{
public:
    json_string_reader();
    json_string_reader(const std::string &str);
    void open(const std::string &str);

private:
    std::istringstream sstream_;
};

PYCPP_END_NAMESPACE
