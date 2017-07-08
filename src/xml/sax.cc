//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <xml/sax.h>
// TODO: need to include libxml2


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
// TODO: need to define the private implementation
//    HandlerImpl impl(*handler_);
//    rapidjson::Reader reader;
//    rapidjson::IStreamWrapper istream(*stream_);
//    reader.Parse(istream, impl);
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
