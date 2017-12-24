//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for null types.
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic formatter for NULL values.
 */
struct lexical_null_formatter
{
    lexical_null_formatter() noexcept = default;
    lexical_null_formatter(nullptr_t nullp) noexcept;
    lexical_null_formatter(const lexical_null_formatter&) noexcept = default;
    lexical_null_formatter& operator=(const lexical_null_formatter&) noexcept = default;
    lexical_null_formatter(lexical_null_formatter&&) noexcept = default;
    lexical_null_formatter& operator=(lexical_null_formatter&&) noexcept = default;

    // DATA
    size_t size() const noexcept;
    size_t length() const noexcept;
    const char* data() const noexcept;
    const char* c_str() const noexcept;

    string_view string() const noexcept;

    // CONVERSIONS
    operator string_view() const noexcept;
};

/**
 *  \brief Generic extractor for NULL values.
 */
struct lexical_null_extractor
{
    lexical_null_extractor(const string_view& string);
    lexical_null_extractor(const lexical_null_extractor&) noexcept = default;
    lexical_null_extractor& operator=(const lexical_null_extractor&) noexcept = default;
    lexical_null_extractor(lexical_null_extractor&&) noexcept = default;
    lexical_null_extractor& operator=(lexical_null_extractor&&) noexcept = default;

    // CONVERSIONS
    nullptr_t value() const noexcept;
    operator nullptr_t() const noexcept;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_null_formatter>: true_type
{};

template <>
struct is_relocatable<lexical_null_extractor>: true_type
{};

PYCPP_END_NAMESPACE
