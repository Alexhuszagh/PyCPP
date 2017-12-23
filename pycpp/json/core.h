//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core JSON definitions.
 */

#pragma once

#include <pycpp/stl/sstream.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/unordered_map.h>
#include <pycpp/stl/vector.h>
#include <stdint.h>

PYCPP_BEGIN_NAMESPACE

// ENUMS
// -----


/**
 *  \brief Enumerated values for a JSON object type.
 */
enum json_type: uint8_t
{
    json_null_type = 0,
    json_boolean_type,
    json_number_type,
    json_string_type,
    json_array_type,
    json_object_type,
};

// FORWARD
// -------

struct json_value_t;

// ALIAS
// -----

using json_pointer_t = uint_fast64_t;
using json_null_t = nullptr_t;
using json_boolean_t = bool;
using json_number_t = double;
using json_string_t = string;
using json_array_t = vector<json_value_t>;
using json_object_t = unordered_map<json_string_t, json_value_t>;
using json_stringstream_t = basic_stringstream<char, char_traits<char>, allocator<char>>;
using json_istringstream_t = basic_istringstream<char, char_traits<char>, allocator<char>>;
using json_ostringstream_t = basic_ostringstream<char, char_traits<char>, allocator<char>>;

// OBJECTS
// -------


/**
 *  \brief JSON value type.
 *
 *  Store the data in a 64-bit type, storing the data by value for
 *  small types (null, bool, numbers) and by pointer for large
 *  values (string, array, object).
 */
struct json_value_t
{
public:
    // MEMBER TYPES
    // ------------
    using allocator_type = allocator<void>;

    // MEMBER FUNCTIONS
    // ----------------
    json_value_t() noexcept;
    json_value_t(const json_value_t&) = delete;
    json_value_t& operator=(const json_value_t&) = delete;
    json_value_t(json_value_t&&) noexcept;
    json_value_t& operator=(json_value_t&&) noexcept;

    json_value_t(json_type type) noexcept;
    json_value_t(json_null_t&&) noexcept;
    json_value_t(json_boolean_t&&);
    json_value_t(json_number_t&&);
    json_value_t(json_string_t&&);
    json_value_t(json_array_t&&);
    json_value_t(json_object_t&&);
    ~json_value_t();

    // CHECKERS
    json_type type() const noexcept;
    bool has_null() const noexcept;
    bool has_boolean() const noexcept;
    bool has_number() const noexcept;
    bool has_string() const noexcept;
    bool has_array() const noexcept;
    bool has_object() const noexcept;

    // GETTERS
    json_null_t& get_null();
    const json_null_t& get_null() const;
    json_boolean_t& get_boolean();
    const json_boolean_t& get_boolean() const;
    json_number_t& get_number();
    const json_number_t& get_number() const;
    json_string_t& get_string();
    const json_string_t& get_string() const;
    json_array_t& get_array();
    const json_array_t& get_array() const;
    json_object_t& get_object();
    const json_object_t& get_object() const;

    // SETTERS
    void set_null(json_null_t&&);
    void set_boolean(json_boolean_t&&);
    void set_number(json_number_t&&);
    void set_string(json_string_t&&);
    void set_array(json_array_t&&);
    void set_object(json_object_t&&);
    void set(json_null_t&&);
    void set(json_boolean_t&&);
    void set(json_number_t&&);
    void set(json_string_t&&);
    void set(json_array_t&&);
    void set(json_object_t&&);

    // RELATIONAL OPERATORS
    bool operator==(const json_value_t&) const noexcept;
    bool operator!=(const json_value_t&) const noexcept;

    // OTHER
    void swap(json_value_t&) noexcept;

private:
    json_type type_;
    json_pointer_t data_;

    void reset();
};

PYCPP_END_NAMESPACE
