//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "stringlib.h"


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
