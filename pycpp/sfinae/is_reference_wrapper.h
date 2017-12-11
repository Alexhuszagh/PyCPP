//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for `std::reference_wrapper`.
 */

#pragma once

#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
struct is_reference_wrapper: std::false_type
{};

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>>: std::true_type
{};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

#endif

PYCPP_END_NAMESPACE
