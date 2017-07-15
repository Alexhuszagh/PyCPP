//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/cache/lru.h>
#include <pycpp/re/re.h>
#include <pycpp/re/regex.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

// TODO: Need an LRU cache and hashmap

match_t search(const string_view& pattern, const string_view& str)
{
    // TODO: need to implement the cache
    return regex_t(pattern).search(str);
}


match_t match(const string_view& pattern, const string_view& str)
{
    // TODO: need to implement the cache
    return regex_t(pattern).match(str);
}

PYCPP_END_NAMESPACE
