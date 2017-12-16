//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <complex> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <complex>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T>
using complex = std::complex<T>;

PYCPP_END_NAMESPACE
