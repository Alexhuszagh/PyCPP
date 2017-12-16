//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <exception> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <exception>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::make_exception_ptr;
using std::current_exception;
using std::rethrow_exception;
using std::throw_with_nested;
using std::rethrow_if_nested;
using std::terminate;
using std::get_terminate;
using std::set_terminate;

using exception = std::exception;
using nested_exception = std::nested_exception;
using bad_exception = std::bad_exception;
using terminate_handler = std::terminate_handler;
using exception_ptr = std::exception_ptr;

#if defined(HAVE_CPP17)

using std::uncaught_exception;
using std::uncaught_exceptions;

#endif          // HAVE_CPP17

PYCPP_END_NAMESPACE