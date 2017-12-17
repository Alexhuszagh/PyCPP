//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Streams module.
 */

#pragma once

#include <stream/filter.h>
#if BUILD_FILESYSTEM
#   include <stream/fd.h>
#   include <stream/mmap.h>
#   include <stream/random_access.h>
#   include <stream/sequential.h>
#endif  // BUILD_FILESYSTEM
