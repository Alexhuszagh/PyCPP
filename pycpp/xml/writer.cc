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
    auto* stream = (ostream*) ctx;
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


void xml_writer::start_element(const string_wrapper&)
{}


void xml_writer::end_element()
{}


void xml_writer::write_attribute(const string_wrapper&, const string_wrapper&)
{}


void xml_writer::write_text(const string_wrapper&)
{}


void xml_writer::write_comment(const string_wrapper&)
{}


void xml_writer::start_element_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&)
{}


void xml_writer::end_element_ns()
{}


void xml_writer::write_attribute_ns(const string_wrapper&, const string_wrapper&, const string_wrapper&, const string_wrapper&)
{}


void xml_writer::flush() const
{}


xml_stream_writer::xml_stream_writer(char c, int width)
{
    set_indent(c, width);
}


xml_stream_writer::xml_stream_writer(ostream& s, char c, int width)
{
    set_indent(c, width);
    open(s);
}


xml_stream_writer::xml_stream_writer(xml_stream_writer&& rhs)
{
    swap(rhs);
}


xml_stream_writer& xml_stream_writer::operator=(xml_stream_writer&& rhs)
{
    swap(rhs);
    return *this;
}


xml_stream_writer::~xml_stream_writer()
{
    if (writer_) {
        xmlTextWriterEndDocument((xmlTextWriterPtr) writer_);
        xmlFreeTextWriter((xmlTextWriterPtr) writer_);
    }
    // Don't free stream_, the writer automatically cleans it up
}


void xml_stream_writer::open(ostream& s)
{
    // cleanup
    if (writer_) {
        xmlTextWriterEndDocument((xmlTextWriterPtr) writer_);
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


bool xml_stream_writer::is_pretty() const
{
    return indent_width_ > 0;
}


void xml_stream_writer::swap(xml_stream_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(indent_character_, rhs.indent_character_);
    swap(indent_width_, rhs.indent_width_);
    swap(stream_, rhs.stream_);
    swap(writer_, rhs.writer_);
}


void xml_stream_writer::start_element(const string_wrapper& name)
{
    xmlTextWriterStartElement((xmlTextWriterPtr) writer_, (const xmlChar*) name.data());
}


void xml_stream_writer::end_element()
{
    xmlTextWriterEndElement((xmlTextWriterPtr) writer_);
}


void xml_stream_writer::write_attribute(const string_wrapper& name, const string_wrapper& value)
{
    auto name_ = (const xmlChar*) name.data();
    auto value_ = (const xmlChar*) value.data();
    xmlTextWriterWriteAttribute((xmlTextWriterPtr) writer_, name_, value_);
}


void xml_stream_writer::write_text(const string_wrapper& text)
{
    xmlTextWriterWriteString((xmlTextWriterPtr) writer_, (const xmlChar*) text.data());
}


void xml_stream_writer::write_comment(const string_wrapper& comment)
{
    xmlTextWriterWriteComment((xmlTextWriterPtr) writer_, (const xmlChar*) comment.data());
}


void xml_stream_writer::start_element_ns(const string_wrapper& uri, const string_wrapper& prefix, const string_wrapper& localname)
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


void xml_stream_writer::write_attribute_ns(const string_wrapper& uri, const string_wrapper& prefix, const string_wrapper& localname, const string_wrapper& value)
{
    auto uri_ = (const xmlChar*) uri.data();
    auto prefix_ = (const xmlChar*) prefix.data();
    auto localname_ = (const xmlChar*) localname.data();
    auto value_ = (const xmlChar*) value.data();
    xmlTextWriterWriteAttributeNS((xmlTextWriterPtr) writer_, prefix_, localname_, uri_, value_);
}


void xml_stream_writer::flush() const
{
    xmlTextWriterFlush((xmlTextWriterPtr) writer_);
}


xml_file_writer::xml_file_writer(xml_file_writer&& rhs)
{
    swap(rhs);
}


xml_file_writer& xml_file_writer::operator=(xml_file_writer&& rhs)
{
    swap(rhs);
    return *this;
}


xml_file_writer::xml_file_writer(const string_view& name)
{
    open(name);
}


void xml_file_writer::open(const string_view& name)
{
    file_.open(name, ios_base::out | ios_base::binary);
    xml_stream_writer::open(file_);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


xml_file_writer::xml_file_writer(const wstring_view& name)
{
    open(name);
}


void xml_file_writer::open(const wstring_view& name)
{
    file_.open(name, ios_base::out | ios_base::binary);
    xml_stream_writer::open(file_);
}


xml_file_writer::xml_file_writer(const u16string_view& name)
{
    open(name);
}


void xml_file_writer::open(const u16string_view& name)
{
    file_.open(name, ios_base::out | ios_base::binary);
    xml_stream_writer::open(file_);
}

#endif                                          // WINDOWS


void xml_file_writer::flush() const
{
    xml_stream_writer::flush();
    file_.flush();
}


void xml_file_writer::swap(xml_file_writer& rhs)
{
    static_assert(false, "");       // TODO: implement
}


xml_string_writer::xml_string_writer()
{
    xml_stream_writer::open(sstream_);
}


xml_string_writer::xml_string_writer(xml_string_writer&& rhs):
    xml_string_writer()
{
    swap(rhs);
}


xml_string_writer& xml_string_writer::operator=(xml_string_writer&& rhs)
{
    swap(rhs);
    return *this;
}


std::string xml_string_writer::str() const
{
    flush();
    return sstream_.str();
}


void xml_string_writer::swap(xml_string_writer& rhs)
{
    static_assert(false, "");       // TODO: implement
}

PYCPP_END_NAMESPACE
