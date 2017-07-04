//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <json/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

// ALIAS
// -----

typedef rapidjson::OStreamWrapper rapidjson_ostream;
typedef rapidjson::PrettyWriter<rapidjson_ostream> rapidjson_prettywriter;
typedef rapidjson::Writer<rapidjson_ostream> rapidjson_writer;

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
    if (is_pretty()) {
        delete (rapidjson_prettywriter*) writer_;
    } else {
        delete (rapidjson_writer*) writer_;
    }
}


void json_stream_writer::reset(std::ostream& s)
{
    delete (rapidjson_ostream*) stream_;
    stream_ = (void*) new rapidjson_ostream(s);
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->Reset(*(rapidjson_ostream*) stream_);
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->Reset(*(rapidjson_ostream*) stream_);
    }
}


void json_stream_writer::set_indent(char c, int width)
{
    // need to delete the old writer
    if (is_pretty()) {
        delete (rapidjson_prettywriter*) writer_;
    } else {
        delete (rapidjson_writer*) writer_;
    }

    // set new writer
    pretty_ = bool(width);
    if (is_pretty()) {
        writer_ = (void*) new rapidjson_prettywriter;
        auto w = (rapidjson_prettywriter*) writer_;
        w->SetIndent(c, width);
    } else {
        writer_ = (void*) new rapidjson_writer;
    }
}


bool json_stream_writer::is_pretty() const
{
    return pretty_;
}


void json_stream_writer::start_object()
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->StartObject();
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->StartObject();
    }
}


void json_stream_writer::end_object()
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->EndObject();
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->EndObject();
    }
}


void json_stream_writer::start_array()
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->StartArray();
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->StartArray();
    }
}


void json_stream_writer::end_array()
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->EndArray();
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->EndArray();
    }
}


void json_stream_writer::key(const string_view& value)
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->Key(value.data(), value.size());
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->Key(value.data(), value.size());
    }
}


void json_stream_writer::null()
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->Null();
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->Null();
    }
}


void json_stream_writer::boolean(bool value)
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->Bool(value);
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->Bool(value);
    }
}


void json_stream_writer::number(double value)
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->Double(value);
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->Double(value);
    }
}


void json_stream_writer::string(const string_view& value)
{
    if (is_pretty()) {
        auto w = (rapidjson_prettywriter*) writer_;
        w->String(value.data(), value.size());
    } else {
        auto w = (rapidjson_writer*) writer_;
        w->String(value.data(), value.size());
    }
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

#if defined(HAVE_WFOPEN)


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
