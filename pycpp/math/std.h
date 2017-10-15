//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Numerical variance and standard deviations.
 */

#pragma once

#include <pycpp/math/average.h>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Calculate variance of range with pre-calculated mean.
 *
 *  \param mean             Mean for calculation
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double variance(double mean,
    Iter first,
    Iter last)
{
    using value_type = typename std::iterator_traits<Iter>::value_type;
    double sum = 0;
    std::for_each(PARALLEL_EXECUTION first, last, [&](const value_type& value) {
        sum += pow(value - mean, 2);
    });

    return sum / std::distance(first, last);
}


/**
 *  \brief Calculate standard deviation of range with pre-calculated mean.
 *
 *  \param mean             Mean for calculation
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double stdev(double mean,
    Iter first,
    Iter last)
{
    return sqrt(variance(mean, first, last));
}


/**
 *  \brief Calculate variance of range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double variance(Iter first,
    Iter last)
{
    return variance(average(first, last), first, last);
}


/**
 *  \brief Calculate standard deviation of range.
 *
 *  \param first            Iterator at beginning of range
 *  \param last             Iterator past end of range
 *
 *  \warning This function does no range checking, and may return
 *  NAN or INF.
 */
template <typename Iter>
double stdev(Iter first,
    Iter last)
{
    return stdev(average(first, last), first, last);
}


/**
 *  \brief Calculate variance of range with custom summer and pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double variance(double mean,
    Iter first,
    Iter last,
    Summer summer)
{
    using value_type = typename std::iterator_traits<Iter>::value_type;
    double sum = 0;
    std::for_each(PARALLEL_EXECUTION first, last, [&](const value_type& value) {
        sum += pow(summer(value) - mean, 2);
    });

    return sum / std::distance(first, last);
}


/**
 *  \brief Calculate standard deviation of range with custom summer and pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double stdev(double mean,
    Iter first,
    Iter last,
    Sum sum)
{
    return sqrt(variance(mean, first, last, sum));
}


/**
 *  \brief Calculate variance of range with custom summer.
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
double variance(Iter first,
    Iter last,
    Sum sum)
{
    return variance(average(first, last, sum), first, last, sum);
}


/**
 *  \brief Calculate standard deviation of range with custom summer.
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
double stdev(Iter first,
    Iter last,
    Sum sum)
{
    return stdev(average(first, last, sum), first, last, sum);
}


/**
 *  \brief Calculate variance of range with a pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double variance(double mean,
    ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last)
{
    double sum = 0;
    double weight = 0;
    size_t distance = std::min(std::distance(value_first, value_last), std::distance(weight_first, weight_last));
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double v = pow(value_first[i] - mean, 2);
        double w = weight_first[i];
        sum += w * v;
        weight += w;
    });

    return sum / (weight * (distance / (distance-1)));
}


/**
 *  \brief Calculate standard deviation of range with a pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double stdev(double mean,
    ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last)
{
    return sqrt(variance(mean, value_first, value_last, weight_first, weight_last));
}


/**
 *  \brief Calculate variance of range.
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
double variance(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last)
{
    auto mean = average(value_first, value_last, weight_first, weight_last);
    return variance(mean, value_first, value_last, weight_first, weight_last);
}


/**
 *  \brief Calculate standard deviation of range.
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
double stdev(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last)
{
    return sqrt(variance(value_first, value_last, weight_first, weight_last));
}


/**
 *  \brief Calculate variance of range with custom predicates and pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double variance(double mean,
    ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Summer summer,
    Weighter weighter)
{
    double sum = 0;
    double weight = 0;
    size_t distance = std::min(std::distance(value_first, value_last), std::distance(weight_first, weight_last));
    auto r = xrange<size_t>(0, distance, 1);
    std::for_each(PARALLEL_EXECUTION r.begin(), r.end(), [&](size_t i) {
        double v = pow(summer(value_first[i]) - mean, 2);
        double w = weighter(weight_first[i]);
        sum += w * v;
        weight += w;
    });

    return sum / (weight * (distance / (distance-1)));
}


/**
 *  \brief Calculate standard deviation of range with custom predicates and pre-calculated mean.
 *
 *  \param mean             Mean for calculation
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
double stdev(double mean,
    ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Sum sum,
    Weight weight)
{
    return sqrt(variance(mean, value_first, value_last, weight_first, weight_last, sum, weight));
}


/**
 *  \brief Calculate variance of range with custom predicates.
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
double variance(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Sum sum,
    Weight weight)
{
    auto mean = average(value_first, value_last, weight_first, weight_last, sum, weight);
    return variance(mean, value_first, value_last, weight_first, weight_last, sum, weight);
}


/**
 *  \brief Calculate standard deviation of range with custom predicates.
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
double stdev(ValueIter value_first,
    ValueIter value_last,
    WeightIter weight_first,
    WeightIter weight_last,
    Sum sum,
    Weight weight)
{
    return sqrt(variance(value_first, value_last, weight_first, weight_last, sum, weight));
}

PYCPP_END_NAMESPACE
