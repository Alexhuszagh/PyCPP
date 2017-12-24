//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for floating-point types.
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int MAX_FLOAT_SIZE = 60;

// DECLARATIONS
// ------------

/**
 *  \brief Generic formatter for floating-point values.
 */
struct lexical_float_formatter
{
public:
    lexical_float_formatter(float value) noexcept;
    lexical_float_formatter(double value) noexcept;
    lexical_float_formatter(const lexical_float_formatter&) noexcept;
    lexical_float_formatter& operator=(const lexical_float_formatter&) noexcept;
    lexical_float_formatter(lexical_float_formatter&&) noexcept = delete;
    lexical_float_formatter& operator=(lexical_float_formatter&&) noexcept = delete;

    // DATA
    size_t size() const noexcept;
    size_t length() const noexcept;
    const char* data() const noexcept;
    const char* c_str() const noexcept;

    string_view string() const noexcept;

    // CONVERSIONS
    explicit operator string_view() const noexcept;

private:
    char buffer_[MAX_FLOAT_SIZE];
    char* last_ = buffer_ + MAX_FLOAT_SIZE;
};


/**
 *  \brief Generic extractor for floating-point values.
 */
class lexical_float_extractor
{
public:
    lexical_float_extractor(const string_view& string) noexcept;
    lexical_float_extractor(const lexical_float_extractor&) noexcept = default;
    lexical_float_extractor& operator=(const lexical_float_extractor&) noexcept = default;
    lexical_float_extractor(lexical_float_extractor&&) noexcept = default;
    lexical_float_extractor& operator=(lexical_float_extractor&&) noexcept = default;

    // CONVERSIONS
    explicit operator float() const noexcept;
    explicit operator double() const noexcept;

private:
    double data_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_float_formatter>: false_type
{};

template <>
struct is_relocatable<lexical_float_extractor>: true_type
{};

PYCPP_END_NAMESPACE
