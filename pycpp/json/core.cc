//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/core.h>
#include <pycpp/stl/stdexcept.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


json_value_t::json_value_t():
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_value_t&& rhs):
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{
    swap(rhs);
}


json_value_t& json_value_t::operator=(json_value_t&& rhs)
{
    swap(rhs);
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
    data_(reinterpret_cast<json_pointer_t>(new json_string_t(move(value))))
{}


json_value_t::json_value_t(json_array_t&& value):
    type_(json_array_type),
    data_(reinterpret_cast<json_pointer_t>(new json_array_t(move(value))))
{}


json_value_t::json_value_t(json_object_t&& value):
    type_(json_object_type),
    data_(reinterpret_cast<json_pointer_t>(new json_object_t(move(value))))
{}


json_value_t::~json_value_t()
{
    reset();
}


void json_value_t::swap(json_value_t& rhs)
{
    PYCPP_NAMESPACE::swap(type_, rhs.type_);
    PYCPP_NAMESPACE::swap(data_, rhs.data_);
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
        throw runtime_error("Type is not null.");
    }
    return nullptr;
}


json_boolean_t json_value_t::get_boolean() const
{
    if (type_ != json_boolean_type) {
        throw runtime_error("Type is not boolean.");
    }
    return *reinterpret_cast<json_boolean_t*>(data_);
}


json_number_t json_value_t::get_number() const
{
    if (type_ != json_number_type) {
        throw runtime_error("Type is not a number.");
    }

    return *reinterpret_cast<json_number_t*>(data_);
}


json_string_t& json_value_t::get_string() const
{
    if (type_ != json_string_type) {
        throw runtime_error("Type is not a string.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_string_t*>(data_);
}


json_array_t& json_value_t::get_array() const
{
    if (type_ != json_array_type) {
        throw runtime_error("Type is not an array.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_array_t*>(data_);
}


json_object_t& json_value_t::get_object() const
{
    if (type_ != json_object_type) {
        throw runtime_error("Type is not an object.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
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
    data_ = reinterpret_cast<json_pointer_t>(new json_string_t(move(value)));
    type_ = json_string_type;
}


void json_value_t::set_array(json_array_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_array_t(move(value)));
    type_ = json_array_type;
}


void json_value_t::set_object(json_object_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(new json_object_t(move(value)));
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
    set_string(forward<json_string_t>(value));
}


void json_value_t::set(json_array_t&& value)
{
    set_array(forward<json_array_t>(value));
}


void json_value_t::set(json_object_t&& value)
{
    set_object(forward<json_object_t>(value));
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
            throw runtime_error("Unexpected JSON value type.");
    }

    type_ = json_null_type;
}

PYCPP_END_NAMESPACE
