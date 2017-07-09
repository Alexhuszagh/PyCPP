//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <xml/dom.h>
#include <sstream>
#include <stdexcept>

// OBJECTS
// -------


xml_dom_handler::xml_dom_handler(xml_node_t& root):
    root_(&root)
{}


void xml_dom_handler::start_document()
{
    levels_.emplace_back(root_);
}


void xml_dom_handler::end_document()
{
    levels_.pop_back();
}


void xml_dom_handler::start_element(const string_view& content, xml_attr_t &attrs)
{
//    xml_node_t* parent = levels_.back();
//    xml_node_t child(parent->get_children());
//    child.set_tag(xml_string_t(content));
//    child.set_attrs(std::forward<xml_attr_t>(attrs));
}


void xml_dom_handler::end_element(const string_view& content)
{
//    levels_.pop_back();
}


void xml_dom_handler::characters(const string_view& content)
{
    xml_node_t* current = levels_.back();
    current->set_text(xml_string_t(content));
}


void xml_document_t::loads(const std::string& data)
{
    std::istringstream stream(data);
    load(stream);
}


void xml_document_t::load(std::istream& stream)
{
    xml_stream_reader reader;
    xml_dom_handler handler(*this);
    reader.set_handler(handler);
    reader.parse(stream);
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
// TODO: implement...
//      (What's the indentation I want?)
//    xml_stream_writer writer(stream, c, width);
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
