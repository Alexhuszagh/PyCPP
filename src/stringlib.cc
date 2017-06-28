//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "stringlib.h"
#include "unicode.h"

// CONSTANTS
// ---------

const std::string ascii_letters = ascii_lowercase + ascii_uppercase;
const std::string ascii_lowercase = "abcdefghijklmnopqrstuvwxyz";
const std::string ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string digits = "0123456789";
const std::string hexdigits = "0123456789abcdefABCDEF";
const std::string letters = lowercase + uppercase;
const std::string lowercase = ascii_lowercase;
const std::string octdigits = "01234567";
const std::string punctuation = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
const std::string printable = digits + letters + punctuation + whitespace;
const std::string uppercase = ascii_uppercase;
const std::string whitespace = " \t\n\r\v\f";

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


std::vector<std::string> split(const std::string& str, const std::string& sep)
{
    typedef typename std::string::const_iterator iterator;
    std::vector<std::string> data;

    auto issep = [&](char c)
    {
        return sep.find(c) != sep.npos;
    };
    auto store = [&](iterator first, iterator second)
    {
        return data.emplace_back(std::string(first, second));
    };

    split(str.begin(), str.end(), issep, store);

    return data;
}
