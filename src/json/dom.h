//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief JSON DOM parser.
 */

#pragma once

#include <json/sax.h>

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
    json_value_t(json_type type);
    json_value_t(json_null_t);
    json_value_t(json_boolean_t);
    json_value_t(json_number_t);
    json_value_t(json_string_t&&);
    json_value_t(json_array_t&&);
    json_value_t(json_object_t&&);
    ~json_value_t();

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

private:
    json_type type_;
    json_pointer_t data_;

    void reset();
};
