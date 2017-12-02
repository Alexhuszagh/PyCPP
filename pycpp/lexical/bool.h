//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for boolean types.
 */

#pragma once

#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic formatter for boolean values.
 */
struct lexical_bool_formatter
{
public:
    lexical_bool_formatter();
    lexical_bool_formatter(bool value);

    // DATA
    size_t size() const;
    size_t length() const;
    const char* data() const;
    const char* c_str() const;

    string_view string() const;

    // CONVERSIONS
    operator string_view() const;

private:
    std::string* string_;
};


/**
 *  \brief Generic extractor for boolean values.
 */
struct lexical_bool_extractor
{
public:
    lexical_bool_extractor(const string_view& string);

    // CONVERSIONS
    bool value() const;
    operator bool() const;

private:
    bool data_;
};

PYCPP_END_NAMESPACE
