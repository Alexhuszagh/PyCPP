//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator utilities.
 */

#pragma once

#include <pycpp/config.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T>
using is_input_iterator = std::is_same<typename std::iterator_traits<T>::iterator_category, std::input_iterator_tag>;

template <typename T>
using is_output_iterator = std::is_same<typename std::iterator_traits<T>::iterator_category, std::output_iterator_tag>;

template <typename T>
using is_forward_iterator = std::is_same<typename std::iterator_traits<T>::iterator_category, std::forward_iterator_tag>;

template <typename T>
using is_bidirectional_iterator = std::is_same<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>;

template <typename T>
using is_random_access_iterator = std::is_same<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>;

PYCPP_END_NAMESPACE
