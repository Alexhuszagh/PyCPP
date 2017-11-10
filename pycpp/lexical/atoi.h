//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical string-to-integer conversion routines.
 *
 *  These routines are naive, yet performant, thread-safe, and locale-
 *  independent, and therefore should be preferred to `std::stol` or
 *  `atol`.
 */

#include <pycpp/config.h>
#include <pycpp/view/string.h>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert string to unsigned 8-bit value.
 */
uint8_t u8toa(const char* first, const char*& last, uint8_t base = 10);
uint8_t u8toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to signed 8-bit value.
 */
int8_t i8toa(const char* first, const char*& last, uint8_t base = 10);
int8_t i8toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to unsigned 16-bit value.
 */
uint16_t u16toa(const char* first, const char*& last, uint8_t base = 10);
uint16_t u16toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to signed 16-bit value.
 */
int16_t i16toa(const char* first, const char*& last, uint8_t base = 10);
int16_t i16toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to unsigned 32-bit value.
 */
uint32_t u32toa(const char* first, const char*& last, uint8_t base = 10);
uint32_t u32toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to signed 32-bit value.
 */
int32_t i32toa(const char* first, const char*& last, uint8_t base = 10);
int32_t i32toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to unsigned 64-bit value.
 */
uint64_t u64toa(const char* first, const char*& last, uint8_t base = 10);
uint64_t u64toa(const string_view& string, uint8_t base = 10);

/**
 *  \brief Convert string to signed 64-bit value.
 */
int64_t i64toa(const char* first, const char*& last, uint8_t base = 10);
int64_t i64toa(const string_view& string, uint8_t base = 10);

PYCPP_END_NAMESPACE