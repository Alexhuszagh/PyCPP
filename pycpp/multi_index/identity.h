//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::identity.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/identity.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename Type>
using identity = multi_index::identity<Type>;

PYCPP_END_NAMESPACE
