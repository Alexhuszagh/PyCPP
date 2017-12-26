//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Compiler-specific traits.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>

// CONSTEXPR
// ---------

// CPP14 allows non-const constexpr variables.
#if defined(HAVE_CPP14)     // HAVE_CPP14
#   define CPP14_CONSTEXPR constexpr
#else                       // !HAVE_CPP14
#   define CPP14_CONSTEXPR
#endif                      // HAVE_CPP14

// CPP17 constexpr implies inline.
#if defined(HAVE_CPP17)     // HAVE_CPP17
#   define CPP17_CONSTEXPR constexpr
#else                       // !HAVE_CPP17
#   define CPP17_CONSTEXPR
#endif                      // HAVE_CPP17

// INLINE
// ------

// CPP17 allows inline variables.
#if defined(HAVE_CPP17)     // HAVE_CPP17
#   define CPP17_INLINE_VARIABLES inline
#else                       // !HAVE_CPP17
#   define CPP17_INLINE_VARIABLES
#endif                      // HAVE_CPP17

// ATTRIBUTES
// ----------

// CPP14+ attributes.
#if defined(HAVE_CPP14)                         // HAVE_CPP14
#   define CPP14_DEPRECATED [[deprecated]]
#elif defined(HAVE_GCC) || defined(HAVE_CLANG)  // HAVE_GCC | HAVE_CLANG
#   define CPP14_DEPRECATED __attribute__((deprecated))
#elif defined(HAVE_MSVC)                        // HAVE_MSVC
#   define CPP14_DEPRECATED __declspec(deprecated)
#else                                           // !HAVE_CPP14
#   define CPP14_DEPRECATED
#endif                                          // HAVE_CPP14

// CPP17+ attributes.
#if defined(HAVE_CPP17)     // HAVE_CPP17
#   define CPP17_FALLTHROUGH [[fallthrough]]
#   define CPP17_NODISCARD [[nodiscard]]
#   define CPP17_UNUSED [[maybe_unused]]
#elif defined(HAVE_GCC)     // HAVE_GCC
#   define CPP17_FALLTHROUGH __attribute__((fallthrough))
#   define CPP17_NODISCARD __attribute__((warn_unused_result))
#   define CPP17_UNUSED __attribute__((unused))
#elif defined(HAVE_CLANG)   // HAVE_CLANG
#   define CPP17_FALLTHROUGH [[clang::fallthrough]]
#   define CPP17_NODISCARD __attribute__((warn_unused_result))
#   define CPP17_UNUSED __attribute__((unused))
#elif defined(HAVE_MSVC)    // HAVE_MSVC
#   define CPP17_FALLTHROUGH (void)0
#   define CPP17_NODISCARD _Check_return_
#   define CPP17_UNUSED
#else                       // !HAVE_CPP17
#   define CPP17_FALLTHROUGH (void)0
#   define CPP17_NODISCARD
#   define CPP17_UNUSED
#endif                      // HAVE_CPP17

// COMPATABILITY
// -------------

// Define compatability for compilers with partial
// C++11 support. Most notably, this includes GCC 5.4,
// TODO: add here...
#define CPP11_PARTIAL_COMPATIBILITY 1
