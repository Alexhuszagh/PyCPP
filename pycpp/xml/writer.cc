//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/xml/writer.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlwriter.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


int stream_write(void* ctx, const char* buffer, int length)
{
    auto* stream = (std::ostream*) ctx;
    try {
        stream->write(buffer, length);
    } catch(...) {
        return -1;
    }

    return length;
}


int stream_close(void*)
{
    return 0;
}

// OBJECTS
// -------


void xml_writer::start_element(const string_view&)
{}


void xml_writer::end_element()
{}


xml_stream_writer::xml_stream_writer(char c, int width)
{
    set_indent(c, width);
}


xml_stream_writer::xml_stream_writer(std::ostream& s, char c, int width)
{
    set_indent(c, width);
    reset(s);
}


xml_stream_writer::~xml_stream_writer()
{
    if (writer_) {
        xmlTextWriterEndDocument((xmlTextWriterPtr) writer_);
    }
    if (stream_) {
        xmlOutputBufferClose((xmlOutputBufferPtr) stream_);
    }
    if (writer_) {
        xmlFreeTextWriter((xmlTextWriterPtr) writer_);
    }
}


void xml_stream_writer::reset(std::ostream& s)
{
    // cleanup
    if (stream_) {
        xmlOutputBufferClose((xmlOutputBufferPtr) stream_);
    }
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


void xml_stream_writer::start_element(const string_view&)
{
    // TODO: implement...
}


void xml_stream_writer::end_element()
{
    // TODO: implement...
}


xml_file_writer::xml_file_writer()
{
    reset(file_);
}


xml_file_writer::xml_file_writer(const std::string &name):
    file_(name)
{
    reset(file_);
}


void xml_file_writer::open(const std::string &name)
{
    file_.open(name, std::ios_base::binary);
}

#if defined(PYCPP_HAVE_WFOPEN)


xml_file_writer::xml_file_writer(const std::wstring &name):
    file_(name)
{
    reset(file_);
}


void xml_file_writer::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::binary);
}

#endif


PYCPP_END_NAMESPACE
