//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for `std::pair`.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
struct is_pair: std::false_type
{};

template <typename T, typename U>
struct is_pair<std::pair<T, U>>: std::true_type
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_pair_v = is_pair<T>::value;

#endif

PYCPP_END_NAMESPACE
