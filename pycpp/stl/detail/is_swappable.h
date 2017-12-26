//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief C++17 backport of swappable detection.
 *
 *  Backport of `is_swappable_with`, `is_nothrow_swappable_with`,
 *  `is_swappable`, and `is_nothrow_swappable` for C++11.
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

// Swappable (C++17 backport)
template <typename T1, typename T2>
struct is_swappable_with_impl
{
private:
    template <typename U1, typename U2, typename = decltype(swap(std::declval<U1&>(), std::declval<U2&>()))>
    inline static std::true_type test(int);

    template <typename U1, typename U2>
    inline static std::false_type test(...);

public:
    using type = decltype(test<T1, T2>(0));
};

template <typename T1, typename T2>
using is_swappable_with = typename is_swappable_with_impl<T1, T2>::type;

template <typename T1, typename T2, bool = is_swappable_with<T1, T2>::value>
struct is_nothrow_swappable_with
{
    static constexpr bool value = noexcept(swap(std::declval<T1&>(), std::declval<T2&>()));
};

template <typename T1, typename T2>
struct is_nothrow_swappable_with<T1, T2, false>: std::false_type
{};

template <typename T>
using is_swappable = is_swappable_with<T, T>;

template <typename T>
using is_nothrow_swappable = is_nothrow_swappable_with<T, T>;

PYCPP_END_NAMESPACE
