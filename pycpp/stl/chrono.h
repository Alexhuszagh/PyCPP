//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <chrono> aliases.
 */

#pragma once

#include <pycpp/stl/ratio.h>
#include <chrono>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

namespace chrono
{

using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;

using system_clock = std::chrono::system_clock;
using steady_clock = std::chrono::steady_clock;
using high_resolution_clock = std::chrono::high_resolution_clock;

template <
    typename Rep,
    typename Period = ratio<1>
>
using duration = std::chrono::duration<Rep, Period>;

template <
    typename Clock,
    typename Duration = typename Clock::duration
>
using time_point = std::chrono::time_point<Clock, Duration>;

template <typename Rep>
using treat_as_floating_point = std::chrono::treat_as_floating_point<Rep>;

template <typename Rep>
using duration_values = std::chrono::duration_values<Rep>;

}   /* chrono */

PYCPP_END_NAMESPACE
