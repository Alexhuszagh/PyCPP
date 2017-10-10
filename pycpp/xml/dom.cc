//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/string/string.h>
#include <pycpp/xml/dom.h>
#include <sstream>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static void dump_impl(const xml_node_t& node, xml_stream_writer& writer)
{
    writer.start_element(node.get_tag());
    for (const auto& pair: node.get_attrs()) {
        writer.write_attribute(pair.first, pair.second);
    }
    writer.write_text(node.get_text());

    for (const xml_node_t& child: node.get_children()) {
        dump_impl(child, writer);
    }

    writer.end_element();
}

// OBJECTS
// -------


xml_string_t xml_node_t::tostring() const
{
    std::ostringstream stream;
    {
        xml_stream_writer writer(stream, ' ', 0);
        dump_impl(*this, writer);
    }

    // remove the XML declaration
    xml_string_t str = stream.str();
    size_t end_decl = str.find("?>\n");
    if (end_decl == str.npos) {
        return str;
    }
    return str.substr(end_decl + 3);
}


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


void xml_dom_handler::start_element(const string_view& name, xml_attr_t&& attrs)
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
    reader.open(stream);
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


void xml_document_t::load(const std::u16string& path)
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
    xml_stream_writer writer(stream, c, width);
    dump_impl(*this, writer);
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


void xml_document_t::dump(const std::u16string& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif

PYCPP_END_NAMESPACE
