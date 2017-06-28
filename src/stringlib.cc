//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "stringlib.h"
#include "unicode.h"

// CONSTANTS
// ---------

const string_t ascii_lowercase = "abcdefghijklmnopqrstuvwxyz";
const string_t ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string_t ascii_letters = ascii_lowercase + ascii_uppercase;
const string_t digits = "0123456789";
const string_t hexdigits = "0123456789abcdefABCDEF";
const string_t lowercase = ascii_lowercase;
const string_t uppercase = ascii_uppercase;
const string_t letters = lowercase + uppercase;
const string_t octdigits = "01234567";
const string_t punctuation = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
const string_t whitespace = " \t\n\r\v\f";
const string_t printable = digits + letters + punctuation + whitespace;

// HELPERS
// -------


template <typename Iter, typename IsSep, typename Store>
static size_t split(Iter first, Iter last, IsSep issep, Store store)
{
    size_t length = 0;
    auto interval = first;
    for (; first != last && interval != last; ++interval) {
        if (issep(*interval)) {
            store(first, interval);
            ++length;
            first = interval + 1;
        }
    }

    if (first != interval) {
        store(first, interval);
        ++length;
    }

    return length;
}


// FUNCTIONS
// ---------


string_list_t split(const string_t& str, const string_t& sep)
{
    typedef typename string_t::const_iterator iterator;
    string_list_t data;

    auto issep = [&](char c)
    {
        return sep.find(c) != sep.npos;
    };
    auto store = [&](iterator first, iterator second)
    {
        return data.emplace_back(string_t(first, second));
    };

    split(str.begin(), str.end(), issep, store);

    return data;
}
