//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL deque alias.
 */

#pragma once

#include <pycpp/stl/memory.h>
#include <deque>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    typename Alloc = allocator<T>
>
using deque = std::deque<T, Alloc>;

PYCPP_END_NAMESPACE
