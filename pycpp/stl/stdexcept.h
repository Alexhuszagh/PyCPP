//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <stdexcept> aliases.
 */

#pragma once

#include <pycpp/stl/exception.h>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::logic_error;
using std::invalid_argument;
using std::domain_error;
using std::length_error;
using std::out_of_range;
using std::runtime_error;
using std::range_error;
using std::overflow_error;
using std::underflow_error;

PYCPP_END_NAMESPACE
