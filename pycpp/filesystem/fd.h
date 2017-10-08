//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief File descriptor definitions.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>
#if defined(OS_WINDOWS)             // WINDOWS
#   include <pycpp/windows/winapi.h>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(OS_WINDOWS)             // WINDOWS
    typedef HANDLE fd_t;
    #define INVALID_FD_VALUE INVALID_HANDLE_VALUE
#else                               // POSIX
    typedef int fd_t;
    #define INVALID_FD_VALUE -1
#endif

PYCPP_END_NAMESPACE
