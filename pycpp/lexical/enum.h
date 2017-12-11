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
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------

/**
 *  \brief Generic formatter for enumerated values.
 */
struct lexical_enum_formatter: lexical_int_formatter
{
    template <typename Enum>
    lexical_enum_formatter(Enum value);
};


/**
 *  \brief Generic extractor for enumerated values.
 */
struct lexical_enum_extractor: protected lexical_int_extractor
{
    using lexical_int_extractor::lexical_int_extractor;

    template <typename Enum>
    explicit operator Enum() const;
};

// DEFINITIONS
// -----------


template <typename Enum>
lexical_enum_formatter::lexical_enum_formatter(const Enum value):
    lexical_int_formatter(static_cast<typename underlying_type<Enum>::type>(value))
{
    static_assert(is_enum<Enum>::value, "Type must be an enumeration.");
}


template <typename Enum>
lexical_enum_extractor::operator Enum() const
{
    static_assert(is_enum<Enum>::value, "Type must be an enumeration.");

    using int_t = typename underlying_type<Enum>::type;
    return static_cast<Enum>(int_t(*this));
}

PYCPP_END_NAMESPACE
