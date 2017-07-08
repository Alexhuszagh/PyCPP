//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <xml/sax.h>


// HELPERS
// -------


int stream_read(std::istream* stream, char* buffer, int length)
{
    stream->read(buffer, length);
    return stream->gcount();
}



int stream_close(std::istream*)
{
    return 0;
}


void error_handler(void *ctx, const char *msg, ...)
{
    throw std::runtime_error(msg);
}


/**
 *  \brief Private text reader which calls the SAX events.
 */
struct text_reader
{
public:
    text_reader(std::istream&);
    ~text_reader();

    void parse(xml_sax_handler&);

private:
    std::istream* stream_ = nullptr;
    xmlTextReaderPtr xml_ = nullptr;
    int status_ = 0;
};


text_reader::text_reader(std::istream& stream):
    stream_(&stream)
{
    // TODO: I think I need this:
    // xmlCreateIOParserCtxt
    xml_ = xmlReaderForIO(
        reinterpret_cast<xmlInputReadCallback>(stream_read),
        reinterpret_cast<xmlInputCloseCallback>(stream_close),
        stream_,
        nullptr,
        nullptr,
        0
    );
    status_ = xml_ ? 1 : 0;
    if (status_) {
        xmlSetGenericErrorFunc(xml_, error_handler);
    }
}


text_reader::~text_reader()
{
    if (xml_) {
        xmlFreeTextReader(xml_);
    }
}


//enum class NodeType: int32_t
//{
//    NONE                    = XML_READER_TYPE_NONE,
//    START_ELEMENT           = XML_READER_TYPE_ELEMENT,
//    ATTRIBUTE               = XML_READER_TYPE_ATTRIBUTE,
//    TEXT                    = XML_READER_TYPE_TEXT,
//    CDATA                   = XML_READER_TYPE_CDATA,
//    ENTITY_REFERENCE        = XML_READER_TYPE_ENTITY_REFERENCE,
//    ENTITY                  = XML_READER_TYPE_ENTITY,
//    PROCESSING_INSTRUCTION  = XML_READER_TYPE_PROCESSING_INSTRUCTION,
//    COMMENT                 = XML_READER_TYPE_COMMENT,
//    DOCUMENT                = XML_READER_TYPE_DOCUMENT,
//    DOCUMENT_TYPE           = XML_READER_TYPE_DOCUMENT_TYPE,
//    DOCUMENT_FRAGMENT       = XML_READER_TYPE_DOCUMENT_FRAGMENT,
//    NOTATION                = XML_READER_TYPE_NOTATION,
//    WHITESPACE              = XML_READER_TYPE_WHITESPACE,
//    SIGNIFICANT_WHITESPACE  = XML_READER_TYPE_SIGNIFICANT_WHITESPACE,
//    END_ELEMENT             = XML_READER_TYPE_END_ELEMENT,
//    END_ENTITY              = XML_READER_TYPE_END_ENTITY,
//    XML_DECLARATION         = XML_READER_TYPE_XML_DECLARATION,
//};


void text_reader::parse(xml_sax_handler& handler)
{
    // xmlTextReaderConstName
    while ((status_ = xmlTextReaderRead(xml_)) == 1) {
        //auto *name = xmlTextReaderConstName(xml_);
        //

        switch (xmlTextReaderNodeType(xml_)) {
            case XML_READER_TYPE_ELEMENT:
                continue;
            case XML_READER_TYPE_ATTRIBUTE:
                // check if a namespace... xmlTextReaderIsNamespaceDecl
                continue;
            case XML_READER_TYPE_TEXT:
                continue;
            case XML_READER_TYPE_END_ELEMENT:
                continue;
            default:
                break;
        }

        // TODO:
        // TODO: check the current node type
        // TODO:
        // TODO
    }
}


// OBJECTS
// -------


void xml_sax_handler::start_document()
{}


void xml_sax_handler::end_document()
{}


void xml_sax_handler::start_element(const string_view& content, xml_attr_t &attrs)
{}


void xml_sax_handler::end_element(const string_view& content)
{}


void xml_sax_handler::characters(const string_view& content)
{}


xml_stream_reader::xml_stream_reader()
{}


void xml_stream_reader::parse(std::istream& s)
{
    stream_ = &s;
    if (!handler_) {
        throw std::runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler_->start_document();
    text_reader reader(s);
    reader.parse(*handler_);
    handler_->end_document();
}


void xml_stream_reader::set_handler(xml_sax_handler& h)
{
    handler_ = &h;
}


xml_file_reader::xml_file_reader()
{}


xml_file_reader::xml_file_reader(const std::string &name)
{
    open(name);
}


void xml_file_reader::open(const std::string &name)
{
    file_.open(name, std::ios_base::binary);
}


void xml_file_reader::parse(const std::string &name)
{
    open(name);
    parse();
}


#if defined(HAVE_WFOPEN)


xml_file_reader::xml_file_reader(const std::wstring &name)
{
    open(name);
}


void xml_file_reader::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::binary);
}


void xml_file_reader::parse(const std::wstring &name)
{
    open(name);
    parse();
}

#endif


void xml_file_reader::parse()
{
    if (!file_.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    xml_stream_reader::parse(file_);
}
