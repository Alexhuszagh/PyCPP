//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for integer types.
 */

#pragma once

#include <pycpp/stl/limits.h>
#include <pycpp/stl/string_view.h>
#include <stdint.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int MAX_INTEGER_SIZE = numeric_limits<unsigned long long>::digits10 + 3;

// DECLARATIONS
// ------------

/**
 *  \brief Union for both signed and unsigned 64-bit integers.
 */
union int64bits_t
{
    int64_t i;
    uint64_t u;
};

/**
 *  \brief Generic formatter for integer values.
 */
struct lexical_int_formatter
{
public:
    lexical_int_formatter(int8_t value);
    lexical_int_formatter(uint8_t value);
    lexical_int_formatter(short value);
    lexical_int_formatter(unsigned short value);
    lexical_int_formatter(int value);
    lexical_int_formatter(unsigned int value);
    lexical_int_formatter(long value);
    lexical_int_formatter(unsigned long value);
    lexical_int_formatter(long long value);
    lexical_int_formatter(unsigned long long value);

    // DATA
    size_t size() const;
    size_t length() const;
    const char* data() const;
    const char* c_str() const;

    string_view string() const;

    // CONVERSIONS
    explicit operator string_view() const;

private:
    char buffer_[MAX_INTEGER_SIZE];
    char* last_ = buffer_ + MAX_INTEGER_SIZE;
};


/**
 *  \brief Generic extractor for integer values.
 */
class lexical_int_extractor
{
public:
    lexical_int_extractor(const string_view& string);

    // DATA
    bool is_signed() const;
    bool is_unsigned() const;
    uint8_t bytes() const;

    // CONVERSIONS
    explicit operator int8_t() const;
    explicit operator uint8_t() const;
    explicit operator short() const;
    explicit operator unsigned short() const;
    explicit operator int() const;
    explicit operator unsigned int() const;
    explicit operator long() const;
    explicit operator unsigned long() const;
    explicit operator long long() const;
    explicit operator unsigned long long() const;

private:
    int64bits_t data_;
    bool minus_;
};

PYCPP_END_NAMESPACE
