//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <cassert>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

typedef rapidjson::OStreamWrapper rapidjson_ostream;
typedef rapidjson::PrettyWriter<rapidjson_ostream> rapidjson_prettywriter;

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


void json_writer::key(const string_view&)
{}


void json_writer::null()
{}


void json_writer::boolean(bool)
{}


void json_writer::number(double)
{}


void json_writer::string(const string_view&)
{}


void json_writer::flush() const
{}


json_stream_writer::json_stream_writer(char c, int width)
{
    set_indent(c, width);
}


json_stream_writer::json_stream_writer(std::ostream& s, char c, int width)
{
    set_indent(c, width);
    open(s);
}


json_stream_writer::~json_stream_writer()
{
    delete (rapidjson_ostream*) stream_;
    delete (rapidjson_prettywriter*) writer_;
}


void json_stream_writer::open(std::ostream& s)
{
    // cleanup
    delete (rapidjson_ostream*) stream_;
    delete (rapidjson_prettywriter*) writer_;

    // reset
    stream_ = (void*) new rapidjson_ostream(s);
    writer_ = (void*) new rapidjson_prettywriter(*(rapidjson_ostream*) stream_);
    auto w = (rapidjson_prettywriter*) writer_;
    w->Reset(*(rapidjson_ostream*) stream_);
    w->SetIndent(indent_character_, indent_width_);
}


void json_stream_writer::set_indent(char c, int width)
{
    indent_character_ = c;
    indent_width_ = width;
}


bool json_stream_writer::is_pretty() const
{
    return indent_width_ > 0;
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


void json_stream_writer::key(const string_view& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->Key(value.data(), value.size());
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


void json_stream_writer::string(const string_view& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    assert(w && "Writer pointer cannot be null.");
    w->String(value.data(), value.size());
}


void json_stream_writer::flush() const      // null-op
{}


json_file_writer::json_file_writer()
{}


json_file_writer::json_file_writer(const string_view& name)
{
    open(name);
}


void json_file_writer::open(const string_view& name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    json_stream_writer::open(file_);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


json_file_writer::json_file_writer(const wstring_view& name)
{
    open(name);
}


void json_file_writer::open(const wstring_view& name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    json_stream_writer::open(file_);
}


json_file_writer::json_file_writer(const u16string_view& name)
{
    open(name);
}


void json_file_writer::open(const u16string_view& name)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    json_stream_writer::open(file_);
}

#endif                                          // WINDOWS


void json_file_writer::flush() const
{
    json_stream_writer::flush();
    file_.flush();
}


json_string_writer::json_string_writer()
{
    json_stream_writer::open(sstream_);
}


std::string json_string_writer::str() const
{
    flush();
    return sstream_.str();
}

PYCPP_END_NAMESPACE
