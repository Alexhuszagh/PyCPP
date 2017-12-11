//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Trapezoidal integration implementation.
 */

#pragma once

#include <pycpp/misc/xrange.h>
#include <pycpp/preprocessor/parallel.h>
#include <pycpp/stl/algorithm.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Calculate trapezoidal integral of equally-spaced values over range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *  \dx                     Spacing of values
 */
template <typename Iter>
double trapz(Iter first, Iter last, double dx = 1)
{
    double integral = 0;
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance-1, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double yi = first[i];
        double yj = first[i+1];
        integral += 0.5 * dx * (yj + yi);
    });

    return integral;
}


/**
 *  \brief Calculate trapezoidal integral of values over range.
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
double trapz(YIter y_first,
    YIter y_last,
    XIter x_first,
    XIter x_last)
{
    double integral = 0;
    size_t distance = std::min(std::distance(y_first, y_last), std::distance(x_first, x_last));
    auto r = xrange<size_t>(0, distance-1, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double xi = x_first[i];
        double xj = x_first[i+1];
        double yi = y_first[i];
        double yj = y_first[i+1];
        integral += 0.5 * (xj - xi) * (yj + yi);
    });

    return integral;
}


/**
 *  \brief Calculate trapezoidal integral of equally spaced values over range.
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
    double dx,
    Fun fun)
{
    double integral = 0;
    size_t distance = std::distance(first, last);
    auto r = xrange<size_t>(0, distance-1, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double yi = fun(first[i]);
        double yj = fun(first[i+1]);
        integral += 0.5 * dx * (yj + yi);
    });

    return integral;
}



/**
 *  \brief Calculate trapezoidal integral of values over range.
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
double trapz(YIter y_first,
    YIter y_last,
    XIter x_first,
    XIter x_last,
    YFun y_fun,
    XFun x_fun)
{
    double integral = 0;
    size_t distance = std::min(std::distance(y_first, y_last), std::distance(x_first, x_last));
    auto r = xrange<size_t>(0, distance-1, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double xi = x_fun(x_first[i]);
        double xj = x_fun(x_first[i+1]);
        double yi = y_fun(y_first[i]);
        double yj = y_fun(y_first[i+1]);
        integral += 0.5 * (xj - xi) * (yj + yi);
    });

    return integral;
}

PYCPP_END_NAMESPACE
