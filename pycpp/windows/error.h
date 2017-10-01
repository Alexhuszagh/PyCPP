//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows-specific error handling.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Translate a Win32 error from `GetLastError()` to `errno`.
 */
int translate_win32_error(int code);


/**
 *  \brief Set `errno` appropriately from `GetLastError()`.
 */
void set_errno_win32();

PYCPP_END_NAMESPACE
