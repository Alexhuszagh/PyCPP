//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <xml/dom.h>
#include <sstream>
#include <stdexcept>

// OBJECTS
// -------


void xml_document_t::loads(const std::string& data)
{
    std::istringstream stream(data);
    load(stream);
}


void xml_document_t::load(std::istream& stream)
{
// TODO: need to implement...
//    json_stream_reader reader;
//    json_dom_handler handler(*this);
//    reader.set_handler(handler);
//    reader.parse(stream);
}


void xml_document_t::load(const std::string& path)
{
    ifstream stream(path);
    load(stream);
}


#if defined(HAVE_WFOPEN)

void xml_document_t::load(const std::wstring& path)
{
    ifstream stream(path);
    load(stream);
}

#endif


std::string xml_document_t::dumps(char c, int width)
{
    std::ostringstream stream;
    dump(stream, c, width);
    return stream.str();
}


void xml_document_t::dump(std::ostream& stream, char c, int width)
{
// TODO: implement... (What's the indentation I want?)
//    json_stream_writer writer(stream, c, width);
//    dump_impl(*this, writer);
}


void xml_document_t::dump(const std::string& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}


#if defined(HAVE_WFOPEN)

void xml_document_t::dump(const std::wstring& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif
