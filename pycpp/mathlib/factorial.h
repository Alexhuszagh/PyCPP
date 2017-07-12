//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Factorial implementation.
 */

#pragma once

#include <pycpp/config.h>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Calculate `N!`, its factorial.
 *
 *  For non-negative inputs, the factorial is approximated using the
 *  gamma function, which is represented by `tgamma(N+1)`, otherwise,
 *  0 is returned.
 */
template <typename N = int>
N factorial(N n)
{
    if (n < 0) {
        return 0;
    } else {
        return tgamma(n+1);
    }
}

PYCPP_END_NAMESPACE
