//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP transfer encoding.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/misc/enum.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Enumerations for acceptable transfer encodings.
 */
enum transfer_encoding_t: unsigned int
{
    CHUNKED     = 1,
    COMPRESS    = 2,
    DEFLATE     = 4,
    GZIP        = 8,
    IDENTITY    = 16,
};

PYCPP_ENUM_FLAG(transfer_encoding_t);

PYCPP_END_NAMESPACE
