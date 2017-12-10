//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper of the fast xxHash algorithm as an STL hash function.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/architecture.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define XXH_NAMESPACE pycpp
#if SYSTEM_ARCHITECTURE <= 32
#   define XXH_NO_LONG_LONG
#endif

// TODO: need a global state....
// TODO: here...




PYCPP_END_NAMESPACE
