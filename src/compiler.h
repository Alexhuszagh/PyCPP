//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Compiler detection macros.
 */

#pragma once

// MSVC
// ----

#if !defined(COMPILER) && defined(_MSC_VER)
#   define HAVE_MSVC 1
#   define COMPILER HAVE_MSVC
#   define COMPILER_PATCH_VERSION 0
#   if _MSC_VER == 1910
#      define HAVE_MSVC_14 1
#      define COMPILER_MAJOR_VERSION 14
#      define COMPILER_MINOR_VERSION 1
#   elif _MSC_VER == 1900
#      define HAVE_MSVC_14 1
#      define COMPILER_MAJOR_VERSION 14
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1800
#      define HAVE_MSVC_12 1
#      define COMPILER_MAJOR_VERSION 12
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1700
#      define HAVE_MSVC_11 1
#      define COMPILER_MAJOR_VERSION 11
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1600
#      define HAVE_MSVC_10 1
#      define COMPILER_MAJOR_VERSION 10
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1500
#      define HAVE_MSVC_9 1
#      define COMPILER_MAJOR_VERSION 9
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1400
#      define HAVE_MSVC_8 1
#      define COMPILER_MAJOR_VERSION 8
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1310
#      define HAVE_MSVC_7 1
#      define COMPILER_MAJOR_VERSION 7
#      define COMPILER_MINOR_VERSION 1
#   elif _MSC_VER == 1300
#      define HAVE_MSVC_7 1
#      define COMPILER_MAJOR_VERSION 7
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1200
#      define HAVE_MSVC_6 1
#      define COMPILER_MAJOR_VERSION 6
#      define COMPILER_MINOR_VERSION 0
#   elif _MSC_VER == 1100
#      define HAVE_MSVC_5 1
#      define COMPILER_MAJOR_VERSION 5
#      define COMPILER_MINOR_VERSION 0
#   endif
#endif

// GCC
// ---

#if !defined(COMPILER) && defined(__GNUC__)
#   define HAVE_GCC 1
#   define COMPILER HAVE_GCC
#   define COMPILER_MAJOR_VERSION __GNUC__
#   define COMPILER_MINOR_VERSION __GNUC_MINOR__
#   define COMPILER_PATCH_VERSION __GNUC_PATCHLEVEL__
#   if COMPILER_MAJOR_VERSION == 7
#      define HAVE_GCC_7 1
#   elif COMPILER_MAJOR_VERSION == 6
#      define HAVE_GCC_6 1
#   elif COMPILER_MAJOR_VERSION == 5
#      define HAVE_GCC_5 1
#   elif COMPILER_MAJOR_VERSION == 4
#      define HAVE_GCC_4 1
#   elif COMPILER_MAJOR_VERSION == 3
#      define HAVE_GCC_3 1
#   endif
#endif

// CLANG
// -----

#if !defined(COMPILER) && defined(__clang__)
#   define HAVE_CLANG 1
#   define COMPILER HAVE_CLANG
#   define COMPILER_MAJOR_VERSION __clang_major__
#   define COMPILER_MINOR_VERSION __clang_minor__
#   define COMPILER_PATCH_VERSION __clang_patchlevel__
#   if COMPILER_MAJOR_VERSION == 4
#      define HAVE_CLANG_4 1
#   elif COMPILER_MAJOR_VERSION == 3
#      define HAVE_CLANG_3 1
#   elif COMPILER_MAJOR_VERSION == 2
#      define HAVE_CLANG_2 1
#   endif
#endif
