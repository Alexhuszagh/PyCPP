//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/new.h>
#include <pycpp/json/sax.h>
#include <pycpp/stl/stdexcept.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using rapidjson_istream = rapidjson::IStreamWrapper;
using rapidjson_reader = rapidjson::GenericReader<
    rapidjson::UTF8<>,
    rapidjson::UTF8<>,
    json_backend_allocator
>;

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
    handler->number(static_cast<double>(value));
    return true;
}


bool handler_impl::Uint64(uint64_t value)
{
    handler->number(static_cast<double>(value));
    return true;
}


bool handler_impl::Double(double value)
{
    handler->number(value);
    return true;
}


bool handler_impl::String(const char* value, rapidjson::SizeType length, bool)
{
    handler->string(string_wrapper(value, length));
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
    handler->key(string_wrapper(value, length));
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


void json_sax_handler::key(const string_wrapper&)
{}


void json_sax_handler::null()
{}


void json_sax_handler::boolean(bool)
{}


void json_sax_handler::number(double)
{}


void json_sax_handler::string(const string_wrapper&)
{}


json_stream_reader::json_stream_reader(json_stream_reader&& rhs) noexcept
{
    swap(rhs);
}


json_stream_reader& json_stream_reader::operator=(json_stream_reader&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


void json_stream_reader::open(istream& s)
{
    stream_ = &s;
    if (!handler_) {
        throw runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler_impl impl(*handler_);
    rapidjson_reader reader;
    rapidjson_istream istream(*stream_);
    handler_->start_document();
    reader.Parse(istream, impl);
    handler_->end_document();
}


void json_stream_reader::set_handler(json_sax_handler& h) noexcept
{
    handler_ = &h;
}


void json_stream_reader::swap(json_stream_reader& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(stream_, rhs.stream_);
    swap(handler_, rhs.handler_);
}


json_file_reader_base::json_file_reader_base():
    file_(json_allocator())
{}


json_file_reader::json_file_reader(json_file_reader&& rhs) noexcept
{
    swap(rhs);
}


json_file_reader& json_file_reader::operator=(json_file_reader&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_file_reader::json_file_reader(const string_view& name)
{
    open(name);
}


void json_file_reader::open(const string_view& name)
{
    file_->open(name, ios_base::in | ios_base::binary);
    json_stream_reader::open(*file_);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS


json_file_reader::json_file_reader(const wstring_view& name)
{
    open(name);
}


void json_file_reader::open(const wstring_view& name)
{
    file_->open(name, ios_base::in | ios_base::binary);
    json_stream_reader::open(*file_);
}


json_file_reader::json_file_reader(const u16string_view& name)
{
    open(name);
}


void json_file_reader::open(const u16string_view& name)
{
    file_->open(name, ios_base::in | ios_base::binary);
    json_stream_reader::open(*file_);
}

#endif                                          // WINDOWS


void json_file_reader::swap(json_file_reader& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    json_stream_reader::swap(rhs);
    swap(file_, rhs.file_);
}


json_string_reader_base::json_string_reader_base():
    sstream_(json_allocator())
{}


json_string_reader::json_string_reader(json_string_reader&& rhs) noexcept
{
    swap(rhs);
}


json_string_reader& json_string_reader::operator=(json_string_reader&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_string_reader::json_string_reader(const string_wrapper& str)
{
    open(str);
}


void json_string_reader::open(const string_wrapper& str)
{
    sstream_ = json_istringstream_t(json_string_t(str), ios_base::in | ios_base::binary);
    json_stream_reader::open(*sstream_);
}


void json_string_reader::swap(json_string_reader& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    json_stream_reader::swap(rhs);
    swap(sstream_, rhs.sstream_);
}

PYCPP_END_NAMESPACE
