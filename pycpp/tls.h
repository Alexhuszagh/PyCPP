//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-compiler thread-local storage.
 */

#pragma once

#include <compiler.h>

// MACROS
// ------

#if defined(HAVE_CLANG) || defined(HAVE_GCC)
#   define thread_local_storage __thread
#elif defined(HAVE_MSVC)
#   if COMPILER_MAJOR_VERSION >= 14
#       define thread_local_storage thread_local
#   else
#       define thread_local_storage __declspec(thread)
#   endif
#else
#   error Compiler does not support thread-local storage.
#endif
