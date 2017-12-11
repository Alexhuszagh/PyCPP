//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `push_back` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(push_back, has_push_back, void (C::*)(typename C::const_reference));


/**
 *  \brief Call `push_back` as a functor.
 */
struct push_back
{
    template <typename T>
    typename std::enable_if<has_push_back<T>::value, void>::type
    operator()(T &t, typename T::const_reference v)
    {
        t.push_back(v);
    }

    template <typename T>
    typename std::enable_if<!has_push_back<T>::value, void>::type
    operator()(T &t, typename T::const_reference v)
    {
        t.insert(t.end(), v);
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_push_back_v = has_push_back<T>::value;

#endif

PYCPP_END_NAMESPACE
