//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Forward declarations for the N-dimensional array
 */

#pragma once

#include <pycpp/view/vector.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename T, typename Allocator = std::allocator<T>>
class ndarray;

template <typename T, size_t N, typename Allocator = std::allocator<T>>
class variadic_ndarray;

template <typename T>
class ndarray_view;

template <typename T>
class ndarray_iterator;

// ALIAS
// -----

using ndarray_dimensions = std::vector<size_t>;
using ndarray_dimension_view = vector_view<size_t>;

PYCPP_END_NAMESPACE
