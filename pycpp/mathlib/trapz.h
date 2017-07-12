//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Trapezoidal integration implementation.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/** \brief Calculate trapezoidal integral of equally-spaced values over range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *  \dx                     Spacing of values
 */
template <typename Iter>
double trapz(Iter first,
    Iter last,
    const double dx = 1)
{
    double integral = 0;
    while ((first+1) != last) {
        auto yi = *first++;
        auto yj = *first;
        integral += 0.5 * dx * (yj + yi);
    }

    return integral;
}


/** \brief Calculate trapezoidal integral of values over range.
 *
 *  \param y_first          Iterator at beginning of range for values
 *  \param y_end            Iterator past end of range for values
 *  \param x_first          Iterator at beginning of range for spacing
 *  \param x_last           Iterator past end of range for spacing
 */
template <
    typename YIter,
    typename XIter
>
double trapz(YIter y_first,
    YIter y_end,
    XIter x_first,
    XIter x_last)
{
    double integral = 0;
    while ((y_first+1) != y_end && (x_first+1) != x_last) {
        auto xi = *x_first++;
        auto xj = *x_first;
        auto yi = *y_first++;
        auto yj = *y_first;
        integral += 0.5 * (xj - xi) * (yj + yi);
    }

    return integral;
}


/** \brief Calculate trapezoidal integral of equally spaced values over range.
 *
 *  \param first            Iterator at beginning of range for values
 *  \param last             Iterator past end of range for values
 *  \param fun              Function to extract value from value_type
 */
template <
    typename Iter,
    typename Fun
>
double trapz(Iter first,
    Iter last,
    const double dx,
    Fun fun)
{
    double integral = 0;
    while ((first+1) != last) {
        auto yi = fun(*first++);
        auto yj = fun(*first);
        integral += 0.5 * dx * (yj + yi);
    }

    return integral;
}



/** \brief Calculate trapezoidal integral of values over range.
 *
 *  \param y_first          Iterator at beginning of range for values
 *  \param y_end            Iterator past end of range for values
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
double trapz(YIter y_first,
    YIter y_end,
    XIter x_first,
    XIter x_last,
    YFun y_fun,
    XFun x_fun)
{
    double integral = 0;
    while ((y_first+1) != y_end && (x_first+1) != x_last) {
        auto xi = x_fun(*x_first++);
        auto xj = x_fun(*x_first);
        auto yi = y_fun(*y_first++);
        auto yj = y_fun(*y_first);
        integral += 0.5 * (xj - xi) * (yj + yi);
    }

    return integral;
}

PYCPP_END_NAMESPACE
