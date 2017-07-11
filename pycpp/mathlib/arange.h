//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Method analogous to NumPy arrange.
 */

#pragma once

#include <pycpp/mathlib/array.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Generate values in range [start, stop) by step.
 */
template <typename T, typename Compare>
ndarray<T> arange(Compare comp, T start, T stop, T step = 1)
{
    std::vector<T> list;
    list.reserve((stop - start) / step);
    for (T i = start; comp(i, stop); i += step) {
        list.push_back(i);
    }
    return ndarray<T>(std::move(list));
}


/**
 *  \brief Generate values in range [start, stop) by step.
 */
template <typename T>
ndarray<T> arange(T start, T stop, T step = 1)
{
    // check if either both step and the range have the same direction
    assert((step > 0) ^ (stop - start < 0));
    if (step > 0) {
        return arange(std::less<T>(), start, stop, step);
    }
    return arange(std::greater<T>(), start, stop, step);
}

PYCPP_END_NAMESPACE
