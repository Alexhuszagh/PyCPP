//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Numerical averages.
 */

#pragma once

#include <pycpp/misc/xrange.h>
#include <pycpp/preprocessor/parallel.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/iterator.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Calculate average of range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double average(Iter first, Iter last) noexcept
{
    using value_type = typename iterator_traits<Iter>::value_type;
    static_assert(is_arithmetic<value_type>::value, "");

    double sum = 0;
    for_each(PARALLEL_EXECUTION first, last, [&sum](const value_type& value) {
        sum += value;
    });

    return sum / distance(first, last);
}


/**
 *  \brief Calculate average of range with custom summer.
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
    typename Summer
>
double average(Iter first,
    Iter last,
    Summer summer) noexcept
{
    using value_type = typename iterator_traits<Iter>::value_type;
    static_assert(is_arithmetic<value_type>::value, "");

    double sum = 0;
    for_each(PARALLEL_EXECUTION first, last, [&](const value_type& value) {
        sum += summer(value);
    });

    return sum / distance(first, last);
}


/**
 *  \brief Calculate weighted average of range.
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
    WeightIter weight_last) noexcept
{
    static_assert(is_arithmetic<typename iterator_traits<ValueIter>::value_type>::value, "");
    static_assert(is_arithmetic<typename iterator_traits<WeightIter>::value_type>::value, "");

    double sum = 0;
    double weight = 0;
    size_t dist = min(distance(value_first, value_last), distance(weight_first, weight_last));
    auto r = xrange<size_t>(0, dist, 1);
    for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double w = weight_first[i];
        sum += value_first[i] * w;
        weight += w;
    });

    return sum / weight;
}


/**
 *  \brief Calculate weighted average of range with custom predicates.
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
    typename Summer,
    typename Weighter
>
double average(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Summer summer,
    Weighter weighter) noexcept
{
    static_assert(is_arithmetic<typename iterator_traits<ValueIter>::value_type>::value, "");
    static_assert(is_arithmetic<typename iterator_traits<WeightIter>::value_type>::value, "");

    double sum = 0;
    double weight = 0;
    size_t dist = min(distance(value_first, value_last), distance(weight_first, weight_last));
    auto r = xrange<size_t>(0, dist, 1);
    for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double w = weighter(weight_first[i]);
        sum += summer(value_first[i]) * w;
        weight += w;
    });

    return sum / weight;
}

PYCPP_END_NAMESPACE
