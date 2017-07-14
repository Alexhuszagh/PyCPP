//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::hashed_unique.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/hashed_index_fwd.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename... Ts>
using hashed_unique = multi_index::hashed_unique<Ts...>;

template <typename... Ts>
using hashed_non_unique = multi_index::hashed_unique<Ts...>;

PYCPP_END_NAMESPACE
