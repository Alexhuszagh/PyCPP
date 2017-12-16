//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <random> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <random>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::default_random_engine;
using std::exponential_distribution;
using std::gamma_distribution;
using std::lognormal_distribution;
using std::mersenne_twister_engine;
using std::normal_distribution;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::weibull_distribution;
using std::mt19937;
using std::mt19937_64;

PYCPP_END_NAMESPACE
