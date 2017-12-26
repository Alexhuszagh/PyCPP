//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if types support `-` operator.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/sfinae/has_op.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_OPERATION(minus, -);

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T, typename U>
constexpr bool has_minus_v = has_minus<T, U>::value;

#endif

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
