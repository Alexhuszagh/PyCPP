//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <iterator> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// TODO: more stuff to add...

using std::advance;
using std::distance;
using std::next;
using std::prev;
using std::begin;
using std::end;
using std::front_inserter;
using std::back_inserter;
using std::inserter;

#if defined(HAVE_CPP14)

using std::cbegin;
using std::cend;
using std::rbegin;
using std::rend;
using std::crbegin;
using std::crend;

#endif          // HAVE_CPP14

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

template <typename Iterator>
using reverse_iterator = std::reverse_iterator<Iterator>;

using input_iterator_tag = std::input_iterator_tag;
using output_iterator_tag = std::output_iterator_tag;
using forward_iterator_tag = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;

PYCPP_END_NAMESPACE
