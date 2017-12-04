//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `push_front` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(push_front, has_push_front, void (C::*)(typename C::const_reference));


/**
 *  \brief Call `push_front` as a functor.
 */
struct push_front
{
    template <typename T>
    typename std::enable_if<has_push_front<T>::value, void>::type
    operator()(T &t, typename T::const_reference v)
    {
        t.push_front(v);
    }

    template <typename T>
    typename std::enable_if<!has_push_front<T>::value, void>::type
    operator()(T &t, typename T::const_reference v)
    {
        t.insert(t.begin(), v);
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_push_front_v = has_push_front<T>::value;

#endif

PYCPP_END_NAMESPACE
