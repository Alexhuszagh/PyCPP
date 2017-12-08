//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON generic writer.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/json/core.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/view/string.h>

#include <sstream>

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
    virtual void flush() const;
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

    void open(std::ostream&);
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
    virtual void flush() const override;

private:
    char indent_character_ = ' ';
    int indent_width_ = 0;
    void* stream_ = nullptr;
    void* writer_ = nullptr;
};


/**
 *  \brief Writer for file-based document.
 */
struct json_file_writer: json_stream_writer
{
public:
    json_file_writer();
    json_file_writer(const string_view& name);
    void open(const string_view& name);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    json_file_writer(const wstring_view& name);
    void open(const wstring_view& name);
    json_file_writer(const u16string_view& name);
    void open(const u16string_view& name);
#endif                                          // WINDOWS
    virtual void flush() const override;

private:
    mutable ofstream file_;
};

/**
 *  \brief Writer for string-based document.
 */
struct json_string_writer: json_stream_writer
{
public:
    json_string_writer();
    std::string str() const;

private:
    std::ostringstream sstream_;
};

PYCPP_END_NAMESPACE
