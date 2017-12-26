//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/new.h>
#include <pycpp/json/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using rapidjson_ostream = rapidjson::OStreamWrapper;
using rapidjson_size_t = rapidjson::SizeType;
using rapidjson_prettywriter = rapidjson::PrettyWriter<
    rapidjson_ostream,
    rapidjson::UTF8<>,
    rapidjson::UTF8<>,
    json_backend_allocator
>;

// OBJECTS
// -------


void json_writer::start_object()
{}


void json_writer::end_object()
{}


void json_writer::start_array()
{}


void json_writer::end_array()
{}


void json_writer::key(const string_wrapper&)
{}


void json_writer::null()
{}


void json_writer::boolean(bool)
{}


void json_writer::number(double)
{}


void json_writer::string(const string_wrapper&)
{}


void json_writer::flush() const
{}


json_stream_writer::json_stream_writer(char c, int width)
{
    set_indent(c, width);
}


json_stream_writer::json_stream_writer(ostream& s, char c, int width)
{
    set_indent(c, width);
    open(s);
}


json_stream_writer::json_stream_writer(json_stream_writer&& rhs) noexcept
{
    swap(rhs);
}


json_stream_writer& json_stream_writer::operator=(json_stream_writer&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_stream_writer::~json_stream_writer() noexcept
{
    json_delete(reinterpret_cast<rapidjson_ostream*>(stream_));
    json_delete(reinterpret_cast<rapidjson_prettywriter*>(writer_));
}


void json_stream_writer::open(ostream& s)
{
    // cleanup
    json_delete(reinterpret_cast<rapidjson_ostream*>(stream_));
    json_delete(reinterpret_cast<rapidjson_prettywriter*>(writer_));

    // reset
    stream_ = reinterpret_cast<void*>(json_new<rapidjson_ostream>(s));
    writer_ = reinterpret_cast<void*>(json_new<rapidjson_prettywriter>(*(rapidjson_ostream*) stream_));
    auto writer = (rapidjson_prettywriter*) writer_;
    writer->Reset(*(rapidjson_ostream*) stream_);
    set_indent(indent_character_, indent_width_);
}


void json_stream_writer::set_indent(char c, int width)
{
    indent_character_ = c;
    indent_width_ = width;
    if (writer_) {
        auto writer = (rapidjson_prettywriter*) writer_;
        writer->SetIndent(indent_character_, indent_width_);
    }
}


bool json_stream_writer::is_pretty() const noexcept
{
    return indent_width_ > 0;
}


void json_stream_writer::swap(json_stream_writer& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(indent_character_, rhs.indent_character_);
    swap(indent_width_, rhs.indent_width_);
    swap(stream_, rhs.stream_);
    swap(writer_, rhs.writer_);
}


void json_stream_writer::start_object()
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->StartObject();
}


void json_stream_writer::end_object()
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->EndObject();
}


void json_stream_writer::start_array()
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->StartArray();
}


void json_stream_writer::end_array()
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->EndArray();
}


void json_stream_writer::key(const string_wrapper& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->Key(value.data(), static_cast<rapidjson_size_t>(value.size()));
}


void json_stream_writer::null()
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->Null();
}


void json_stream_writer::boolean(bool value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->Bool(value);
}


void json_stream_writer::number(double value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->Double(value);
}


void json_stream_writer::string(const string_wrapper& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->String(value.data(), static_cast<rapidjson_size_t>(value.size()));
}


void json_stream_writer::flush() const      // null-op
{}


json_file_writer_base::json_file_writer_base():
    file_(json_allocator())
{}


json_file_writer::json_file_writer(json_file_writer&& rhs) noexcept
{
    swap(rhs);
}


json_file_writer& json_file_writer::operator=(json_file_writer&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_file_writer::json_file_writer(const string_view& name)
{
    open(name);
}


void json_file_writer::open(const string_view& name)
{
    file_->open(name, ios_base::out | ios_base::binary);
    json_stream_writer::open(*file_);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


json_file_writer::json_file_writer(const wstring_view& name)
{
    open(name);
}


void json_file_writer::open(const wstring_view& name)
{
    file_->open(name, ios_base::out | ios_base::binary);
    json_stream_writer::open(*file_);
}


json_file_writer::json_file_writer(const u16string_view& name)
{
    open(name);
}


void json_file_writer::open(const u16string_view& name)
{
    file_->open(name, ios_base::out | ios_base::binary);
    json_stream_writer::open(*file_);
}

#endif                                          // WINDOWS


void json_file_writer::flush() const
{
    json_stream_writer::flush();
    file_->flush();
}


void json_file_writer::swap(json_file_writer& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    json_stream_writer::swap(rhs);
    swap(file_, rhs.file_);
}


json_string_writer_base::json_string_writer_base():
    sstream_(json_allocator())
{}


json_string_writer::json_string_writer()
{
    json_stream_writer::open(*sstream_);
}


json_string_writer::json_string_writer(json_string_writer&& rhs):
    json_string_writer()
{
    swap(rhs);
}


json_string_writer& json_string_writer::operator=(json_string_writer&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_string_t json_string_writer::str() const
{
    flush();
    return sstream_->str();
}


void json_string_writer::swap(json_string_writer& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    json_stream_writer::swap(rhs);
    swap(sstream_, rhs.sstream_);
}

PYCPP_END_NAMESPACE
