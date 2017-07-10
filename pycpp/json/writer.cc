//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

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


json_stream_writer::json_stream_writer(char c, int width)
{
    set_indent(c, width);
}


json_stream_writer::json_stream_writer(std::ostream& s, char c, int width)
{
    set_indent(c, width);
    reset(s);
}


json_stream_writer::~json_stream_writer()
{
    delete (rapidjson_ostream*) stream_;
    delete (rapidjson_prettywriter*) writer_;
}


void json_stream_writer::reset(std::ostream& s)
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
    w->StartObject();
}


void json_stream_writer::end_object()
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->EndObject();
}


void json_stream_writer::start_array()
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->StartArray();
}


void json_stream_writer::end_array()
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->EndArray();
}


void json_stream_writer::key(const string_view& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->Key(value.data(), value.size());
}


void json_stream_writer::null()
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->Null();
}


void json_stream_writer::boolean(bool value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->Bool(value);
}


void json_stream_writer::number(double value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->Double(value);
}


void json_stream_writer::string(const string_view& value)
{
    auto w = (rapidjson_prettywriter*) writer_;
    w->String(value.data(), value.size());
}


json_file_writer::json_file_writer()
{
    reset(file_);
}


json_file_writer::json_file_writer(const std::string &name):
    file_(name)
{
    reset(file_);
}


void json_file_writer::open(const std::string &name)
{
    file_.open(name, std::ios_base::binary);
}

#if defined(PYCPP_HAVE_WFOPEN)


json_file_writer::json_file_writer(const std::wstring &name):
    file_(name)
{
    reset(file_);
}


void json_file_writer::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::binary);
}

#endif

PYCPP_END_NAMESPACE
