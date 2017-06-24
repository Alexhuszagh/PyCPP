//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Cross-platform getline implementation.
 */

#pragma once

#ifndef _MSC_VER
#   include "sys/time.h"
#else

// OBJECTS
// -------

#ifndef _TIMEVAL_DEFINED
#   define _TIMEVAL_DEFINED

struct timeval
{
    long tv_sec;
    long tv_usec;
};

#endif


// FUNCTIONS
// ---------


/** \brief gettimeofday implementation for MSVC.
 *
 *  tz, the timezone, is ignored and no longer stored by the kernel.
 */
int gettimeofday(struct timeval* tp, void* tz);

#endif
