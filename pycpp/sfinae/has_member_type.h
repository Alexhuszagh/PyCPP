//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check a type contains a specific member type.
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>
#include <utility>

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
        typedef typename std::conditional<std::is_class<T>::value, T, U>::type V;   \
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
