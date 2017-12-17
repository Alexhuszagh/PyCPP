//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Default hash alias.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/detail/xxhash.h>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::bind;
using std::ref;
using std::cref;

#if defined(HAVE_CPP17)

using std::invoke;

#endif          // HAVE_CPP17

using std::equal_to;
using std::less;
using std::less_equal;
using std::greater;
using std::greater_equal;
using std::function;
using std::reference_wrapper;

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
