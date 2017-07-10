//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON generic writer.
 */

#pragma once

#include <pycpp/json/core.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Generic writer for a JSON document.
 */
struct json_writer
{
    virtual void start_object();
    virtual void end_object();
    virtual void start_array();
    virtual void end_array();
    virtual void key(const string_view&);
    virtual void null();
    virtual void boolean(bool);
    virtual void number(double);
    virtual void string(const string_view&);
};


/**
 *  \brief Writer for stream-based document.
 */
struct json_stream_writer: json_writer
{
public:
    json_stream_writer(char = ' ', int = 4);
    json_stream_writer(std::ostream&, char = ' ', int = 4);
    ~json_stream_writer();

    void reset(std::ostream&);
    void set_indent(char = ' ', int = 4);
    bool is_pretty() const;

    virtual void start_object() override;
    virtual void end_object() override;
    virtual void start_array() override;
    virtual void end_array() override;
    virtual void key(const string_view&) override;
    virtual void null() override;
    virtual void boolean(bool) override;
    virtual void number(double) override;
    virtual void string(const string_view&) override;

private:
    bool pretty_ = false;
    void* stream_ = nullptr;
    void* writer_ = nullptr;
};


/**
 *  \brief Writer for stream-based document.
 */
struct json_file_writer: json_stream_writer
{
public:
    json_file_writer();
    json_file_writer(const std::string &name);
    void open(const std::string &name);

#if defined(PYCPP_HAVE_WFOPEN)
    json_file_writer(const std::wstring &name);
    void open(const std::wstring &name);
#endif

private:
    ofstream file_;
};

PYCPP_END_NAMESPACE
