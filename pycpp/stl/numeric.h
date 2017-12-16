//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <numeric> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <numeric>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::iota;
using std::accumulate;
using std::inner_product;
using std::adjacent_difference;
using std::partial_sum;

#if defined(HAVE_CPP17)

using std::reduce;
using std::transform_reduce;
using std::inclusive_scan;
using std::exclusive_scan;
using std::transform_inclusive_scan;
using std::transform_exclusive_scan;
using std::gcd;
using std::lcm;

#endif          // HAVE_CPP17

PYCPP_END_NAMESPACE
