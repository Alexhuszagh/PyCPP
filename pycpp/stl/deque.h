//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL deque alias.
 */

#pragma once

#include <pycpp/config.h>
#if USE_POLYMORPHIC_ALLOCATOR
#   include <pycpp/allocator/polymorphic.h>
#endif
#include <deque>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if USE_POLYMORPHIC_ALLOCATOR           // POLYMOPRHIC

template <
    typename T,
    typename Alloc = polymorphic_allocator<T>
>
using deque = std::deque<T, Alloc>;

#else                                   // !POLYMOPRHIC

template <
    typename T,
    typename Alloc = std::allocator<T>
>
using deque = std::deque<T, Alloc>;

#endif                                  // POLYMOPRHIC

PYCPP_END_NAMESPACE
