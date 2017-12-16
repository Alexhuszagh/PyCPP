//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Core helpers for semiintrusive STL containers.
 */

#pragma once

#include <pycpp/iterator/transform.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

namespace sequence_detail
{
// DECLARATION
// -----------

template <typename P>
using deref = decltype(*declval<P>());

template <typename P>
using double_deref = deref<deref<P>>;

template <typename P>
using double_deref_value = remove_reference_t<double_deref<P>>;

template <typename P>
using const_double_deref = const double_deref_value<P>&;

template <typename P>
using sequence_iterator_impl = transform_iterator<P, function<double_deref<P>(deref<P>)>>;

template <typename P>
using sequence_const_iterator_impl = transform_iterator<P, function<const_double_deref<P>(deref<P>)>>;

}   /* sequence_detail */

PYCPP_END_NAMESPACE
