//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Dot product implementation.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/** \brief Calculate dot product over range.
 *
 *  \param y_first          Iterator at beginning of range for values
 *  \param y_last           Iterator past end of range for values
 *  \param x_first          Iterator at beginning of range for spacing
 *  \param x_last           Iterator past end of range for spacing
 */
template <
    typename YIter,
    typename XIter
>
double dot(XIter x_first,
    XIter x_last,
    YIter y_first,
    YIter y_last)
{
    double sum = 0;
    while (x_first != x_last && y_first != y_last) {
        sum += *x_first++ * *y_first++;
    }

    return sum;
}


/** \brief Calculate dot product over range.
 *
 *  \param y_first          Iterator at beginning of range for values
 *  \param y_last           Iterator past end of range for values
 *  \param x_first          Iterator at beginning of range for spacing
 *  \param x_last           Iterator past end of range for spacing
 *  \param y_fun            Function to extract y-value from value_type
 *  \param x_fun            Function to extract x-value from value_type
 */
template <
    typename YIter,
    typename XIter,
    typename YFun,
    typename XFun
>
double dot(XIter x_first,
    XIter x_last,
    YIter y_first,
    YIter y_last,
    XFun x_fun,
    YFun y_fun)
{
    double sum = 0;
    while (x_first != x_last && y_first != y_last) {
        sum += x_fun(*x_first++) * y_fun(*y_first++);
    }

    return sum;
}

PYCPP_END_NAMESPACE
