//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core helpers for reference-based STL containers.
 */

#pragma once

#include <pycpp/iterator/transform.h>
#include <functional>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// DECLARATION
// -----------

template <typename P>
using deref = decltype(*std::declval<P>());

template <typename P>
using double_deref = deref<deref<P>>;

template <typename P>
using sequence_iterator_impl = transform_iterator<P, std::function<double_deref<P>(deref<P>)>>;

}   /* detail */

PYCPP_END_NAMESPACE
