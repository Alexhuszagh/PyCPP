//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::indexed_by.
 */

#pragma once

#include <pycpp/multi_index/config.h>
#include <multi_index/indexed_by.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename... Ts>
using indexed_by = multi_index::indexed_by<Ts...>;

PYCPP_END_NAMESPACE
