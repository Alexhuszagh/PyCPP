//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/json/core.h>
#include <pycpp/json/new.h>
#include <pycpp/stl/stdexcept.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


json_value_t::json_value_t() noexcept:
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_value_t&& rhs) noexcept:
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{
    swap(rhs);
}


json_value_t& json_value_t::operator=(json_value_t&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


json_value_t::json_value_t(json_type type) noexcept:
    type_(type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_null_t&&) noexcept:
    type_(json_null_type),
    data_((json_pointer_t) nullptr)
{}


json_value_t::json_value_t(json_boolean_t&& value):
    type_(json_boolean_type),
    data_(reinterpret_cast<json_pointer_t>(json_new<json_boolean_t>(move(value))))
{}


json_value_t::json_value_t(json_number_t&& value):
    type_(json_number_type),
    data_(reinterpret_cast<json_pointer_t>(json_new<json_number_t>(move(value))))
{}


json_value_t::json_value_t(json_string_t&& value):
    type_(json_string_type),
    data_(reinterpret_cast<json_pointer_t>(json_new<json_string_t>(move(value), json_allocator())))
{}


json_value_t::json_value_t(json_array_t&& value):
    type_(json_array_type),
    data_(reinterpret_cast<json_pointer_t>(json_new<json_array_t>(move(value), json_allocator())))
{}


json_value_t::json_value_t(json_object_t&& value):
    type_(json_object_type),
    data_(reinterpret_cast<json_pointer_t>(json_new<json_object_t>(move(value), json_allocator())))
{}


json_value_t::~json_value_t()
{
    reset();
}


json_type json_value_t::type() const noexcept
{
    return type_;
}


bool json_value_t::has_null() const noexcept
{
    return type() == json_null_type;
}


bool json_value_t::has_boolean() const noexcept
{
    return type() == json_boolean_type;
}


bool json_value_t::has_number() const noexcept
{
    return type() == json_number_type;
}


bool json_value_t::has_string() const noexcept
{
    return type() == json_string_type;
}


bool json_value_t::has_array() const noexcept
{
    return type() == json_array_type;
}


bool json_value_t::has_object() const noexcept
{
    return type() == json_object_type;
}


json_null_t& json_value_t::get_null()
{
    if (!has_null()) {
        throw runtime_error("Type is not null.");
    }
    return *reinterpret_cast<json_null_t*>(&data_);
}


const json_null_t& json_value_t::get_null() const
{
    if (!has_null()) {
        throw runtime_error("Type is not null.");
    }
    return *reinterpret_cast<const json_null_t*>(&data_);
}


json_boolean_t& json_value_t::get_boolean()
{
    if (!has_boolean()) {
        throw runtime_error("Type is not boolean.");
    }
    return *reinterpret_cast<json_boolean_t*>(data_);
}


const json_boolean_t& json_value_t::get_boolean() const
{
    if (!has_boolean()) {
        throw runtime_error("Type is not boolean.");
    }
    return *reinterpret_cast<const json_boolean_t*>(data_);
}


json_number_t& json_value_t::get_number()
{
    if (!has_number()) {
        throw runtime_error("Type is not a number.");
    }

    return *reinterpret_cast<json_number_t*>(data_);
}


const json_number_t& json_value_t::get_number() const
{
    if (!has_number()) {
        throw runtime_error("Type is not a number.");
    }

    return *reinterpret_cast<const json_number_t*>(data_);
}


json_string_t& json_value_t::get_string()
{
    if (!has_string()) {
        throw runtime_error("Type is not a string.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_string_t*>(data_);
}


const json_string_t& json_value_t::get_string() const
{
    if (!has_string()) {
        throw runtime_error("Type is not a string.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<const json_string_t*>(data_);
}


json_array_t& json_value_t::get_array()
{
    if (!has_array()) {
        throw runtime_error("Type is not an array.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_array_t*>(data_);
}


const json_array_t& json_value_t::get_array() const
{
    if (!has_array()) {
        throw runtime_error("Type is not an array.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<const json_array_t*>(data_);
}


json_object_t& json_value_t::get_object()
{
    if (!has_object()) {
        throw runtime_error("Type is not an object.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<json_object_t*>(data_);
}


const json_object_t& json_value_t::get_object() const
{
    if (!has_object()) {
        throw runtime_error("Type is not an object.");
    }
    if (!data_) {
        throw runtime_error("Value is null.");
    }
    return *reinterpret_cast<const json_object_t*>(data_);
}


void json_value_t::set_null(json_null_t&& value)
{
    reset();
    data_ = (json_pointer_t) nullptr;
    type_ = json_null_type;
}


void json_value_t::set_boolean(json_boolean_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(json_new<json_boolean_t>(move(value)));
    type_ = json_boolean_type;
}


void json_value_t::set_number(json_number_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(json_new<json_number_t>(move(value)));
    type_ = json_number_type;
}


void json_value_t::set_string(json_string_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(json_new<json_string_t>(move(value), json_allocator()));
    type_ = json_string_type;
}


void json_value_t::set_array(json_array_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(json_new<json_array_t>(move(value), json_allocator()));
    type_ = json_array_type;
}


void json_value_t::set_object(json_object_t&& value)
{
    reset();
    data_ = reinterpret_cast<json_pointer_t>(json_new<json_object_t>(move(value), json_allocator()));
    type_ = json_object_type;
}


void json_value_t::set(json_null_t&& value)
{
    set_null(forward<json_null_t>(value));
}


void json_value_t::set(json_boolean_t&& value)
{
    set_boolean(forward<json_boolean_t>(value));
}


void json_value_t::set(json_number_t&& value)
{
    set_number(forward<json_number_t>(value));
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


bool json_value_t::operator==(const json_value_t& rhs) const noexcept
{
    // same underlying type
    if (type() != rhs.type()) {
        return false;
    }

    // same underlying value
    switch (type()) {
        case json_null_type:
            return true;
        case json_boolean_type:
            return *reinterpret_cast<json_boolean_t*>(data_) == *reinterpret_cast<json_boolean_t*>(rhs.data_);
        case json_number_type:
            return *reinterpret_cast<json_number_t*>(data_) == *reinterpret_cast<json_number_t*>(rhs.data_);
        case json_string_type:
            return *reinterpret_cast<json_string_t*>(data_) == *reinterpret_cast<json_string_t*>(rhs.data_);
        case json_array_type:
            return *reinterpret_cast<json_array_t*>(data_) == *reinterpret_cast<json_array_t*>(rhs.data_);
        case json_object_type:
            return *reinterpret_cast<json_object_t*>(data_) == *reinterpret_cast<json_object_t*>(rhs.data_);
        default:
            assert(false && "Unexpected JSON value type.");
            return false;
    }
}


bool json_value_t::operator!=(const json_value_t& rhs) const noexcept
{
    return !operator==(rhs);
}


void json_value_t::swap(json_value_t& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(type_, rhs.type_);
    swap(data_, rhs.data_);
}


void json_value_t::reset()
{
    allocator_type allocator;
    switch (type()) {
        case json_null_type:
            break;
        case json_boolean_type:
            json_delete(reinterpret_cast<json_boolean_t*>(data_));
            break;
        case json_number_type:
            json_delete(reinterpret_cast<json_number_t*>(data_));
            break;
        case json_string_type:
            json_delete(reinterpret_cast<json_string_t*>(data_));
            break;
        case json_array_type:
            json_delete(reinterpret_cast<json_array_t*>(data_));
            break;
        case json_object_type:
            json_delete(reinterpret_cast<json_object_t*>(data_));
            break;
        default:
            assert(false && "Unexpected JSON value type.");
    }

    type_ = json_null_type;
    data_ = (json_pointer_t) nullptr;
}

PYCPP_END_NAMESPACE
