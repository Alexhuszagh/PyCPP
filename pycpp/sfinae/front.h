//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `front` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/back.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(front, has_front, typename C::reference (C::*)());


/**
 *  \brief Call `front` as a functor.
 */
struct front
{
    template <typename T>
    typename std::enable_if<has_front<T>::value && !back_detail::is_const<T>::value, typename T::reference>::type
    operator()(T &t)
    {
        return t.front();
    }

    template <typename T>
    typename std::enable_if<!has_front<T>::value && !back_detail::is_const<T>::value, typename T::reference>::type
    operator()(T &t)
    {
        return *t.begin();
    }

    template <typename T>
    typename std::enable_if<has_front<T>::value, typename T::const_reference>::type
    operator()(const T &t) const
    {
        return t.front();
    }

    template <typename T>
    typename std::enable_if<!has_front<T>::value, typename T::const_reference>::type
    operator()(const T &t) const
    {
        return *t.begin();
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_front_v = has_front<T>::value;

#endif

PYCPP_END_NAMESPACE
