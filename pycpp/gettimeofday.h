//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform getline implementation.
 */

#pragma once

#ifndef _MSC_VER
#   include <sys/time.h>
#else
#   include <winsock2.h>

// FUNCTIONS
// ---------


/** \brief gettimeofday implementation for MSVC.
 *
 *  tz, the timezone, is ignored and no longer stored by the kernel.
 */
int gettimeofday(struct timeval* tp, void* tz);

#endif
