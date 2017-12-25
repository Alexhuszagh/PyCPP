//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical integer-to-string conversion routines.
 *
 *  This module will set `last` to the be the element past-the-end
 *  of the number (containing a null character), and will always write
 *  a null terminator.
 *
 *  Unoptimized verions of each routine are ~25% slower than `std::to_string`,
 *  however, each base has an optimize version that is ~5x faster than
 *  `std::to_string`.
 */

#include <pycpp/stl/string.h>
#include <stdint.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert unsigned 8-bit value to string.
 */
void u8toa(uint8_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string u8toa(uint8_t value, uint8_t base = 10);

/**
 *  \brief Convert signed 8-bit value to string.
 */
void i8toa(int8_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string i8toa(int8_t value, uint8_t base = 10);

/**
 *  \brief Convert unsigned 16-bit value to string.
 */
void u16toa(uint16_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string u16toa(uint16_t value, uint8_t base = 10);

/**
 *  \brief Convert signed 16-bit value to string.
 */
void i16toa(int16_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string i16toa(int16_t value, uint8_t base = 10);

/**
 *  \brief Convert unsigned 32-bit value to string.
 */
void u32toa(uint32_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string u32toa(uint32_t value, uint8_t base = 10);

/**
 *  \brief Convert signed 32-bit value to string.
 */
void i32toa(int32_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string i32toa(int32_t value, uint8_t base = 10);

/**
 *  \brief Convert unsigned 64-bit value to string.
 */
void u64toa(uint64_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string u64toa(uint64_t value, uint8_t base = 10);

/**
 *  \brief Convert signed 64-bit value to string.
 */
void i64toa(int64_t value, char* first, char*& last, uint8_t base = 10) noexcept;
string i64toa(int64_t value, uint8_t base = 10);

PYCPP_END_NAMESPACE
