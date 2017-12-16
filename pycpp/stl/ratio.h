//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <ratio> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <ratio>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    intmax_t Numerator,
    intmax_t Denominator = 1
>
using ratio = std::ratio<Numerator, Denominator>;

PYCPP_END_NAMESPACE
