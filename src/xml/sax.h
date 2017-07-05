//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief XML SAX parser.
 */

#pragma once

#include <xml/core.h>
#include <stream/fstream.h>
#include <view/string.h>

// OBJECTS
// -------


/**
 *  \brief SAX handler for an XML document.
 */
class xml_sax_handler
{
    virtual void start_document();
    virtual void end_document();
// TODO: need to consider how to handle element attributes...
//    virtual void start_element(const string_view& content);
//    virtual void end_element(const string_view& content);
//    virtual void characters(const string_view& content);
};


/**
 *  \brief Reader for stream-based document.
 */
struct xml_stream_reader
{
public:
    xml_stream_reader();

    void parse(std::istream&);
    void set_handler(xml_sax_handler&);

private:
    std::istream* stream_ = nullptr;
    xml_sax_handler* handler_ = nullptr;
};


/**
 *  \brief Reader for stream-based document.
 */
struct xml_file_reader: xml_stream_reader
{
public:
    xml_file_reader();
    xml_file_reader(const std::string &name);
    void open(const std::string &name);
    void parse(const std::string &name);

#if defined(HAVE_WFOPEN)
    xml_file_reader(const std::wstring &name);
    void open(const std::wstring &name);
    void parse(const std::wstring &name);
#endif

    void parse();

private:
    ifstream file_;
};
