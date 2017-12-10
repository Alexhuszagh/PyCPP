//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL unordered_map and unordered_multimap alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#include <pycpp/stl/hash.h>
#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename Key,
    typename T,
    typename Hash = hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = allocator<std::pair<const Key, T>>
>
using unordered_map = std::unordered_map<Key, T, Hash, Pred, Alloc>;

template <
    typename Key,
    typename T,
    typename Hash = hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = allocator<std::pair<const Key, T>>
>
using unordered_multimap = std::unordered_multimap<Key, T, Hash, Pred, Alloc>;

PYCPP_END_NAMESPACE
