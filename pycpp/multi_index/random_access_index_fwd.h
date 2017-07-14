//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::random_access.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/random_access_index_fwd.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename TagList = tag<>>
using random_access = multi_index::random_access<TagList>;

PYCPP_END_NAMESPACE
