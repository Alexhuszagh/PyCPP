//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Random number generators.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

/**
 *  \brief Get random bytes for cryptographic applications.
 */
size_t sysrandom(void* dst, size_t dstlen);

/**
 *  \brief Get random bytes for cryptographic applications.
 */
std::string sysrandom(size_t length);

/**
 *  \brief Get psuedo-random bytes for general purposes.
 *
 *  \param deterministic        Use deterministic seed.
 */
size_t pseudorandom(void* dst, size_t dstlen, bool deterministic = false);

/**
 *  \brief Get psuedo-random bytes for general purposes.
 *
 *  \param deterministic        Use deterministic seed.
 */
std::string pseudorandom(size_t length, bool deterministic = false);
