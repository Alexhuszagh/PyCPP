//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type traits for trivial constructors and assignment operators.
 *
 *  Reasonable default implementations of `is_trivially_*` type traits
 *  for compilers with partial C++11 support.
 *
 *  \synopsis
 *      template <typename T>
 *      using is_trivially_copyable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_default_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_copy_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_move_constructible = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_assignable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_copy_assignable = implementation-defined;
 *
 *      template <typename T>
 *      using is_trivially_move_assignable = implementation-defined;
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// `is_trivially_destructible` is supported by GCC early on in
// the 4.x series, and therefore does not have to be wrapped here.

#if defined(CPP11_PARTIAL_TYPE_TRAITS)      // CPP11_PARTIAL_TYPE_TRAITS

// Guess all trivially constructible/assignable types are
// `is_trivial`.

template <typename T>
using is_trivially_copyable = std::is_trivial<T>;

template <typename T>
using is_trivially_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_default_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_copy_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_move_constructible = std::is_trivial<T>;

template <typename T>
using is_trivially_assignable = std::is_trivial<T>;

template <typename T>
using is_trivially_copy_assignable = std::is_trivial<T>;

template <typename T>
using is_trivially_move_assignable = std::is_trivial<T>;

#else                                       // !CPP11_PARTIAL_TYPE_TRAITS

using std::is_trivially_copyable;
using std::is_trivially_constructible;
using std::is_trivially_default_constructible;
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
using std::is_trivially_assignable;
using std::is_trivially_copy_assignable;
using std::is_trivially_move_assignable;

#endif                                      // CPP11_PARTIAL_TYPE_TRAITS

PYCPP_END_NAMESPACE
