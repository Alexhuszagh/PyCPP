//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for null types.
 */

#pragma once

#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Generic formatter for NULL values.
 */
struct lexical_null_formatter
{
    lexical_null_formatter();
    lexical_null_formatter(std::nullptr_t nullp);

    // DATA
    size_t size() const;
    size_t length() const;
    const char* data() const;
    const char* c_str() const;

    string_view string() const;

    // CONVERSIONS
    operator string_view() const;
};

/**
 *  \brief Generic extractor for NULL values.
 */
struct lexical_null_extractor
{
    lexical_null_extractor(const string_view& string);

    // CONVERSIONS
    std::nullptr_t value() const;
    operator std::nullptr_t() const;
};

PYCPP_END_NAMESPACE
