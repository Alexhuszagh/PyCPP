//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/atoi.h>
#include <pycpp/lexical/format.h>
#include <pycpp/string/casemap.h>
#include <pycpp/string/string.h>
#include <cmath>
#include <limits>
#include <functional>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename Int>
using atoi_function = std::function<Int(const char*, const char*&, uint8_t)>;

// HELPERS
// -------

template <typename Float, typename Int, int Significand>
Float atof_(const char* first, const char*& last, uint8_t base, atoi_function<Int> function)
{
    static_assert(std::numeric_limits<Float>::is_iec559, "Must support IEC 559/IEEE 754 standard.");

    // check if it is a special IEEE754 number (NaN, INF)
    string_wrapper view(first, last);
    if (view.startswith(NAN_STRING)) {
        last = first + NAN_STRING.size() + 1;
        return std::numeric_limits<Float>::quiet_NaN();
    } else if (view.startswith(INFINITY_STRING)) {
        last = first + INFINITY_STRING.size() + 1;
        return std::numeric_limits<Float>::infinity();
    } else if (view.size() >= 1 && view[0] == '-') {
        view = string_wrapper(first+1, last);
        if (view.startswith(INFINITY_STRING)) {
            last = first + INFINITY_STRING.size() + 2;
            return -std::numeric_limits<Float>::infinity();
        }
    }

    // calculate the integer portion
    const char* tmp_first = first;
    const char* tmp_last = last;
    double value = static_cast<double>(function(tmp_first, tmp_last, base));

    // calculate the decimal portion
    if (tmp_last != last && *tmp_last == '.') {
        size_t digits = 0;
        double fraction;
        ++tmp_last;
        do {
            tmp_first = tmp_last;
            tmp_last = std::min(last, tmp_first + Significand);
            fraction = static_cast<double>(function(tmp_first, tmp_last, base));
            digits += std::distance(tmp_first, tmp_last);
            value += (fraction / std::pow(base, digits));
        } while (tmp_last != last && fraction != 0);
    }

    // calculate the exponential portion, if
    // we have an `e[+-]?\d+`.
    size_t distance = std::distance(tmp_last, last);
    if (distance > 1 && ascii_tolower(*tmp_last) == 'e') {
        ++tmp_last;
        tmp_first = tmp_last;
        tmp_last =last, tmp_first;
        double exponent = static_cast<double>(function(tmp_first, tmp_last, base));
        value *= std::pow(base, exponent);
    }

    return value;
}

// FUNCTIONS
// ---------

float atof32(const char* first, const char*& last, uint8_t base)
{
    string_wrapper view(first, last);
    if (view.startswith(NAN_STRING)) {
        // TODO: here
    } else if (view.startswith(INFINITY_STRING)) {
        // TODO: here
    }

    int32_t (*f)(const char*, const char*&, uint8_t) = atoi32;
    // a 32-bit, base-36 number can encoded max 7 digits, so
    // use 6 to ensure no possible overflow for any radix.
    return atof_<float, int32_t, 6>(first, last, base, f);
}


float atof32(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atof32(first, last, base);
}


double atof64(const char* first, const char*& last, uint8_t base)
{
    int64_t (*f)(const char*, const char*&, uint8_t) = atoi64;
    // a 64-bit, base-36 number can encoded max 13 digits, so
    // use 12 to ensure no possible overflow for any radix.
    return atof_<double, int64_t, 12>(first, last, base, f);
}


double atof64(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atof64(first, last, base);
}


PYCPP_END_NAMESPACE
