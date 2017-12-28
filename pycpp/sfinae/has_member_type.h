//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check a type contains a specific member type.
 *
 *  Macro to detect if a class contains a member type of the given
 *  name (`member`), expanding to a struct similar to `bool_constant`.
 *
 *  \synopsis
 *      #define PYCPP_HAS_MEMBER_TYPE(member, name)     implementation-defined
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Check if class has member type.
 *
 *  \warning The conditional typedef is so primitives do not cause a
 *  compilation error.
 */
#define PYCPP_HAS_MEMBER_TYPE(member, name)                                         \
    template <typename T>                                                           \
    class name                                                                      \
    {                                                                               \
    protected:                                                                      \
        struct U {};                                                                \
        using V = conditional_t<is_class<T>::value, T, U>;                          \
                                                                                    \
        struct Fallback                                                             \
        {                                                                           \
            struct member {};                                                       \
        };                                                                          \
                                                                                    \
        struct Derived: V, Fallback                                                 \
        {};                                                                         \
                                                                                    \
        template <typename C> static char &test(typename C::member*);               \
        template <typename C> static long &test(...);                               \
                                                                                    \
    public:                                                                         \
        enum { value = sizeof(test<Derived>(0)) == sizeof(long) };                  \
    }

PYCPP_END_NAMESPACE
