//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atoi.h>
#include <pycpp/lexical/precise_float.h>
#include <pycpp/lexical/table.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/stdexcept.h>
#include <ctype.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

// RANGE

inline bool is_valid_num(char c, char upper)
{
    return c >= '0' && c <= upper;
}


inline bool is_valid_alnum(char c, char upper)
{
    c = ::toupper(c);
    return is_valid_num(c, '9') || (c >= 'A' && c <= upper);
}


bool is_valid_digit(char c, uint8_t base)
{
    if (base <= 10) {
        return is_valid_num(c, BASEN[base-1]);
    } else {
        return is_valid_alnum(c, BASEN[base-1]);
    }
}


// GENERIC

template <typename Int>
static Int atoi_num(const char* first, const char*& last, uint8_t base)
{
    // Generic itao for bases of <= 10, where only
    // numerical characters are used.

    Int value = 0;
    char upper = BASEN[base-1];

    while (first < last && is_valid_num(first[0], upper)) {
        value *= base;
        value += *first++ - '0';
    }
    last = first;
    return value;
}


template <typename Int>
static Int atoi_alnum(const char* first, const char*& last, uint8_t base)
{
    // Generic itao for bases of > 10, where
    // alphabetical characters are also used.

    Int value = 0;
    char upper = BASEN[base-1];

    while (first < last && is_valid_alnum(first[0], upper)) {
        value *= base;
        char c = *first++;
        if (c <= '9') {
            value += c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            value += c - 'A' + 10;
        } else {
            assert(c >= 'a' && c <= 'z');
            value += c - 'a' + 10;
        }
    }
    last = first;
    return value;
}


template <typename Int>
static Int atoi_impl(const char* first, const char*& last, uint8_t base)
{
    if (base < 2 || base > 36) {
        throw invalid_argument("Numerical base must be from 2-36");
    } else if (base <= 10) {
        return atoi_num<Int>(first, last, base);
    } else {
        return atoi_alnum<Int>(first, last, base);
    }
}


template <typename Int>
Int atoi_(const char* first, const char*& last, uint8_t base)
{
    if (first == last) {
        return Int(0);
    } else if (first[0] == '+') {
        return atoi_impl<Int>(first+1, last, base);
    } else if (first[0] == '-') {
        return -atoi_impl<Int>(first+1, last, base);
    } else {
        return atoi_impl<Int>(first, last, base);
    }
}


// FUNCTIONS
// ---------

uint8_t atou8(const char* first, const char*& last, uint8_t base)
{
    return atoi_<uint8_t>(first, last, base);
}


uint8_t atou8(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atou8(first, last, base);
}


int8_t atoi8(const char* first, const char*& last, uint8_t base)
{
    return atoi_<int8_t>(first, last, base);
}


int8_t atoi8(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atoi8(first, last, base);
}


uint16_t atou16(const char* first, const char*& last, uint8_t base)
{
    return atoi_<uint16_t>(first, last, base);
}


uint16_t atou16(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atou16(first, last, base);
}


int16_t atoi16(const char* first, const char*& last, uint8_t base)
{
    return atoi_<int16_t>(first, last, base);
}


int16_t atoi16(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atoi16(first, last, base);
}


uint32_t atou32(const char* first, const char*& last, uint8_t base)
{
    return atoi_<uint32_t>(first, last, base);
}


uint32_t atou32(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atou32(first, last, base);
}


int32_t atoi32(const char* first, const char*& last, uint8_t base)
{
    return atoi_<int32_t>(first, last, base);
}


int32_t atoi32(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atoi32(first, last, base);
}


uint64_t atou64(const char* first, const char*& last, uint8_t base)
{
    return atoi_<uint64_t>(first, last, base);
}


uint64_t atou64(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atou64(first, last, base);
}


int64_t atoi64(const char* first, const char*& last, uint8_t base)
{
    return atoi_<int64_t>(first, last, base);
}


int64_t atoi64(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atoi64(first, last, base);
}


// Compatiblility for `ftoa.cc`
// We can have overflow with any integer type for
// `float` and `double`, so we need to use `double`
// for our str-to-int conversions in `ftoa.cc`.
precise_float_t atoi_precise_float(const char* first, const char*& last, uint8_t base)
{
    return atoi_<precise_float_t>(first, last, base);
}


PYCPP_END_NAMESPACE
