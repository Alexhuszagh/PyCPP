//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform whitespace defintions.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr char SPACE[] = " ";
static constexpr char WHITESPACE[] = " \t\n\r\v\f";
static constexpr char WINDOWS_NEWLINE[] = "\r\n";
static constexpr char POSIX_NEWLINE[] = "\n";
#if defined(OS_WINDOWS)
    static constexpr char NEWLINE[] = "\r\n";
#else
    static constexpr char NEWLINE[] = "\n";
#endif

PYCPP_END_NAMESPACE
