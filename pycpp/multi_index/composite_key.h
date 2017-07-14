//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::hashed_unique.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/composite_key.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename Value, typename... Ts>
using composite_key = multi_index::composite_key<Value, Ts...>;

PYCPP_END_NAMESPACE
