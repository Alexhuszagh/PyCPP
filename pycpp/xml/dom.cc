//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/xml/dom.h>
#include <sstream>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

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


void xml_dom_handler::start_element(const string_view& name, xml_attr_t &attrs)
{
    xml_node_t* parent = levels_.back();
    xml_node_list_t& list = parent->get_children();

    // create and append child
    xml_node_t child;
    child.set_tag(xml_string_t(name));
    child.set_attrs(std::forward<xml_attr_t>(attrs));
    list.push_back(std::move(child));

    levels_.emplace_back(&*list.rbegin());
}


void xml_dom_handler::end_element(const string_view& name)
{
    levels_.pop_back();
}


void xml_dom_handler::characters(const string_view& content)
{
    xml_node_t* current = levels_.back();
    current->set_text(current->get_text() + xml_string_t(content));
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


#if defined(PYCPP_HAVE_WFOPEN)

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


#if defined(PYCPP_HAVE_WFOPEN)

void xml_document_t::dump(const std::wstring& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif

PYCPP_END_NAMESPACE
