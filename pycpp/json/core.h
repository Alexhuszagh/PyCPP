//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core JSON definitions.
 */

#pragma once

#include <pycpp/config.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

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

typedef uint_fast64_t json_pointer_t;
typedef std::nullptr_t json_null_t;
typedef bool json_boolean_t;
typedef double json_number_t;
typedef std::string json_string_t;
typedef std::vector<json_value_t> json_array_t;
typedef std::unordered_map<json_string_t, json_value_t> json_object_t;


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
    json_value_t();
    json_value_t(json_value_t&&);
    json_value_t & operator=(json_value_t&&);

    json_value_t(json_type type);
    json_value_t(json_null_t);
    json_value_t(json_boolean_t);
    json_value_t(json_number_t);
    json_value_t(json_string_t&&);
    json_value_t(json_array_t&&);
    json_value_t(json_object_t&&);
    ~json_value_t();

    void swap(json_value_t&);

    // CHECKERS
    json_type type() const;
    bool has_null() const;
    bool has_boolean() const;
    bool has_number() const;
    bool has_string() const;
    bool has_array() const;
    bool has_object() const;

    // GETTERS
    json_null_t get_null() const;
    json_boolean_t get_boolean() const;
    json_number_t get_number() const;
    json_string_t& get_string() const;
    json_array_t& get_array() const;
    json_object_t& get_object() const;

    // SETTERS
    void set_null(json_null_t);
    void set_boolean(json_boolean_t);
    void set_number(json_number_t);
    void set_string(json_string_t&&);
    void set_array(json_array_t&&);
    void set_object(json_object_t&&);
    void set(json_null_t);
    void set(json_boolean_t);
    void set(json_number_t);
    void set(json_string_t&&);
    void set(json_array_t&&);
    void set(json_object_t&&);

    // CONVERSIONS
    operator json_boolean_t() const;
    operator json_number_t() const;
    operator json_string_t&() const;
    operator json_array_t&() const;
    operator json_object_t&() const;

private:
    json_type type_;
    json_pointer_t data_;

    void reset();
};

PYCPP_END_NAMESPACE
