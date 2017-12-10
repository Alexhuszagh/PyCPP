//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <typeindex>.
 */

#pragma once

#include <pycpp/config.h>
#include <functional>
#include <typeindex>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

template <>
struct hash<std::type_index>
{
    using argument_type = std::type_index;
    using result_type = size_t;

    inline size_t operator()(argument_type x) const noexcept
    {
        return std::hash<argument_type>()(x);
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
