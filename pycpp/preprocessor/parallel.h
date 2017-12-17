//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Parallel STL detection and execution macros.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#if defined(HAVE_CPP17)
#   include <pycpp/stl/execution.h>
#endif

// MACROS
// ------

#if defined(HAVE_CPP17)
#   define PARALLEL_EXECUTION PYCPP_NAMESPACE::execution::par,
#else
#   define PARALLEL_EXECUTION
#endif
