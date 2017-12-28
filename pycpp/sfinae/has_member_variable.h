//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check a type contains a specific member variable.
 *
 *  Macro to detect if a class contains a member variable of the given
 *  name (`member`), expanding to a struct inhering from `bool_constant`.
 *
 *  \synopsis
 *      #define PYCPP_HAS_MEMBER_VARIABLE(member, name)     implementation-defined
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Check if class has member variable.
 */
#define PYCPP_HAS_MEMBER_VARIABLE(member, name)                 \
    template <typename T, typename = int>                       \
    struct name: false_type                                     \
    {};                                                         \
                                                                \
    template <typename T>                                       \
    struct name <T, decltype((void) T::member, 0)>: true_type   \
    {}


PYCPP_END_NAMESPACE
