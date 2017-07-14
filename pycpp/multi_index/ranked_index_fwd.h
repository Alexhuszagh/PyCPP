//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::ranked_unique.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/ranked_index_fwd.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename T1, typename... Ts>
using ranked_unique = multi_index::ranked_unique<T1, Ts...>;

template <typename T1, typename... Ts>
using ranked_non_unique = multi_index::ranked_non_unique<T1, Ts...>;

PYCPP_END_NAMESPACE
