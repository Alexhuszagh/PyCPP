//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core JSON definitions.
 */

#pragma once

#include <cstdint>
#include <string>

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
struct json_array_t;
//struct json_object_t;

// ALIAS
// -----

typedef uint_fast64_t json_pointer_t;
typedef std::nullptr_t json_null_t;
typedef bool json_boolean_t;
typedef double json_number_t;
typedef std::string json_string_t;
