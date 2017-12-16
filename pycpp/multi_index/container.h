//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::multi_index_container.
 */

#pragma once

#include <pycpp/multi_index/identity.h>
#include <pycpp/multi_index/indexed_by.h>
#include <pycpp/multi_index/ordered_index_fwd.h>
#include <pycpp/stl/memory.h>
#include <multi_index_container.hpp>

#if defined(PYCPP_HAVE_NAMESPACE)

PYCPP_BEGIN_NAMESPACE

template <
    typename Value,
    typename IndexSpecifierList = indexed_by<ordered_unique<identity<Value>>>,
    typename Allocator = allocator<Value>
>
using multi_index_container = multi_index::multi_index_container<Value, IndexSpecifierList, Allocator>;

PYCPP_END_NAMESPACE

#endif                  // PYCPP_HAVE_NAMESPACE
