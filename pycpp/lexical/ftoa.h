//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast lexical float-to-string conversion routines.
 *
 *  This module will set `last` to the be the element past-the-end
 *  of the number (containing a null character), and will always write
 *  a null terminator.
 */

#include <pycpp/stl/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Get character designating the exponent in sci notation.
 */
char e_notation_char(uint8_t base) noexcept;

/**
 *  \brief Convert unsigned 8-bit value to string.
 */
void f32toa(float value, char* first, char*& last, uint8_t base = 10) noexcept;
std::string f32toa(float value, uint8_t base = 10);

/**
 *  \brief Convert signed 8-bit value to string.
 */
void f64toa(double value, char* first, char*& last, uint8_t base = 10) noexcept;
std::string f64toa(double value, uint8_t base = 10);

PYCPP_END_NAMESPACE
