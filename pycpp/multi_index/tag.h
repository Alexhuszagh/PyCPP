//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::tag.
 */

#pragma once

#include <pycpp/multi_index/config.h>
#include <multi_index/tag.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename... Ts>
using tag = multi_index::tag<Ts...>;

PYCPP_END_NAMESPACE
