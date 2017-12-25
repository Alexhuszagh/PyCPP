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
    lexical_int_formatter(int8_t value) noexcept;
    lexical_int_formatter(uint8_t value) noexcept;
    lexical_int_formatter(short value) noexcept;
    lexical_int_formatter(unsigned short value) noexcept;
    lexical_int_formatter(int value) noexcept;
    lexical_int_formatter(unsigned int value) noexcept;
    lexical_int_formatter(long value) noexcept;
    lexical_int_formatter(unsigned long value) noexcept;
    lexical_int_formatter(long long value) noexcept;
    lexical_int_formatter(unsigned long long value) noexcept;
    lexical_int_formatter(const lexical_int_formatter&) noexcept;
    lexical_int_formatter& operator=(const lexical_int_formatter&) noexcept;
    lexical_int_formatter(lexical_int_formatter&&) noexcept = delete;
    lexical_int_formatter& operator=(lexical_int_formatter&&) noexcept = delete;

    // DATA
    size_t size() const noexcept;
    size_t length() const noexcept;
    const char* data() const noexcept;
    const char* c_str() const noexcept;

    string_view str() const noexcept;

    // CONVERSIONS
    explicit operator string_view() const noexcept;

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
    lexical_int_extractor(const string_view& string) noexcept;
    lexical_int_extractor(const lexical_int_extractor&) noexcept = default;
    lexical_int_extractor& operator=(const lexical_int_extractor&) noexcept = default;
    lexical_int_extractor(lexical_int_extractor&&) noexcept = default;
    lexical_int_extractor& operator=(lexical_int_extractor&&) noexcept = default;

    // DATA
    bool is_signed() const noexcept;
    bool is_unsigned() const noexcept;
    uint8_t bytes() const noexcept;

    // CONVERSIONS
    explicit operator int8_t() const noexcept;
    explicit operator uint8_t() const noexcept;
    explicit operator short() const noexcept;
    explicit operator unsigned short() const noexcept;
    explicit operator int() const noexcept;
    explicit operator unsigned int() const noexcept;
    explicit operator long() const noexcept;
    explicit operator unsigned long() const noexcept;
    explicit operator long long() const noexcept;
    explicit operator unsigned long long() const noexcept;

private:
    int64bits_t data_;
    bool minus_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lexical_int_formatter>: false_type
{};

template <>
struct is_relocatable<lexical_int_extractor>: true_type
{};

PYCPP_END_NAMESPACE
