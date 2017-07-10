//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP methods.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef DELETE

// OBJECTS
// -------


/**
 *  \brief HTTP request methods.
 */
enum method_t: unsigned int
{
    STOP    = 0,
    GET     = 1,
    HEAD    = 2,
    POST    = 3,
    DELETE  = 4,
    OPTIONS = 5,
    PATCH   = 6,
    PUT     = 7,
    TRACE   = 8,
    CONNECT = 9,
};

PYCPP_END_NAMESPACE
