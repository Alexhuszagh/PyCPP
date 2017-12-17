//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <new> aliases.
 */

#pragma once

#include <pycpp/stl/exception.h>
#include <new>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::bad_alloc;
using std::bad_array_new_length;
using std::new_handler;
using std::nothrow_t;
using std::nothrow;
using std::set_new_handler;

PYCPP_END_NAMESPACE
