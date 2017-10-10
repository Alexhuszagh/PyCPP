//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/dom.h>
#include <sstream>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


template <typename Levels, typename... Ts>
static void add_value(Levels &levels, bool& has_key, json_string_t& key, Ts... ts)
{
    json_value_t* parent, *value;

    parent = levels.back();
    if (levels.size() == 1 && parent->has_null()) {
        // root element
        json_value_t value(std::forward<Ts>(ts)...);
        std::swap(*parent, value);
        levels.push_back(parent);
        return;
    } else if (has_key) {
        // adding to object
        auto &object = parent->get_object();
        object.emplace(key, json_value_t(std::forward<Ts>(ts)...));
        value = &object.at(key);
        key.clear();
        has_key = false;
    } else {
        // add to array
        auto &array = parent->get_array();
        array.push_back(json_value_t(std::forward<Ts>(ts)...));
        value = &array.back();
    }

    // add new level
    if (value->has_object() || value->has_array()) {
        levels.push_back(value);
    }
}


static void dump_impl(const json_value_t&, json_stream_writer&);


static void dump_array_impl(const json_array_t& array, json_stream_writer& writer)
{
    writer.start_array();
    for (const json_value_t& v: array) {
        dump_impl(v, writer);
    }
    writer.end_array();
}


static void dump_object_impl(const json_object_t& object, json_stream_writer& writer)
{
    writer.start_object();
    for (const auto& pair: object) {
        writer.key(string_view(pair.first));
        dump_impl(pair.second, writer);
    }
    writer.end_object();
}


static void dump_null_impl(json_stream_writer& writer)
{
    writer.null();
}


static void dump_bool_impl(const json_boolean_t& value, json_stream_writer& writer)
{
    writer.boolean(value);
}


static void dump_number_impl(const json_number_t& value, json_stream_writer& writer)
{
    writer.number(value);
}


static void dump_string_impl(const json_string_t& value, json_stream_writer& writer)
{
    writer.string(value);
}


static void dump_impl(const json_value_t& value, json_stream_writer& writer)
{
    switch (value.type()) {
        case json_null_type:
            dump_null_impl(writer);
            break;
        case json_boolean_type:
            dump_bool_impl(value.get_boolean(), writer);
            break;
        case json_number_type:
            dump_number_impl(value.get_number(), writer);
            break;
        case json_string_type:
            dump_string_impl(value.get_string(), writer);
            break;
        case json_array_type:
            dump_array_impl(value.get_array(), writer);
            break;
        case json_object_type:
            dump_object_impl(value.get_object(), writer);
            break;
        default:
            throw std::runtime_error("Unexpected JSON value type.");
    }
}


// OBJECTS
// -------


json_dom_handler::json_dom_handler(json_value_t& root):
    root_(&root)
{}


void json_dom_handler::start_document()
{
    levels_.emplace_back(root_);
}


void json_dom_handler::end_document()
{
    levels_.pop_back();
}


void json_dom_handler::start_object()
{
    add_value(levels_, has_key_, key_, json_object_t());
}


void json_dom_handler::end_object(size_t)
{
    levels_.pop_back();
}


void json_dom_handler::start_array()
{
    add_value(levels_, has_key_, key_, json_array_t());
}


void json_dom_handler::end_array(size_t)
{
    levels_.pop_back();
}


void json_dom_handler::key(const string_view& str)
{
    has_key_ = true;
    key_ = json_string_t(str);
}


void json_dom_handler::null()
{
    add_value(levels_, has_key_, key_, nullptr);
}


void json_dom_handler::boolean(bool v)
{
    add_value(levels_, has_key_, key_, v);
}


void json_dom_handler::number(double d)
{
    add_value(levels_, has_key_, key_, d);
}


void json_dom_handler::string(const string_view& str)
{
    add_value(levels_, has_key_, key_, json_string_t(str));
}


void json_document_t::loads(const std::string& data)
{
    std::istringstream stream(data);
    load(stream);
}


void json_document_t::load(std::istream& stream)
{
    json_stream_reader reader;
    json_dom_handler handler(*this);
    reader.set_handler(handler);
    reader.open(stream);
}


void json_document_t::load(const std::string& path)
{
    ifstream stream(path);
    load(stream);
}


#if defined(PYCPP_HAVE_WFOPEN)

void json_document_t::load(const std::wstring& path)
{
    ifstream stream(path);
    load(stream);
}

#endif


std::string json_document_t::dumps(char c, int width)
{
    std::ostringstream stream;
    dump(stream, c, width);
    return stream.str();
}


void json_document_t::dump(std::ostream& stream, char c, int width)
{
    json_stream_writer writer(stream, c, width);
    dump_impl(*this, writer);
}


void json_document_t::dump(const std::string& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}


#if defined(PYCPP_HAVE_WFOPEN)

void json_document_t::dump(const std::wstring& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif

PYCPP_END_NAMESPACE
