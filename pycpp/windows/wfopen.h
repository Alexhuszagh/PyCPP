//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if `_wfopen` is defined.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>

// MACROS
// ------

#if defined(HAVE_GCC)
#   include <stdio.h>
#   if defined(_wfopen)
#       define PYCPP_HAVE_WFOPEN 1
#   endif
#elif defined(HAVE_MSVC)
#   define PYCPP_HAVE_WFOPEN 1
#endif