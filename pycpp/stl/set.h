//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL set and multiset alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#include <set>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    typename Compare = std::less<T>,
    typename Alloc = allocator<T>
>
using set = std::set<T, Compare, Alloc>;

template <
    typename T,
    typename Compare = std::less<T>,
    typename Alloc = allocator<T>
>
using multiset = std::multiset<T, Compare, Alloc>;

PYCPP_END_NAMESPACE
