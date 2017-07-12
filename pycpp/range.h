//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance generic type implementation.
 */

#pragma once

#include <pycpp/coroutine.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/**
 *  \brief Create generator from range [start, stop) using step intervals.
 */
template <typename T>
generator<T> range(T start, T stop, T step = 1)
{
    return generator<T>([=](generator<T>& gen) mutable {
        COROUTINE_REENTER(gen.coroutine()) {
            for (; start < stop; start += step) {
                COROUTINE_YIELD gen.store(start);
            }
        }
    });
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
