//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Default hash alias.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stl/detail/xxhash.h>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T = void>
using equal_to = std::equal_to<T>;

template <typename T = void>
using less = std::less<T>;

template <typename T = void>
using less_equal = std::less_equal<T>;

template <typename T = void>
using greater = std::greater<T>;

template <typename T = void>
using greater_equal = std::greater_equal<T>;

template <typename R, typename ... Ts>
using function = std::function<R, Ts...>;

template <typename T>
using reference_wrapper = std::reference_wrapper<T>;

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
