//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for boolean types.
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic formatter for boolean values.
 */
struct lexical_bool_formatter
{
public:
    lexical_bool_formatter() noexcept;
    lexical_bool_formatter(bool value) noexcept;
    lexical_bool_formatter(const lexical_bool_formatter&) noexcept = default;
    lexical_bool_formatter& operator=(const lexical_bool_formatter&) noexcept = default;
    lexical_bool_formatter(lexical_bool_formatter&&) noexcept = default;
    lexical_bool_formatter& operator=(lexical_bool_formatter&&) noexcept = default;

    // DATA
    size_t size() const noexcept;
    size_t length() const noexcept;
    const char* data() const noexcept;
    const char* c_str() const noexcept;

    string_view string() const noexcept;

    // CONVERSIONS
    operator string_view() const noexcept;

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
    lexical_bool_extractor(const lexical_bool_extractor&) noexcept = default;
    lexical_bool_extractor& operator=(const lexical_bool_extractor&) noexcept = default;
    lexical_bool_extractor(lexical_bool_extractor&&) noexcept = default;
    lexical_bool_extractor& operator=(lexical_bool_extractor&&) noexcept = default;

    // CONVERSIONS
    bool value() const noexcept;
    operator bool() const noexcept;

private:
    bool data_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_bool_formatter>: true_type
{};

template <>
struct is_relocatable<lexical_bool_extractor>: true_type
{};

PYCPP_END_NAMESPACE
