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

using logic_error = std::logic_error;
using invalid_argument = std::invalid_argument;
using domain_error = std::domain_error;
using length_error = std::length_error;
using out_of_range = std::out_of_range;
using runtime_error = std::runtime_error;
using range_error = std::range_error;
using overflow_error = std::overflow_error;
using underflow_error = std::underflow_error;

PYCPP_END_NAMESPACE