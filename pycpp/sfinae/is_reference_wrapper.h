//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for `reference_wrapper`.
 *
 *  Detect if a type is a `std::reference_wrapper`.
 *
 *  \synopsis
 *      template <typename T
 *      using is_reference_wrapper<T> = implementation-defined;
 */

#pragma once

#include <pycpp/sfinae/is_specialization.h>
#include <pycpp/stl/functional.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
using is_reference_wrapper = is_specialization<T, reference_wrapper>;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

#endif

PYCPP_END_NAMESPACE
