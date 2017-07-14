//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::sequenced.
 */

#pragma once

#include <pycpp/multi_index/tag.h>
#include <multi_index/sequenced_index_fwd.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename TagList = tag<>>
using sequenced = multi_index::sequenced<TagList>;

PYCPP_END_NAMESPACE
