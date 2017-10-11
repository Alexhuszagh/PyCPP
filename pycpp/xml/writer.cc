//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/xml/writer.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlwriter.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static int stream_write(void* ctx, const char* buffer, int length)
{
    auto* stream = (std::ostream*) ctx;
    try {
        stream->write(buffer, length);
    } catch(...) {
        return -1;
    }

    return length;
}


static int stream_close(void*)
{
    return 0;
}

// OBJECTS
// -------


void xml_writer::start_element(const string_view&)
{}


void xml_writer::end_element()
{}


void xml_writer::write_attribute(const string_view&, const string_view&)
{}


void xml_writer::write_text(const string_view&)
{}


void xml_writer::write_comment(const string_view&)
{}


void xml_writer::start_element_ns(const string_view&, const string_view&, const string_view&)
{}


void xml_writer::end_element_ns()
{}


void xml_writer::write_attribute_ns(const string_view&, const string_view&, const string_view&, const string_view&)
{}


xml_stream_writer::xml_stream_writer(char c, int width)
{
    set_indent(c, width);
}


xml_stream_writer::xml_stream_writer(std::ostream& s, char c, int width)
{
    set_indent(c, width);
    open(s);
}


xml_stream_writer::~xml_stream_writer()
{
    if (writer_) {
        xmlTextWriterEndDocument((xmlTextWriterPtr) writer_);
        xmlFreeTextWriter((xmlTextWriterPtr) writer_);
    }
}


void xml_stream_writer::open(std::ostream& s)
{
    // cleanup
    if (writer_) {
        xmlFreeTextWriter((xmlTextWriterPtr) writer_);
    }

    // reset
    stream_ = (void*) xmlOutputBufferCreateIO(
        stream_write,
        stream_close,
        &s,
        nullptr
    );
    writer_ = (void*) xmlNewTextWriter((xmlOutputBufferPtr) stream_);
    if (writer_) {
        auto* writer = (xmlTextWriterPtr) writer_;
        std::string indent(indent_width_, indent_character_);
        xmlTextWriterSetIndent(writer, indent_width_);
        xmlTextWriterSetIndentString(writer, (const xmlChar*) indent.data());
        xmlTextWriterStartDocument(writer, nullptr, "utf-8", nullptr);
    }
}


void xml_stream_writer::set_indent(char c, int width)
{
    indent_character_ = c;
    indent_width_ = width;
}


void xml_stream_writer::start_element(const string_view& name)
{
    xmlTextWriterStartElement((xmlTextWriterPtr) writer_, (const xmlChar*) name.data());
}


void xml_stream_writer::end_element()
{
    xmlTextWriterEndElement((xmlTextWriterPtr) writer_);
}


void xml_stream_writer::write_attribute(const string_view& name, const string_view& value)
{
    auto name_ = (const xmlChar*) name.data();
    auto value_ = (const xmlChar*) value.data();
    xmlTextWriterWriteAttribute((xmlTextWriterPtr) writer_, name_, value_);
}


void xml_stream_writer::write_text(const string_view& text)
{
    xmlTextWriterWriteString((xmlTextWriterPtr) writer_, (const xmlChar*) text.data());
}


void xml_stream_writer::write_comment(const string_view& comment)
{
    xmlTextWriterWriteComment((xmlTextWriterPtr) writer_, (const xmlChar*) comment.data());
}


void xml_stream_writer::start_element_ns(const string_view& uri, const string_view& prefix, const string_view& localname)
{
    auto uri_ = (const xmlChar*) uri.data();
    auto prefix_ = (const xmlChar*) prefix.data();
    auto localname_ = (const xmlChar*) localname.data();
    xmlTextWriterStartElementNS((xmlTextWriterPtr) writer_, prefix_, localname_, uri_);
}


void xml_stream_writer::end_element_ns()
{
    xmlTextWriterEndElement((xmlTextWriterPtr) writer_);
}


void xml_stream_writer::write_attribute_ns(const string_view& uri, const string_view& prefix, const string_view& localname, const string_view& value)
{
    auto uri_ = (const xmlChar*) uri.data();
    auto prefix_ = (const xmlChar*) prefix.data();
    auto localname_ = (const xmlChar*) localname.data();
    auto value_ = (const xmlChar*) value.data();
    xmlTextWriterWriteAttributeNS((xmlTextWriterPtr) writer_, prefix_, localname_, uri_, value_);
}


xml_file_writer::xml_file_writer()
{}


xml_file_writer::xml_file_writer(const std::string &name)
{
    open(name);
}


void xml_file_writer::open(const std::string &name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    xml_stream_writer::open(file_);
}

#if defined(PYCPP_HAVE_WFOPEN)


xml_file_writer::xml_file_writer(const std::wstring &name)
{
    open(name);
}


void xml_file_writer::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    xml_stream_writer::open(file_);
}


xml_file_writer::xml_file_writer(const std::u16string &name)
{
    open(name);
}


void xml_file_writer::open(const std::u16string &name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    xml_stream_writer::open(file_);
}

#endif


xml_string_writer::xml_string_writer()
{
    xml_stream_writer::open(sstream_);
}


std::string xml_string_writer::str() const
{
    return sstream_.str();
}

PYCPP_END_NAMESPACE
