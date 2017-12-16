
//  :copyright: (c) 2011-2012 Andrzej Krzemienski.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Disambiguation tags for any/optional/variant.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <utility>

#if !defined(HAVE_CPP17)             // HAVE_CPP17
#   include <cstdlib>
#else                               // !HAVE_CPP17
#endif                              // HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename ... Ts>
struct tuple;

template <typename T>
struct tuple_size;

template <size_t I, typename T>
struct tuple_element;

// ALIAS
// -----

using std::swap;
using std::forward;
using std::move;
using std::move_if_noexcept;
using std::declval;
using std::make_pair;

template <typename T, typename U>
using pair = std::pair<T, U>;

using piecewise_construct_t = std::piecewise_construct_t;

#if defined(HAVE_CPP17)             // HAVE_CPP17

using in_place = std::in_place;
using in_place_t = std::in_place_t;
template <typename T> using in_place_type_t = std::in_place_type_t<T>;
template <typename T> using in_place_type = std::in_place_type<T>;
template <size_t N> using in_place_index_t = std::in_place_index_t<N>;
template <size_t N> using in_place_index = std::in_place_index<N>;

#else                               // !HAVE_CPP17

struct in_place_t
{
    explicit in_place_t() = default;
};

constexpr in_place_t in_place {};

template <typename T>
struct in_place_type_t
{
    explicit in_place_type_t() = default;
};

template <size_t N>
struct in_place_index_t
{
    explicit in_place_index_t() = default;
};

#if defined(HAVE_CPP14)             // HAVE_CPP14

template <typename T>
constexpr in_place_type_t<T> in_place_type {};

template <size_t N>
constexpr in_place_index_t<N> in_place_index {};

template <typename T, T ... Is>
using integer_sequence = std::integer_sequence<T, Is...>;

template <size_t... Is>
using index_sequence = std::index_sequence<Is...>;

#else                               // !HAVE_CPP14

template <typename T, T... Is>
struct integer_sequence
{
    using value_type = T;
    static constexpr size_t size() noexcept
    {
        return sizeof...(Is);
    }
};

template <size_t... Is>
using index_sequence = integer_sequence<size_t, Is...>;

#endif                              // HAVE_CPP14

#endif                              // HAVE_CPP17

// GLOBALS

constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();

PYCPP_END_NAMESPACE
