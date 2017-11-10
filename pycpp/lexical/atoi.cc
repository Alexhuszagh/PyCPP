//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atoi.h>
#include <limits>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr char NUMBERS_BASEN[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// HELPERS
// -------

// RANGE

inline bool is_valid_num(char c, char upper)
{
    return c >= '0' && c <= upper;
}


inline bool is_valid_alnum(char c, char upper)
{
    return is_valid_num(c, '9') || (c >= 'A' && c <= upper);
}

// GENERIC

template <typename Int>
static Int itoa_num(const char* first, const char*& last, uint8_t base)
{
    // Generic itao for bases of <= 10, where only
    // numerical characters are used.

    Int value = 0;
    char upper = NUMBERS_BASEN[base];

    while (first < last && is_valid_num(first[0], upper)) {
        value *= base;
        value += *first++ - '0';
    }
    last = first;
    return value;
}


template <typename Int>
static Int itoa_alnum(const char* first, const char*& last, uint8_t base)
{
    // Generic itao for bases of > 10, where
    // alphabetical characters are also used.

    Int value = 0;
    char upper = NUMBERS_BASEN[base];

    while (first < last && is_valid_alnum(first[0], upper)) {
        value *= base;
        value += *first++ - '0';
    }
    last = first;
    return value;
}


template <typename Int>
static Int itoa_impl(const char* first, const char*& last, uint8_t base)
{
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Numerical base must be from 2-36");
    } else if (base <= 10) {
        return itoa_num<Int>(first, last, base);
    } else {
        return itoa_alnum<Int>(first, last, base);
    }
}


template <typename Int>
static Int itoa(const char* first, const char*& last, uint8_t base)
{
    if (first == last) {
        return Int(0);
    } else if (first[0] == '-') {
        return -itoa_impl<Int>(first+1, last, base);
    } else {
        return itoa_impl<Int>(first, last, base);
    }
}


// FUNCTIONS
// ---------

uint8_t u8toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<uint8_t>(first, last, base);
}


uint8_t u8toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return u8toa(first, last, base);
}


int8_t i8toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<int8_t>(first, last, base);
}


int8_t i8toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return i8toa(first, last, base);
}


uint16_t u16toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<uint16_t>(first, last, base);
}


uint16_t u16toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return u16toa(first, last, base);
}


int16_t i16toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<int16_t>(first, last, base);
}


int16_t i16toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return i16toa(first, last, base);
}


uint32_t u32toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<uint32_t>(first, last, base);
}


uint32_t u32toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return u32toa(first, last, base);
}


int32_t i32toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<int32_t>(first, last, base);
}


int32_t i32toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return i32toa(first, last, base);
}


uint64_t u64toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<uint64_t>(first, last, base);
}


uint64_t u64toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return u64toa(first, last, base);
}


int64_t i64toa(const char* first, const char*& last, uint8_t base)
{
    return itoa<int64_t>(first, last, base);
}


int64_t i64toa(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return i64toa(first, last, base);
}


PYCPP_END_NAMESPACE
