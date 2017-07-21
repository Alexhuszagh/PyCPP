//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief File descriptor definitions.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/os.h>
#if defined(OS_WINDOWS)             // WINDOWS
#   include <windows.h>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(OS_WINDOWS)             // WINDOWS
    typedef HANDLE fd_t;
#else                               // POSIX
    typedef int fd_t;
#endif

PYCPP_END_NAMESPACE
