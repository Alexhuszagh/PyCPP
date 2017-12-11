//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL map and multimap alias.
 */

#pragma once

#include <pycpp/stl/iterator.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/utility.h>
#include <map>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename Key,
    typename T,
    typename Compare = std::less<Key>,
    typename Alloc = allocator<std::pair<const Key, T>>
>
using map = std::map<Key, T, Compare, Alloc>;

template <
    typename Key,
    typename T,
    typename Compare = std::less<Key>,
    typename Alloc = allocator<std::pair<const Key, T>>
>
using multimap = std::multimap<Key, T, Compare, Alloc>;

PYCPP_END_NAMESPACE
