//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-compiler thread-local storage.
 *
 *  Keyword-like macro for thread-local storage duration specifiers,
 *  similar to the C++11 keyword `thread_local`. Variables declared
 *  with thread-local storage will have a separate instance in each
 *  thread, at the cost of slower data access.
 *
 *  \code
 *      thread_local_storage int x = 5;
 *
 *  \synopsis
 *      #define thread_local_storage implementation-defined
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>

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
#   error "Compiler does not support thread-local storage."
#endif
