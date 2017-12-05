//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Forward declarations for counters.
 */

#pragma once

#include <pycpp/config.h>
#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using count_t = std::ptrdiff_t;

// FORWARD
// -------

template <
    typename Key,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, count_t>>,
    template <typename, typename, typename, typename, typename> class Map = std::unordered_map
>
struct counter;

template <
    typename Key,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, count_t>>,
    template <typename, typename, typename, typename, typename> class Map = std::unordered_map
>
struct threshold_counter;

PYCPP_END_NAMESPACE
