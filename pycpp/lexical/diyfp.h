//  :copyright: (c) 2010 V8 Project Authors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: BSD-3, see licenses/bsd-3.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief "Do it yourself" floating point number.
 */

#pragma once

#include <pycpp/config.h>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  This "Do It Yourself Floating Point" class implements a floating-point number
 *  with a uint64 significand and an int exponent. Normalized diyfp_t numbers will
 *  have the most significant bit of the significand set.
 *  Multiplication and Subtraction do not normalize their results.
 *  diyfp_t are not designed to contain special doubles (NaN and Infinity).
 */
class diyfp_t
{
public:
    static const int kSignificandSize = 64;

    diyfp_t();
    diyfp_t(uint64_t significand, int exponent);

    /**
    *   this = this - other.
    *   The exponents of both numbers must be the same and the significand of this
    *   must be bigger than the significand of other.
    *   The result will not be normalized.
    */
    diyfp_t& operator-=(const diyfp_t& rhs);

    /**
     *  Returns this - other.
     *  The exponents of both numbers must be the same and this must be bigger
     *  than other. The result will not be normalized.
     */
    diyfp_t operator-(const diyfp_t& rhs) const;

    /**
     *  this = this * other.
     */
    diyfp_t& operator*=(const diyfp_t& rhs);

    /**
     *  returns a * b;
     */
    diyfp_t operator*(const diyfp_t& rhs) const;

    diyfp_t& normalize();

    uint64_t f() const;
    void f(uint64_t v);
    int e() const;
    void e(int v);

private:
    static const uint64_t kUint64MSB = 0x8000000000000000ULL;

    uint64_t f_;
    int e_;
};

PYCPP_END_NAMESPACE
