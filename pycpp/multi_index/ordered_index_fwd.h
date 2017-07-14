//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::ordered_unique.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/ordered_index_fwd.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename T1, typename... Ts>
using ordered_unique = multi_index::ordered_unique<T1, Ts...>;

template <typename T1, typename... Ts>
using ordered_non_unique = multi_index::ordered_unique<T1, Ts...>;

PYCPP_END_NAMESPACE
