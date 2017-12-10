//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Default hash alias.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/hashlib/xxhash.h>
#if !defined(USE_XXHASH)
#   include <functional>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if USE_XXHASH                          // XXHASH

template <typename Key>
struct hash: xxhash<Key>
{};

#else                                   // !XXHASH

template <typename Key>
struct hash: std::hash<Key>
{};

#endif                                  // XXHASH

PYCPP_END_NAMESPACE
