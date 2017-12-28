//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `emplace_front` and fail-safe implementation.
 *
 *  Construct item in-place at the start of the container.
 *
 *  \synopsis
 *      struct emplace_front
 *      {
 *          template <typename T, typename... Ts>
 *          void operator()(T& t, Ts&&... ts);
 *      };
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(emplace_front, has_emplace_front, void (C::*)(typename C::value_type&&));


/**
 *  \brief Call `emplace_front` as a functor.
 */
struct emplace_front
{
    template <typename T, typename... Ts>
    enable_if_t<has_emplace_front<T>::value, void>
    operator()(T &t, Ts&&... ts)
    {
        t.emplace_front(forward<Ts>(ts)...);
    }

    template <typename T, typename... Ts>
    enable_if_t<!has_emplace_front<T>::value, void>
    operator()(T &t, Ts&&... ts)
    {
        t.insert(t.begin(), forward<Ts>(ts)...);
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_emplace_front_v = has_emplace_front<T>::value;

#endif

PYCPP_END_NAMESPACE
