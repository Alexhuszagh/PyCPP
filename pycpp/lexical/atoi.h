//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical string-to-integer conversion routines.
 *
 *  These routines are naive, yet 30-50% faster than the STL versions,
 *  thread-safe, and locale-independent, and should be generally
 *  preferred to `std::stol` or `atol`. See out `bench/lexical.cc` for
 *  benchmarks comparing PyCPP to the STL versions.
 */

#include <pycpp/stl/string_view.h>
#include <stdint.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert string to unsigned 8-bit value.
 */
uint8_t atou8(const char* first, const char*& last, uint8_t base = 10) noexcept;
uint8_t atou8(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to signed 8-bit value.
 */
int8_t atoi8(const char* first, const char*& last, uint8_t base = 10) noexcept;
int8_t atoi8(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to unsigned 16-bit value.
 */
uint16_t atou16(const char* first, const char*& last, uint8_t base = 10) noexcept;
uint16_t atou16(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to signed 16-bit value.
 */
int16_t atoi16(const char* first, const char*& last, uint8_t base = 10) noexcept;
int16_t atoi16(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to unsigned 32-bit value.
 */
uint32_t atou32(const char* first, const char*& last, uint8_t base = 10) noexcept;
uint32_t atou32(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to signed 32-bit value.
 */
int32_t atoi32(const char* first, const char*& last, uint8_t base = 10) noexcept;
int32_t atoi32(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to unsigned 64-bit value.
 */
uint64_t atou64(const char* first, const char*& last, uint8_t base = 10) noexcept;
uint64_t atou64(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to signed 64-bit value.
 */
int64_t atoi64(const char* first, const char*& last, uint8_t base = 10) noexcept;
int64_t atoi64(const string_view& string, uint8_t base = 10) noexcept;

PYCPP_END_NAMESPACE
