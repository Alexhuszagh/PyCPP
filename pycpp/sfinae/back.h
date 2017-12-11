//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `back` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

namespace back_detail
{
// SFINAE
// ------

template <typename T>
using dereference_iterator_type = decltype(*std::declval<typename T::iterator>());

template <typename T>
using is_const_iterator = std::is_same<dereference_iterator_type<T>, typename T::const_reference>;

template <typename T>
struct is_const: std::integral_constant<
        bool,
        std::is_const<T>::value || is_const_iterator<T>::value
    >
{};

}   /* back_detail */

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(back, has_back, typename C::reference (C::*)());


/**
 *  \brief Call `back` as a functor.
 *
 *  Must support bidirectional iteration.
 */
struct back
{
    template <typename T>
    typename std::enable_if<has_back<T>::value && !back_detail::is_const<T>::value, typename T::reference>::type
    operator()(T &t)
    {
        return t.back();
    }

    template <typename T>
    typename std::enable_if<!has_back<T>::value && !back_detail::is_const<T>::value, typename T::reference>::type
    operator()(T &t)
    {
        return *t.rbegin();
    }

    template <typename T>
    typename std::enable_if<has_back<T>::value, typename T::const_reference>::type
    operator()(const T &t) const
    {
        return t.back();
    }

    template <typename T>
    typename std::enable_if<!has_back<T>::value, typename T::const_reference>::type
    operator()(const T &t) const
    {
        return *t.rbegin();
    }
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_back_v = has_back<T>::value;

#endif

PYCPP_END_NAMESPACE
