//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Binomial coefficient implementation.
 */

#pragma once

#include <pycpp/stl/stdexcept.h>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Fast binomial coefficient calculation.
 *  \author Modified from [Etceterology](1).
 *
 *  \param n            Number of total values
 *  \param k            Number of choices.
 *  \return             Binomial coefficient
 *
 *  1. http://etceterology.com/fast-binomial-coefficients
 */
template <typename T>
T binomial(T n, T k)
{
    if (!k || n == k) {
        return 1;
    } else if (n < 0) {
        throw overflow_error("Numeric overflow in binomial coeff.\n");
    } else if (k < 0 || k > n) {
        return 0;
    }

    if (k > (n-k)) {
        k = n - k;
    }

    T value = 1;
    for (T i = 1; i <= k; ++i) {
        value *= (n - (k - i));
        if (value < 0) {
            throw overflow_error("Numeric overflow in binomial coeff.\n");
        }
        value /= i;
    }
    return value;
}

PYCPP_END_NAMESPACE
