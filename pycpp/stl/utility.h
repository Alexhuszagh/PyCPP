
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
using std::pair;
using std::piecewise_construct_t;
using std::piecewise_construct;

#if defined(HAVE_CPP17)             // HAVE_CPP17

using std::in_place;
using std::in_place_t;
using std::in_place_type_t;
using std::in_place_type;
using std::in_place_index_t;
using std::in_place_index;

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

using std::integer_sequence;
using std::index_sequence;

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

PYCPP_END_NAMESPACE
