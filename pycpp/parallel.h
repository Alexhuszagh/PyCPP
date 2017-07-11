//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Parallel STL detection and execution macros.
 */

#pragma once

#include <pycpp/compiler.h>
#if defined(HAVE_CPP17)
#   include <execution>
#endif

// MACROS
// ------

#if defined(HAVE_CPP17)
#   define PARALLEL_EXECUTION std::execution::par,
#else
#   define PARALLEL_EXECUTION
#endif
