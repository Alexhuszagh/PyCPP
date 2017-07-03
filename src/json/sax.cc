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

struct HandlerImpl: rapidjson::BaseReaderHandler<rapidjson::UTF8<>, HandlerImpl>
{
    HandlerImpl(json_sax_handler&);

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


HandlerImpl::HandlerImpl(json_sax_handler& h):
    handler(&h)
{}


bool HandlerImpl::Null()
{
    handler->null();
    return true;
}


bool HandlerImpl::Bool(bool value)
{
    handler->boolean(value);
    return true;
}


bool HandlerImpl::Int(int value)
{
    handler->number(value);
    return true;
}


bool HandlerImpl::Uint(unsigned value)
{
    handler->number(value);
    return true;
}


bool HandlerImpl::Int64(int64_t value)
{
    handler->number(value);
    return true;
}


bool HandlerImpl::Uint64(uint64_t value)
{
    handler->number(value);
    return true;
}


bool HandlerImpl::Double(double value)
{
    handler->number(value);
    return true;
}


bool HandlerImpl::String(const char* value, rapidjson::SizeType length, bool)
{
    handler->string(value, length);
    return true;
}


bool HandlerImpl::StartObject()
{
    handler->start_object();
    return true;
}


bool HandlerImpl::EndObject(rapidjson::SizeType length)
{
    handler->end_object(length);
    return true;
}


bool HandlerImpl::Key(const char* value, rapidjson::SizeType length, bool)
{
    handler->key(value, length);
    return true;
}


bool HandlerImpl::StartArray()
{
    handler->start_array();
    return true;
}


bool HandlerImpl::EndArray(rapidjson::SizeType length)
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


void json_sax_handler::key(const char*, size_t)
{}


void json_sax_handler::null()
{}


void json_sax_handler::boolean(bool)
{}


void json_sax_handler::number(double)
{}


void json_sax_handler::string(const char*, size_t)
{}


json_stream_reader::json_stream_reader()
{}


void json_stream_reader::parse(std::istream& s)
{
    stream = &s;
    if (!handler) {
        throw std::runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler->start_document();
    HandlerImpl impl(*handler);
    rapidjson::Reader reader;
    rapidjson::IStreamWrapper istream(*stream);
    reader.Parse(istream, impl);
    handler->end_document();
}


void json_stream_reader::set_handler(json_sax_handler& h)
{
    handler = &h;
}


json_file_reader::json_file_reader()
{}


json_file_reader::json_file_reader(const std::string &name)
{
    open(name);
}


void json_file_reader::open(const std::string &name)
{
    file.open(name);
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
    file.open(name);
}


void json_file_reader::parse(const std::wstring &name)
{
    open(name);
    parse();
}

#endif


void json_file_reader::parse()
{
    if (!file.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    json_stream_reader::parse(file);
}
