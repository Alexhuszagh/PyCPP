//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Access pattern definitions.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// ENUM
// ----

/**
 *  \brief File I/O access patterns.
 */
enum io_access_pattern
{
    access_normal = 0,
    access_sequential,
    access_random,
};

PYCPP_END_NAMESPACE
