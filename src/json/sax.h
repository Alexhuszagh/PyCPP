//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON SAX parser.
 */

#pragma once

#include <json/core.h>
#include <json/object.h>            // TODO: remove
#include <stream/fstream.h>
#include <view/string.h>


// OBJECTS
// -------


/**
 *  \brief SAX handler for a JSON document.
 */
struct json_sax_handler
{
    virtual void start_document();
    virtual void end_document();
    virtual void start_object();
    virtual void end_object(size_t);
    virtual void start_array();
    virtual void end_array(size_t);
    virtual void key(const char*, size_t);
    virtual void null();
    virtual void boolean(bool);
    virtual void number(double);
    virtual void string(const char*, size_t);
};


/**
 *  \brief Reader for stream-based document.
 */
struct json_stream_reader
{
public:
    json_stream_reader();

    void parse(std::istream&);
    void set_handler(json_sax_handler&);

private:
    std::istream* stream = nullptr;
    json_sax_handler* handler = nullptr;
};


/**
 *  \brief Reader for stream-based document.
 */
struct json_file_reader: json_stream_reader
{
public:
    json_file_reader();
    json_file_reader(const std::string &name);
    void open(const std::string &name);
    void parse(const std::string &name);

#if defined(HAVE_WFOPEN)
    json_file_reader(const std::wstring &name);
    void open(const std::wstring &name);
    void parse(const std::wstring &name);
#endif

    void parse();

private:
    ifstream file;
};


// TODO: need a stream writer
