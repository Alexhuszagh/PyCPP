//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL array alias.
 */

#pragma once

#include <pycpp/stl/iterator.h>
#include <array>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename T,
    size_t N
>
using array = std::array<T, N>;

PYCPP_END_NAMESPACE
