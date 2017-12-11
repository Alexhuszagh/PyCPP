//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <limits> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <limits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T>
using numeric_limits = std::numeric_limits<T>;

PYCPP_END_NAMESPACE
