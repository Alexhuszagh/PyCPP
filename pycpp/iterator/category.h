//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator utilities.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// TYPE

template <typename T>
using is_input_iterator = is_same<typename iterator_traits<T>::iterator_category, input_iterator_tag>;

template <typename T>
using is_output_iterator = is_same<typename iterator_traits<T>::iterator_category, output_iterator_tag>;

template <typename T>
using is_forward_iterator = is_same<typename iterator_traits<T>::iterator_category, forward_iterator_tag>;

template <typename T>
using is_bidirectional_iterator = is_same<typename iterator_traits<T>::iterator_category, bidirectional_iterator_tag>;

template <typename T>
using is_random_access_iterator = is_same<typename iterator_traits<T>::iterator_category, random_access_iterator_tag>;

// CONCEPTS

template <typename T>
using is_input_iterable = is_base_of<input_iterator_tag, typename iterator_traits<T>::iterator_category>;

template <typename T>
using is_output_iterable = is_base_of<output_iterator_tag, typename iterator_traits<T>::iterator_category>;

template <typename T>
using is_forward_iterable = is_base_of<forward_iterator_tag, typename iterator_traits<T>::iterator_category>;

template <typename T>
using is_bidirectional_iterable = is_base_of<bidirectional_iterator_tag, typename iterator_traits<T>::iterator_category>;

template <typename T>
using is_random_access_iterable = is_base_of<random_access_iterator_tag, typename iterator_traits<T>::iterator_category>;

#ifdef HAVE_CPP14

// SFINAE
// ------

// TYPE

template <typename T>
constexpr bool is_input_iterator_v = is_input_iterator<T>::value;

template <typename T>
constexpr bool is_output_iterator_v = is_output_iterator<T>::value;

template <typename T>
constexpr bool is_forward_iterator_v = is_forward_iterator<T>::value;

template <typename T>
constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;

template <typename T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;

// CONCEPTS

template <typename T>
constexpr bool is_input_iterable_v = is_input_iterable<T>::value;

template <typename T>
constexpr bool is_output_iterable_v = is_output_iterable<T>::value;

template <typename T>
constexpr bool is_forward_iterable_v = is_forward_iterable<T>::value;

template <typename T>
constexpr bool is_bidirectional_iterable_v = is_bidirectional_iterable<T>::value;

template <typename T>
constexpr bool is_random_access_iterable_v = is_random_access_iterable<T>::value;

#endif

PYCPP_END_NAMESPACE
