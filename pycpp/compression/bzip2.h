//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief BZIP2 compression and decompression.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/** \brief Convert UTF8-encoded data to Punycode.
 */
std::string bzip2_compress(const std::string &str);

PYCPP_END_NAMESPACE
