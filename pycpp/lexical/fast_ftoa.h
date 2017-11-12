//  :copyright: (c) 2010 V8 Project Authors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: BSD-3, see licenses/bsd-3.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Modification of V8's fast dtoa implementation.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// TODO: change the kConstant syntax to CONSTANT
// CONSTANTS
// ---------

/**
 *  FastDtoa will produce at most kFastDtoaMaximalLength digits. This does not
 *  include the terminating '\0' character.
 */
static const int kFastDtoaMaximalLength = 17;
static const int kFastDtoaMaximalSingleLength = 9;

// ENUMS
// -----

enum fast_dtoa_mode {
    /**
     *  Computes the shortest representation of the given input. The returned
     *  result will be the most accurate number of this length. Longer
     *  representations might be more accurate.
     */
    fast_dtoa_shortest,
    /**
     *  Same as fast_dtoa_shortest but for single-precision floats.
     */
    fast_dtoa_shortest_single,
    /**
     *  Computes a representation where the precision (number of digits) is
     *  given as input. The precision is independent of the decimal point.
     */
    fast_dtoa_precision
};

// FUNCTIONS
// ---------

/**
 *  Provides a decimal representation of v.
 *  The result should be interpreted as buffer * 10^(point - length).
 *
 *  Precondition:
 *    * v must be a strictly positive finite double.
 *
 *  Returns true if it succeeds, otherwise the result can not be trusted.
 *  There will be *length digits inside the buffer followed by a null terminator.
 *  If the function returns true and mode equals
 *    - fast_dtoa_shortest, then
 *      the parameter requested_digits is ignored.
 *      The result satisfies
 *          v == (double) (buffer * 10^(point - length)).
 *      The digits in the buffer are the shortest representation possible. E.g.
 *      if 0.099999999999 and 0.1 represent the same double then "1" is returned
 *      with point = 0.
 *      The last digit will be closest to the actual v. That is, even if several
 *      digits might correctly yield 'v' when read again, the buffer will contain
 *      the one closest to v.
 *    - fast_dtoa_precision, then
 *      the buffer contains requested_digits digits.
 *      the difference v - (buffer * 10^(point-length)) is closest to zero for
 *      all possible representations of requested_digits digits.
 *      If there are two values that are equally close, then FastDtoa returns
 *      false.
 *  For both modes the buffer must be large enough to hold the result.
 */
bool fast_dtoa(double d, fast_dtoa_mode mode, int requested_digits,
               char* buffer, int* length, int* decimal_point);

PYCPP_END_NAMESPACE
