//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL vector alias.
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <functional>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    typename Alloc = allocator<T>
>
using vector = std::vector<T, Alloc>;

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

// vector<bool> uses a very fast hash implementation by default
// that iterates over each bit. Let `std::hash` take care of it.
template <typename Allocator>
struct hash<std::vector<bool, Allocator>>
{
    using argument_type = std::vector<bool, Allocator>;
    using result_type = size_t;

    inline size_t operator()(const argument_type& x) const noexcept
    {
        return std::hash<argument_type>()(x);
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
