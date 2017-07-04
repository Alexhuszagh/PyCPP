//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <json/dom.h>
#include <sstream>
#include <stdexcept>


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


json_value_t::json_value_t():
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_value_t&& other):
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{
    swap(other);
}


json_value_t& json_value_t::operator=(json_value_t&& other)
{
    swap(other);
    return *this;
}


json_value_t::json_value_t(json_type type):
    type_(type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_null_t):
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(bool value):
    type_(json_boolean_type),
    data_(reinterpret_cast<json_pointer_t>(new json_boolean_t(value)))
{}


json_value_t::json_value_t(double value):
    type_(json_number_type),
    data_(reinterpret_cast<json_pointer_t>(new json_number_t(value)))
{}


json_value_t::json_value_t(json_string_t&& value):
    type_(json_string_type),
    data_(reinterpret_cast<json_pointer_t>(new json_string_t(std::move(value))))
{}


json_value_t::json_value_t(json_array_t&& value):
    type_(json_array_type),
    data_(reinterpret_cast<json_pointer_t>(new json_array_t(std::move(value))))
{}


json_value_t::json_value_t(json_object_t&& value):
    type_(json_object_type),
    data_(reinterpret_cast<json_pointer_t>(new json_object_t(std::move(value))))
{}


json_value_t::~json_value_t()
{
    reset();
}


void json_value_t::swap(json_value_t& other)
{
    std::swap(type_, other.type_);
    std::swap(data_, other.data_);
}


json_type json_value_t::type() const
{
    return type_;
}


bool json_value_t::has_null() const
{
    return type() == json_null_type;
}


bool json_value_t::has_boolean() const
{
    return type() == json_boolean_type;
}


bool json_value_t::has_number() const
{
    return type() == json_number_type;
}


bool json_value_t::has_string() const
{
    return type() == json_string_type;
}


bool json_value_t::has_array() const
{
    return type() == json_array_type;
}


bool json_value_t::has_object() const
{
    return type() == json_object_type;
}


json_null_t json_value_t::get_null() const
{
    if (type_ != json_null_type) {
        throw std::runtime_error("Type is not null.");
    }
    return nullptr;
}


json_boolean_t json_value_t::get_boolean() const
{
    if (type_ != json_boolean_type) {
        throw std::runtime_error("Type is not boolean.");
    }
    return *reinterpret_cast<json_boolean_t*>(data_);
}


json_number_t json_value_t::get_number() const
{
    if (type_ != json_number_type) {
        throw std::runtime_error("Type is not a number.");
    }

    return *reinterpret_cast<json_number_t*>(data_);
}


json_string_t& json_value_t::get_string() const
{
    if (type_ != json_string_type) {
        throw std::runtime_error("Type is not a string.");
    }
    if (!data_) {
        throw std::runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_string_t*>(data_);
}


json_array_t& json_value_t::get_array() const
{
    if (type_ != json_array_type) {
        throw std::runtime_error("Type is not an array.");
    }
    if (!data_) {
        throw std::runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_array_t*>(data_);
}


json_object_t& json_value_t::get_object() const
{
    if (type_ != json_object_type) {
        throw std::runtime_error("Type is not an object.");
    }
    if (!data_) {
        throw std::runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_object_t*>(data_);
}


void json_value_t::set_null(json_null_t value)
{
    reset();
    data_ = (json_pointer_t) nullptr;
    type_ = json_null_type;
}


void json_value_t::set_boolean(json_boolean_t value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_boolean_t(value));
    type_ = json_boolean_type;
}


void json_value_t::set_number(json_number_t value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_number_t(value));
    type_ = json_number_type;
}


void json_value_t::set_string(json_string_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_string_t(std::move(value)));
    type_ = json_string_type;
}


void json_value_t::set_array(json_array_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_array_t(std::move(value)));
    type_ = json_array_type;
}


void json_value_t::set_object(json_object_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_object_t(std::move(value)));
    type_ = json_object_type;
}


void json_value_t::set(json_null_t value)
{
    set_null(value);
}


void json_value_t::set(json_boolean_t value)
{
    set_boolean(value);
}


void json_value_t::set(json_number_t value)
{
    set_number(value);
}


void json_value_t::set(json_string_t&& value)
{
    set_string(std::forward<json_string_t>(value));
}


void json_value_t::set(json_array_t&& value)
{
    set_array(std::forward<json_array_t>(value));
}


void json_value_t::set(json_object_t&& value)
{
    set_object(std::forward<json_object_t>(value));
}


json_value_t::operator json_boolean_t() const
{
    return get_boolean();
}


json_value_t::operator json_number_t() const
{
    return get_number();
}


json_value_t::operator json_string_t&() const
{
    return get_string();
}


json_value_t::operator json_array_t&() const
{
    return get_array();
}


json_value_t::operator json_object_t&() const
{
    return get_object();
}


void json_value_t::reset()
{
    switch (type_) {
        case json_null_type:
            break;
        case json_boolean_type:
            delete (json_boolean_t*) data_;
            break;
        case json_number_type:
            delete (json_number_t*) data_;
            break;
        case json_string_type:
            delete (json_string_t*) data_;
            break;
        case json_array_type:
            delete (json_array_t*) data_;
            break;
        case json_object_type:
            delete (json_object_t*) data_;
            break;
        default:
            throw std::runtime_error("Unexpected JSON value type.");
    }

    type_ = json_null_type;
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
    reader.parse(stream);
}


void json_document_t::load(const std::string& path)
{
    ifstream stream(path);
    load(stream);
}


#if defined(HAVE_WFOPEN)

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


#if defined(HAVE_WFOPEN)

void json_document_t::dump(const std::wstring& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif
