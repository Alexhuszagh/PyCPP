//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::global_fun.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/global_fun.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename Value, typename T, T (*PtrToFunction)(Value)>
using global_fun = multi_index::global_fun<Value, T, PtrToFunction>;

PYCPP_END_NAMESPACE
