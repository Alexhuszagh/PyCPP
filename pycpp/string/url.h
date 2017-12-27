//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief URL encoding and decoding.
 *
 *  Convert strings to encode or decode characters to a special character
 *  set so they are suitable to be sent to web servers as a URL.
 *
 *  \synopsis
 *      string url_encode(const string_wrapper& string,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string url_decode(const string_wrapper& string,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief URL-encode string.
 *
 *  \param string           String to encode.
 *  \param allocator        Allocator for output string.
 */
string url_encode(const string_wrapper& string,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief URL-decode string.
 *
 *  \param string           String to encode.
 *  \param allocator        Allocator for output string.
 */
string url_decode(const string_wrapper& string,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
