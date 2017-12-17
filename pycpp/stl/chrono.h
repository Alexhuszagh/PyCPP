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
using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::time_point;
using std::chrono::treat_as_floating_point;
using std::chrono::duration_values;

}   /* chrono */

PYCPP_END_NAMESPACE
