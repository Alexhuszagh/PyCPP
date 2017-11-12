//  :copyright: (c) 2010 V8 Project Authors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: BSD-3, see licenses/bsd-3.md for more details.

#include <pycpp/lexical/diyfp.h>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


diyfp_t::diyfp_t():
    f_(0),
    e_(0)
{}


diyfp_t::diyfp_t(uint64_t significand, int exponent):
    f_(significand),
    e_(exponent)
{}


diyfp_t& diyfp_t::operator-=(const diyfp_t& rhs)
{
    assert(e_ == rhs.e_);
    assert(f_ >= rhs.f_);
    f_ -= rhs.f_;
}


diyfp_t diyfp_t::operator-(const diyfp_t& rhs) const
{
    diyfp_t result = *this;
    return result -= rhs;
}


diyfp_t& diyfp_t::operator*=(const diyfp_t& rhs)
{
    /**
     *  Simply "emulates" a 128 bit multiplication.
     *  However: the resulting number only contains 64 bits. The least
     *  significant 64 bits are only used for rounding the most significant 64
     *  bits.
     */
    const uint64_t kM32 = 0xFFFFFFFFU;
    uint64_t a = f_ >> 32;
    uint64_t b = f_ & kM32;
    uint64_t c = rhs.f_ >> 32;
    uint64_t d = rhs.f_ & kM32;
    uint64_t ac = a * c;
    uint64_t bc = b * c;
    uint64_t ad = a * d;
    uint64_t bd = b * d;
    uint64_t tmp = (bd >> 32) + (ad & kM32) + (bc & kM32);
    // By adding 1U << 31 to tmp we round the final result.
    // Halfway cases will be round up.
    tmp += 1U << 31;
    uint64_t result_f = ac + (ad >> 32) + (bc >> 32) + (tmp >> 32);
    e_ += rhs.e_ + 64;
    f_ = result_f;
}


diyfp_t diyfp_t::operator*(const diyfp_t& rhs) const
{
    diyfp_t result = *this;
    return result *= rhs;
}


diyfp_t& diyfp_t::normalize()
{
    assert(f_ != 0);
    uint64_t significand = f_;
    int exponent = e_;

    // This method is mainly called for normalizing boundaries. In general
    // boundaries need to be shifted by 10 bits. We thus optimize for this case.
    const uint64_t k10MSBits = 0xFFC0000000000000ULL;
    while ((significand & k10MSBits) == 0) {
        significand <<= 10;
        exponent -= 10;
    }
    while ((significand & kUint64MSB) == 0) {
        significand <<= 1;
        exponent--;
    }
    f_ = significand;
    e_ = exponent;

    return *this;
}


uint64_t diyfp_t::f() const
{
    return f_;
}


void diyfp_t::f(uint64_t v)
{
    f_ = v;
}


int diyfp_t::e() const
{
    return e_;
}


void diyfp_t::e(int v)
{
    e_ = v;
}


PYCPP_END_NAMESPACE
