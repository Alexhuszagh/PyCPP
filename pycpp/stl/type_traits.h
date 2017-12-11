//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <type_traits> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T>
using is_arithmetic = std::is_arithmetic<T>;

template <typename T, typename U>
using is_same = std::is_same<T, U>;

template <typename T, typename U>
using is_base_of = std::is_base_of<T, U>;

PYCPP_END_NAMESPACE
