//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <tuple> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <tuple>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, typename Allocator>
struct uses_allocator;

// ALIAS
// -----

using std::make_tuple;
using std::tie;
using std::forward_as_tuple;
using std::tuple_cat;
using std::get;
using std::swap;

// Inherit publicly from partial specializations
template <typename T>
struct tuple_size: std::tuple_size<T>
{};

template <size_t I, typename T>
struct tuple_element: std::tuple_element<I, T>
{};

template <typename ... Ts, typename Allocator>
struct uses_allocator<std::tuple<Ts...>, Allocator>: std::uses_allocator<Ts..., Allocator>
{};

template <typename ... Ts>
struct tuple: std::tuple<Ts...>
{
    using std::tuple<Ts...>::tuple;
};

// CONVENIENCE
// -----------

template <size_t I, typename T>
using tuple_element_t = typename tuple_element<I, T>::type;

PYCPP_END_NAMESPACE
