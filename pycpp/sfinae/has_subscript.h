//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check if container supports `[]` with subscript.
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename C, typename I, typename = void>
struct has_subscript: false_type
{};

template <typename C, typename I>
struct has_subscript<C, I, void_t<decltype(declval<C>()[declval<I>()])>>: true_type
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename C, typename I>
constexpr bool has_subscript_v = has_subscript<C, I>::value;

#endif

PYCPP_END_NAMESPACE
