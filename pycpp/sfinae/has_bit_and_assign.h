//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if types support `&=` operator.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/sfinae/has_op.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_OPERATION(bit_and_assign, &=);

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T, typename U>
constexpr bool has_bit_and_assign_v = has_bit_and_assign<T, U>::value;

#endif

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
