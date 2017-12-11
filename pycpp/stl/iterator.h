//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <iterator> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename Category,
    typename T,
    typename Distance = ptrdiff_t,
    typename Pointer = T*,
    typename Reference = T&
>
using iterator = std::iterator<Category, T, Distance, Pointer, Reference>;

template <typename Iterator>
using iterator_traits = std::iterator_traits<Iterator>;

using input_iterator_tag = std::input_iterator_tag;
using output_iterator_tag = std::output_iterator_tag;
using forward_iterator_tag = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;

PYCPP_END_NAMESPACE
