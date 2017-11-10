//  :copyright: (c) 2014 Milo Yip.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical integer-to-string conversion routines.
 */

#include <pycpp/config.h>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert unsigned 32-bit value to string.
 */
char* u32toa(uint32_t value, char* buffer);

/**
 *  \brief Convert signed 32-bit value to string.
 */
char* i32toa(int32_t value, char* buffer);

/**
 *  \brief Convert unsigned 64-bit value to string.
 */
char* u64toa(uint64_t value, char* buffer);

/**
 *  \brief Convert signed 64-bit value to string.
 */
char* i64toa(int64_t value, char* buffer);

PYCPP_END_NAMESPACE
