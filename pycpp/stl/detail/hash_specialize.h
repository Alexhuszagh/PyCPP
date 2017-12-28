//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lightweight include to specialize hash functions.
 *
 *  Lightweight wrapper to specialize a custom hash function in terms
 *  of `std::hash`.
 *
 *  \synopsis
 *      #define PYCPP_SPECIALIZE_HASH_VALUE(name, type)     implementation-defined
 *      #define PYCPP_SPECIALIZE_HASH_REFERENCE(name, type) implementation-defined
 */

#pragma once

#include <pycpp/config.h>
#include <functional>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  Specialize primitive types by value using std::hash by default.
 */
#define PYCPP_SPECIALIZE_HASH_VALUE(name, type)                         \
    template <typename T> struct name;                                  \
                                                                        \
    template <>                                                         \
    struct name<type>                                                   \
    {                                                                   \
        using argument_type = type;                                     \
        using result_type = size_t;                                     \
                                                                        \
        inline size_t operator()(type x) const noexcept                 \
        {                                                               \
            return std::hash<type>()(x);                                \
        }                                                               \
    }

/**
 *  Specialize classes by const reference using std::hash by default.
 */
#define PYCPP_SPECIALIZE_HASH_REFERENCE(name, type)                     \
    template <typename T> struct name;                                  \
                                                                        \
    template <>                                                         \
    struct name<type>                                                   \
    {                                                                   \
        using argument_type = type;                                     \
        using result_type = size_t;                                     \
                                                                        \
        inline size_t operator()(const argument_type& x) const noexcept \
        {                                                               \
            return std::hash<argument_type>()(x);                       \
        }                                                               \
    }

PYCPP_END_NAMESPACE
