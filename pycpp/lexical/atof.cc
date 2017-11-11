//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/atoi.h>
#include <pycpp/string/casemap.h>
#include <cmath>
#include <functional>
#include <iostream> // TODO: remove

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

// TODO: here...

// ALIAS
// -----

template <typename Int>
using atoi_function = std::function<Int(const char*, const char*&, uint8_t)>;

// TODO: this won't work... I need a definition...
template <typename Int>
extern Int atoi_(const char* first, const char*& last, uint8_t base);

// HELPERS
// -------

template <typename Float, typename Int, int Significand>
Float atof_(const char* first, const char*& last, uint8_t base, atoi_function<Int> function)
{
    // calculate the integer portion
    const char* tmp_first = first;
    const char* tmp_last = last;
    Float value = static_cast<Float>(function(tmp_first, tmp_last, base));

    // calculate the decimal portion
    if (tmp_last != last && *tmp_last == '.') {
        size_t digits = 0;
        ++tmp_last;
        do {
            tmp_first = tmp_last;
            tmp_last = std::min(last, tmp_first + Significand);
            Float fraction = static_cast<Float>(function(tmp_first, tmp_last, base));
            digits += std::distance(tmp_first, tmp_last);
            value += (fraction / std::pow(base, digits));
        } while (tmp_last != last);
    }

    // calculate the exponential portion
    if (tmp_last != last && ascii_tolower(*tmp_last) == 'e') {
    }
    // TODO: here....

    return value;
}

// FUNCTIONS
// ---------

float atof32(const char* first, const char*& last, uint8_t base)
{
    int32_t (*f)(const char*, const char*&, uint8_t) = atoi32;
    return atof_<float, int32_t, 20>(first, last, base, f);
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
    return atof_<double, int64_t, 50>(first, last, base, f);
}


double atof64(const string_view& string, uint8_t base)
{
    const char* first = string.begin();
    const char* last = string.end();
    return atof64(first, last, base);
}


PYCPP_END_NAMESPACE
