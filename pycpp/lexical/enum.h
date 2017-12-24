//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for enumerated types.
 */

#pragma once

#include <pycpp/lexical/int.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/type_traits.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------

/**
 *  \brief Generic formatter for enumerated values.
 */
struct lexical_enum_formatter: lexical_int_formatter
{
    template <typename Enum> lexical_enum_formatter(Enum value) noexcept;
    lexical_enum_formatter(const lexical_enum_formatter&) noexcept = default;
    lexical_enum_formatter& operator=(const lexical_enum_formatter&) noexcept = default;
    lexical_enum_formatter(lexical_enum_formatter&&) noexcept = delete;
    lexical_enum_formatter& operator=(lexical_enum_formatter&&) noexcept = delete;
};


/**
 *  \brief Generic extractor for enumerated values.
 */
struct lexical_enum_extractor: protected lexical_int_extractor
{
    using lexical_int_extractor::lexical_int_extractor;
    lexical_enum_extractor(const lexical_enum_extractor&) noexcept = default;
    lexical_enum_extractor& operator=(const lexical_enum_extractor&) noexcept = default;
    lexical_enum_extractor(lexical_enum_extractor&&) noexcept = default;
    lexical_enum_extractor& operator=(lexical_enum_extractor&&) noexcept = default;

    template <typename Enum>
    explicit operator Enum() const noexcept;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_enum_formatter>: false_type
{};

template <>
struct is_relocatable<lexical_enum_extractor>: true_type
{};

// DEFINITIONS
// -----------


template <typename Enum>
lexical_enum_formatter::lexical_enum_formatter(Enum value) noexcept:
    lexical_int_formatter(static_cast<underlying_type_t<Enum>>(value))
{
    static_assert(is_enum<Enum>::value, "Type must be an enumeration.");
}


template <typename Enum>
lexical_enum_extractor::operator Enum() const noexcept
{
    static_assert(is_enum<Enum>::value, "Type must be an enumeration.");

    using int_t = underlying_type_t<Enum>;
    return static_cast<Enum>(int_t(*this));
}

PYCPP_END_NAMESPACE
