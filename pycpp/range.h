//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance generic type implementation.
 */

#pragma once

#include <pycpp/coroutine.h>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/**
 *  \brief Create generator from range [start, stop) using step intervals.
 */
template <typename Compare, typename T>
generator<T> range_impl(Compare comp, T start, T stop, T step)
{
    return generator<T>([=](generator<T>& gen) mutable {
        COROUTINE_REENTER(gen.coroutine()) {
            for (; comp(start, stop); start += step) {
                COROUTINE_YIELD gen.store(start);
            }
        }
    });
}


/**
 *  \brief Create generator from range [start, stop) using step intervals.
 */
template <typename T>
generator<T> range(T start, T stop, T step = 1)
{
    assert((step > 0) ^ (stop - start < 0));
    if (step > 0) {
        return range_impl(std::less<T>(), start, stop, step);
    }
    return range_impl(std::greater<T>(), start, stop, step);
}


/**
 *  \brief Create generator from range [0, stop).
 */
template <typename T>
generator<T> range(T stop)
{
    return range(0, stop, 1);
}


PYCPP_END_NAMESPACE
