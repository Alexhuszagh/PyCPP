//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Numerical averages.
 */

#pragma once

#include <pycpp/config.h>
#include <cstdlib>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/** \brief Calculate average of range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double average(Iter first,
    Iter last)
{
    double sum = 0;
    size_t length = 0;
    while (first != last) {
        sum += *first++;
        ++length;
    }

    return sum / length;
}


/** \brief Calculate average of range with custom summer.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *  \param sum              Operation to extract value from value_type
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <
    typename Iter,
    typename Sum
>
double average(Iter first,
    Iter last,
    Sum sum)
{
    double sums = 0;
    size_t length = 0;
    while (first != last) {
        sums += sum(*first++);
        ++length;
    }

    return sums / length;
}


/** \brief Calculate weighted average of range.
 *
 *  \param value_first      Iterator at beginning of values
 *  \param value_last       Iterator past end of values
 *  \param weight_first     Iterator at beginning of weights
 *  \param weight_last      Iterator past end of weights
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <
    typename ValueIter,
    typename WeightIter
>
double average(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last)
{
    double sums = 0;
    double weights = 0;
    while (value_first != value_last && weight_first != weight_last) {
        const double w = *weight_first++;
        sums += w * *value_first++;
        weights += w;
    }

    return sums / weights;
}


/** \brief Calculate weighted average of range with custom predicates.
 *
 *  \param value_first      Iterator at beginning of values
 *  \param value_last       Iterator past end of values
 *  \param weight_first     Iterator at beginning of weights
 *  \param weight_last      Iterator past end of weights
 *  \param sum              Operation to extract value from value_type
 *  \param weight           Operation to extract weight from value_type
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <
    typename ValueIter,
    typename WeightIter,
    typename Sum,
    typename Weight
>
double average(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Sum sum,
    Weight weight)
{
    double sums = 0;
    double weights = 0;
    while (value_first != value_last && weight_first != weight_last) {
        const double w = weight(*weight_first++);
        sums += w * sum(*value_first++);
        weights += w;
    }

    return sums / weights;
}

PYCPP_END_NAMESPACE
