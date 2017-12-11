//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for character types.
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Generic formatter for character values.
 */
struct lexical_char_formatter
{
public:
    lexical_char_formatter(char c);
    lexical_char_formatter(unsigned char c);

    // DATA
    size_t size() const;
    size_t length() const;
    const char * data() const;
    const char * c_str() const;

    string_view string() const;

    // CONVERSIONS
    explicit operator string_view() const;

protected:
    char buffer_[2];
};


/**
 *  \brief Generic extractor for charcters.
 */
struct lexical_char_extractor
{
public:
    lexical_char_extractor(const string_view &string);

    // CONVERSIONS
    explicit operator char() const;
    explicit operator unsigned char() const;

protected:
    char c;
};


PYCPP_END_NAMESPACE
