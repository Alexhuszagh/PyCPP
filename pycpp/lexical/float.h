//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lexical formatting for floating-point types.
 */

#pragma once

#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int MAX_FLOAT_SIZE = 60;

// DECLARATIONS
// ------------

/**
 *  \brief Generic formatter for floating-point values.
 */
struct lexical_float_formatter
{
public:
    lexical_float_formatter(float value);
    lexical_float_formatter(double value);

    // DATA
    size_t size() const;
    size_t length() const;
    const char* data() const;
    const char* c_str() const;

    string_view string() const;
    string_view escape() const;
    string_view jsonify() const;

    // CONVERSIONS
    explicit operator string_view() const;

private:
    char buffer_[MAX_FLOAT_SIZE];
    char* last_ = buffer_ + MAX_FLOAT_SIZE;
};


/**
 *  \brief Generic extractor for floating-point values.
 */
class lexical_float_extractor
{
public:
    lexical_float_extractor(const string_view& string);

    // CONVERSIONS
    explicit operator float() const;
    explicit operator double() const;

private:
    double data_;
};

PYCPP_END_NAMESPACE
