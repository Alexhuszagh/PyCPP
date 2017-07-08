//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <json/sax.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>

// HELPERS
// -------

/**
 *  \brief Transform RapidJSON API to public SAX handler.
 */

struct handler_impl: rapidjson::BaseReaderHandler<rapidjson::UTF8<>, handler_impl>
{
    handler_impl(json_sax_handler&);

    // HANDLER
    bool Null();
    bool Bool(bool);
    bool Int(int);
    bool Uint(unsigned);
    bool Int64(int64_t);
    bool Uint64(uint64_t);
    bool Double(double);
    bool String(const char*, rapidjson::SizeType, bool);
    bool StartObject();
    bool EndObject(rapidjson::SizeType);
    bool Key(const char*, rapidjson::SizeType, bool);
    bool StartArray();
    bool EndArray(rapidjson::SizeType);

private:
    json_sax_handler* handler = nullptr;
};


handler_impl::handler_impl(json_sax_handler& h):
    handler(&h)
{}


bool handler_impl::Null()
{
    handler->null();
    return true;
}


bool handler_impl::Bool(bool value)
{
    handler->boolean(value);
    return true;
}


bool handler_impl::Int(int value)
{
    handler->number(value);
    return true;
}


bool handler_impl::Uint(unsigned value)
{
    handler->number(value);
    return true;
}


bool handler_impl::Int64(int64_t value)
{
    handler->number(value);
    return true;
}


bool handler_impl::Uint64(uint64_t value)
{
    handler->number(value);
    return true;
}


bool handler_impl::Double(double value)
{
    handler->number(value);
    return true;
}


bool handler_impl::String(const char* value, rapidjson::SizeType length, bool)
{
    handler->string(string_view(value, length));
    return true;
}


bool handler_impl::StartObject()
{
    handler->start_object();
    return true;
}


bool handler_impl::EndObject(rapidjson::SizeType length)
{
    handler->end_object(length);
    return true;
}


bool handler_impl::Key(const char* value, rapidjson::SizeType length, bool)
{
    handler->key(string_view(value, length));
    return true;
}


bool handler_impl::StartArray()
{
    handler->start_array();
    return true;
}


bool handler_impl::EndArray(rapidjson::SizeType length)
{
    handler->end_array(length);
    return true;
}


// OBJECTS
// -------


void json_sax_handler::start_document()
{}


void json_sax_handler::end_document()
{}


void json_sax_handler::start_object()
{}


void json_sax_handler::end_object(size_t)
{}


void json_sax_handler::start_array()
{}


void json_sax_handler::end_array(size_t)
{}


void json_sax_handler::key(const string_view&)
{}


void json_sax_handler::null()
{}


void json_sax_handler::boolean(bool)
{}


void json_sax_handler::number(double)
{}


void json_sax_handler::string(const string_view&)
{}


json_stream_reader::json_stream_reader()
{}


void json_stream_reader::parse(std::istream& s)
{
    stream_ = &s;
    if (!handler_) {
        throw std::runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler_->start_document();
    handler_impl impl(*handler_);
    rapidjson::Reader reader;
    rapidjson::IStreamWrapper istream(*stream_);
    reader.Parse(istream, impl);
    handler_->end_document();
}


void json_stream_reader::set_handler(json_sax_handler& h)
{
    handler_ = &h;
}


json_file_reader::json_file_reader()
{}


json_file_reader::json_file_reader(const std::string &name)
{
    open(name);
}


void json_file_reader::open(const std::string &name)
{
    file_.open(name, std::ios_base::binary);
}


void json_file_reader::parse(const std::string &name)
{
    open(name);
    parse();
}


#if defined(HAVE_WFOPEN)


json_file_reader::json_file_reader(const std::wstring &name)
{
    open(name);
}


void json_file_reader::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::binary);
}


void json_file_reader::parse(const std::wstring &name)
{
    open(name);
    parse();
}

#endif


void json_file_reader::parse()
{
    if (!file_.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    json_stream_reader::parse(file_);
}
