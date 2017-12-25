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
    lexical_char_formatter(char c) noexcept;
    lexical_char_formatter(unsigned char c) noexcept;
    lexical_char_formatter(const lexical_char_formatter&) noexcept = default;
    lexical_char_formatter& operator=(const lexical_char_formatter&) noexcept = default;
    lexical_char_formatter(lexical_char_formatter&&) noexcept = default;
    lexical_char_formatter& operator=(lexical_char_formatter&&) noexcept = default;

    // DATA
    size_t size() const noexcept;
    size_t length() const noexcept;
    const char * data() const noexcept;
    const char * c_str() const noexcept;

    string_view str() const noexcept;

    // CONVERSIONS
    explicit operator string_view() const noexcept;

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
    lexical_char_extractor(const lexical_char_extractor&) noexcept = default;
    lexical_char_extractor& operator=(const lexical_char_extractor&) noexcept = default;
    lexical_char_extractor(lexical_char_extractor&&) noexcept = default;
    lexical_char_extractor& operator=(lexical_char_extractor&&) noexcept = default;

    // CONVERSIONS
    explicit operator char() const noexcept;
    explicit operator unsigned char() const noexcept;

protected:
    char c;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_char_formatter>: true_type
{};

template <>
struct is_relocatable<lexical_char_extractor>: true_type
{};

PYCPP_END_NAMESPACE
