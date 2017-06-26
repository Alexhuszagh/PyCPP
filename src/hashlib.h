//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Hash functions.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

// MD5

/**
 *  \brief Calculate MD5 hash.
 */
size_t md5_hash(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Calculate MD5 hash.
 */
std::string md5_hash(const std::string& str);

/**
 *  \brief Calculate hex digest of MD5 hash.
 */
size_t md5_digest(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Calculate hex digest of MD5 hash.
 */
std::string md5_digest(const std::string& str);

// SHA1

/**
 *  \brief Calculate SHA1 hash.
 */
size_t sha1_hash(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Calculate SHA1 hash.
 */
std::string sha1_hash(const std::string& str);

/**
 *  \brief Calculate hex digest of SHA1 hash.
 */
size_t sha1_digest(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Calculate hex digest of SHA1 hash.
 */
std::string sha1_digest(const std::string& str);
