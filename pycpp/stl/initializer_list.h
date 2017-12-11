//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <initializer_list> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <initializer_list>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T>
using initializer_list = std::initializer_list<T>;

PYCPP_END_NAMESPACE
