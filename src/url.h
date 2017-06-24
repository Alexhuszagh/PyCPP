//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief URL encoding and decoding.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------


/** \brief URL-encode string.
 *
 *  \param string       String to encode
 *  \return             URL-encoded string
 */
std::string url_encode(const std::string &string);

/** \brief URL-decode string.
 *
 *  \param string       String to encode
 *  \return             URL-decoded string
 */
std::string url_decode(const std::string &string);
