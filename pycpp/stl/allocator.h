//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Default allocator alias.
 */

#pragma once

#include <pycpp/config.h>
#if USE_POLYMORPHIC_ALLOCATOR
#   include <pycpp/allocator/polymorphic.h>
#else
#   include <memory>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if USE_POLYMORPHIC_ALLOCATOR           // POLYMOPRHIC

template <typename T>
using allocator = polymorphic_allocator<T>;

#else                                   // !POLYMOPRHIC

template <typename T>
using allocator = std::allocator<T>;

#endif                                  // POLYMOPRHIC

PYCPP_END_NAMESPACE
