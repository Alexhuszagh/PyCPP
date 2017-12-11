//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Forward declarations for counters.
 */

#pragma once

#include <pycpp/stl/unordered_map.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using count_t = ptrdiff_t;

// FORWARD
// -------

template <
    typename Key,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    typename Alloc = allocator<pair<const Key, count_t>>,
    template <typename, typename, typename, typename, typename> class Map = unordered_map
>
struct counter;

template <
    typename Key,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    typename Alloc = allocator<pair<const Key, count_t>>,
    template <typename, typename, typename, typename, typename> class Map = unordered_map
>
struct threshold_counter;

PYCPP_END_NAMESPACE
