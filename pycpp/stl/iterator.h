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

using std::iterator;
using std::istream_iterator;
using std::ostream_iterator;
using std::istreambuf_iterator;
using std::ostreambuf_iterator;
using std::iterator_traits;
using std::reverse_iterator;
using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

PYCPP_END_NAMESPACE
