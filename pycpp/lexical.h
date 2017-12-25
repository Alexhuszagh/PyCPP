//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical cast routines.
 */

#pragma once

#include <pycpp/lexical/bool.h>
#include <pycpp/lexical/char.h>
#include <pycpp/lexical/enum.h>
#include <pycpp/lexical/float.h>
#include <pycpp/lexical/format.h>
#include <pycpp/lexical/int.h>
#include <pycpp/lexical/null.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T, typename U>
using lexical_is_same = is_same<remove_cv_t<T>, U>;

// MACROS
// ------

/**
 *  \brief Define a lexical formatter.
 */
#define PYCPP_LEXICAL_FORMATTER(type_, formatter)                                   \
    template <typename T>                                                           \
    enable_if_t<lexical_is_same<T, type_>::value, string>                           \
    lexical(const T& value)                                                         \
    {                                                                               \
        return string(formatter(value).str());                                      \
    }                                                                               \
                                                                                    \
    template <typename T>                                                           \
    enable_if_t<lexical_is_same<T, type_>::value, string>                           \
    format(const T& value)                                                          \
    {                                                                               \
        return string(formatter(value).str());                                      \
    }

/**
 *  \brief Define a lexical extractor.
 */
#define PYCPP_LEXICAL_EXTRACTOR(type_, extractor)                                   \
    template <typename T>                                                           \
    enable_if_t<lexical_is_same<T, type_>::value, T>                                \
    lexical(const string_view& value)                                               \
    {                                                                               \
        return (type_)(extractor(value));                                           \
    }                                                                               \
                                                                                    \
    template <typename T>                                                           \
    enable_if_t<lexical_is_same<T, type_>::value, T>                                \
    extract(const string_view& value)                                               \
    {                                                                               \
        return (type_)(extractor(value));                                           \
    }

// FUNCTIONS
// ---------

// FORMATTERS
// Formatters are provided with overloads for `format()` and
// `lexical()`, of which the latter is an alias of the former.

PYCPP_LEXICAL_FORMATTER(nullptr_t, lexical_null_formatter)
PYCPP_LEXICAL_FORMATTER(bool, lexical_bool_formatter)
PYCPP_LEXICAL_FORMATTER(char, lexical_char_formatter)
PYCPP_LEXICAL_FORMATTER(unsigned char, lexical_char_formatter)
PYCPP_LEXICAL_FORMATTER(short, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(unsigned short, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(int, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(unsigned int, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(long, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(unsigned long, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(long long, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(unsigned long long, lexical_int_formatter)
PYCPP_LEXICAL_FORMATTER(float, lexical_float_formatter)
PYCPP_LEXICAL_FORMATTER(double, lexical_float_formatter)

template <typename T>
enable_if_t<is_enum<T>::value, string>
lexical(const T& value)
{
    return string(lexical_enum_formatter(value).str());
}

template <typename T>
enable_if_t<is_enum<T>::value, string>
format(const T& value)
{
    return string(lexical_enum_formatter(value).str());
}

// EXTRACTORS
// Cannot use functional-style casts with `nullptr_t` on
// MSVC 2015 (and likely later), and therefore the method `value()`
// is provided to avoid the cast.

template <typename T>
enable_if_t<lexical_is_same<T, nullptr_t>::value, T>
lexical(const string_view& value)
{
    return lexical_null_extractor(value).value();
}

template <typename T>
enable_if_t<lexical_is_same<T, nullptr_t>::value, string>
extract(const string_view& value)
{
    return lexical_null_extractor(value).value();
}

PYCPP_LEXICAL_EXTRACTOR(bool, lexical_bool_extractor)
PYCPP_LEXICAL_EXTRACTOR(char, lexical_char_extractor)
PYCPP_LEXICAL_EXTRACTOR(unsigned char, lexical_char_extractor)
PYCPP_LEXICAL_EXTRACTOR(short, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(unsigned short, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(int, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(unsigned int, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(long, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(unsigned long, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(long long, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(unsigned long long, lexical_int_extractor)
PYCPP_LEXICAL_EXTRACTOR(float, lexical_float_extractor)
PYCPP_LEXICAL_EXTRACTOR(double, lexical_float_extractor)


template <typename T>
enable_if_t<is_enum<T>::value, T>
lexical(const string_view& value)
{
    return T(lexical_enum_extractor(value));
}

template <typename T>
enable_if_t<is_enum<T>::value, T>
extract(const string_view& value)
{
    return T(lexical_enum_extractor(value));
}

// CLEAN UP
// --------

# undef PYCPP_LEXICAL_FORMATTER
# undef PYCPP_LEXICAL_EXTRACTOR

PYCPP_END_NAMESPACE
