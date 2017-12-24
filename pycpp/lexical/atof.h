//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical string-to-float conversion routines.
 *
 *  TODO: change values based on benchmark values
 *  These routines are naive, yet 15% faster than the STL versions,
 *  thread-safe, and locale-independent, and should be generally
 *  preferred to `std::stod` or `atof`. See out `bench/lexical.cc` for
 *  benchmarks comparing PyCPP to the STL versions.
 */

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

/**
 *  \brief Convert string to 32-bit IEE754 floating point value.
 */
float atof32(const char* first, const char*& last, uint8_t base = 10) noexcept;
float atof32(const string_view& string, uint8_t base = 10) noexcept;

/**
 *  \brief Convert string to 64-bit IEE754 floating point value.
 */
double atof64(const char* first, const char*& last, uint8_t base = 10) noexcept;
double atof64(const string_view& string, uint8_t base = 10) noexcept;

PYCPP_END_NAMESPACE
