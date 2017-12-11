//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief URL encoding and decoding.
 */

#pragma once

#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/**
 *  \brief URL-encode string.
 *
 *  \param string       String to encode
 *  \return             URL-encoded string
 */
std::string url_encode(const string_wrapper& string);

/**
 *  \brief URL-decode string.
 *
 *  \param string       String to encode
 *  \return             URL-decoded string
 */
std::string url_decode(const string_wrapper& string);

PYCPP_END_NAMESPACE
