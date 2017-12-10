//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL vector alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    typename Alloc = allocator<T>
>
using vector = std::vector<T, Alloc>;

PYCPP_END_NAMESPACE
