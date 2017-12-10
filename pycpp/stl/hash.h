//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Default hash alias.
 */

#pragma once

#include <pycpp/config.h>
// TODO: when xxhash is implemented, use it.
// Right now, just use std::hash
#undef USE_XXHASH
#if USE_XXHASH
#   include <pycpp/hashlib/xxhash.h>
#else
#   include <functional>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if USE_XXHASH                          // XXHASH

template <typename Key>
using hash = xxhash<Key>;

#else                                   // !XXHASH

template <typename Key>
using hash = std::hash<Key>;

#endif                                  // XXHASH

PYCPP_END_NAMESPACE
