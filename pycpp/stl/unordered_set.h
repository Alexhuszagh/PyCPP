//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL unordered_set and unordered_multiset alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#include <pycpp/stl/hash.h>
#include <unordered_set>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    typename Hash = hash<T>,
    typename Pred = std::equal_to<T>,
    typename Alloc = allocator<T>
>
using unordered_set = std::unordered_set<T, Hash, Pred, Alloc>;

template <
    typename T,
    typename Hash = hash<T>,
    typename Pred = std::equal_to<T>,
    typename Alloc = allocator<T>
>
using unordered_multiset = std::unordered_multiset<T, Hash, Pred, Alloc>;

PYCPP_END_NAMESPACE
