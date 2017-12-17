//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `emplace_back` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(emplace_back, has_emplace_back, void (C::*)(typename C::value_type&&));


/**
 *  \brief Call `emplace_back` as a functor.
 */
struct emplace_back
{
    template <typename T, typename... Ts>
    enable_if_t<has_emplace_back<T>::value, void>
    operator()(T &t, Ts&&... ts)
    {
        t.emplace_back(forward<Ts>(ts)...);
    }

    template <typename T, typename... Ts>
    enable_if_t<!has_emplace_back<T>::value, void>
    operator()(T &t, Ts&&... ts)
    {
        t.insert(t.end(), forward<Ts>(ts)...);
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_emplace_back_v = has_emplace_back<T>::value;

#endif

PYCPP_END_NAMESPACE
