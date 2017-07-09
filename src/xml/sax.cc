//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <xml/sax.h>
#include <cstring>


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


void internal_subset(void* ctx, const xmlChar* name,
                     const xmlChar* external_id,
                     const xmlChar * system_id)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
}


static void start_document(void* ctx)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    handler->start_document();
}


static void end_document(void* ctx)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    handler->end_document();
}


static void start_element(void * ctx, const xmlChar * name, const xmlChar ** attrs)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    // TODO: need to get the start/end element
    //handler->end_element(string_view((char*) name));
}


static void end_element(void * ctx, const xmlChar * name)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    handler->end_element(string_view((char*) name));
}


void start_element_ns(void* ctx, const xmlChar* localname,
                      const xmlChar* prefix, const xmlChar* uri,
                      int nb_namespaces, const xmlChar** namespaces,
                      int nb_attributes, int nb_defaulted,
                      const xmlChar** attrs)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    // TODO:
}


void end_element_ns(void* ctx, const xmlChar* localname,
                    const xmlChar* prefix, const xmlChar* uri)
{
    xml_sax_handler* handler = (xml_sax_handler*) ctx;
    // TODO:
}


/**
 *  \brief Transform libxml2 API to public SAX handler.
 */
struct handler_impl
{
    handler_impl(xml_sax_handler&);

private:
    xml_sax_handler* handler = nullptr;
    xmlSAXHandler impl;
};


handler_impl::handler_impl(xml_sax_handler& h):
    handler(&h)
{
    memset(&impl, 0, sizeof(impl));

// TODO: implement the rest of the callbacks
    impl.internalSubset = internal_subset;
//    isStandaloneSAXFunc isStandalone
//    hasInternalSubsetSAXFunc    hasInternalSubset
//    hasExternalSubsetSAXFunc    hasExternalSubset
//    resolveEntitySAXFunc    resolveEntity
//    getEntitySAXFunc    getEntity
//    entityDeclSAXFunc   entityDecl
//    notationDeclSAXFunc notationDecl
//    attributeDeclSAXFunc    attributeDecl
//    elementDeclSAXFunc  elementDecl
//    unparsedEntityDeclSAXFunc   unparsedEntityDecl
//    setDocumentLocatorSAXFunc   setDocumentLocator
    impl.startDocument = start_document;
    impl.endDocument = end_document;
    impl.startElement = start_element;
    impl.endElement = end_element;
//    referenceSAXFunc    reference
//    charactersSAXFunc   characters
//    ignorableWhitespaceSAXFunc  ignorableWhitespace
//    processingInstructionSAXFunc    processingInstruction
//    commentSAXFunc  comment
//    warningSAXFunc  warning
//    errorSAXFunc    error
//    fatalErrorSAXFunc   fatalError  : unused error() get all the errors
//    getParameterEntitySAXFunc   getParameterEntity
//    cdataBlockSAXFunc   cdataBlock
//    externalSubsetSAXFunc   externalSubset
    impl.initialized = XML_SAX2_MAGIC;
    impl.startElementNs = start_element_ns;
   impl.endElementNs = end_element_ns;
//    xmlStructuredErrorFunc  serror
}


struct reader_impl
{
//    void parse(std::istream&, );
};


//struct parser_state
//{
////    RetVal return_val;
////    StatesEnum state;
////    ...
//};


#if 0



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
    xmlSAXHandler handler_;
    parser_state state_;
};


text_reader::text_reader(std::istream& stream):
    stream_(&stream)
{
    //xmlSAXHandler saxHandler

    // TODO: I think I need this:
    // xmlCreateIOParserCtxt
//    xml_ = xmlCreateIOParserCtxt(
        // TODO: need sax_handler
        // TODO: need user_data
//        reinterpret_cast<xmlInputReadCallback>(stream_read),
//        reinterpret_cast<xmlInputCloseCallback>(stream_close),
//        stream_,  /* stream */
//        0         /* encoding */
//    );
//    status_ = xml_ ? 1 : 0;
//    if (status_) {
//        xmlSetGenericErrorFunc(xml_, error_handler);
//    }
}


text_reader::~text_reader()
{
//    if (xml_) {
//        xmlFreeTextReader(xml_);
//    }
}


void text_reader::parse(xml_sax_handler& handler)
{
//    // xmlTextReaderConstName
//    while ((status_ = xmlTextReaderRead(xml_)) == 1) {
//        //auto *name = xmlTextReaderConstName(xml_);
//        //
//
//        switch (xmlTextReaderNodeType(xml_)) {
//            case XML_READER_TYPE_ELEMENT:
//                continue;
//            case XML_READER_TYPE_ATTRIBUTE:
//                // check if a namespace... xmlTextReaderIsNamespaceDecl
//                continue;
//            case XML_READER_TYPE_TEXT:
//                continue;
//            case XML_READER_TYPE_END_ELEMENT:
//                continue;
//            default:
//                break;
//        }
//
//        // TODO:
//        // TODO: check the current node type
//        // TODO:
//        // TODO
//    }
}
#endif


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
// TODO: restore
    stream_ = &s;
    if (!handler_) {
        throw std::runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler_impl(*handler_);
//    handler_->start_document();
//    text_reader reader(s);
//    reader.parse(*handler_);
//    handler_->end_document();
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
