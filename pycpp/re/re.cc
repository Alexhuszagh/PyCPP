//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/cache/lru.h>
#include <pycpp/re/re.h>
#include <pycpp/re/regex.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

#define REGEX_CACHE_SIZE 100

// GLOBALS
// -------

using regex_cache = lru_cache<std::string, regex_t>;
regex_cache REGEX_CACHE(REGEX_CACHE_SIZE);

// HELPERS
// -------


/**
 *  Compile regex if not previously present in the cache.
 */
static regex_t& compile(const std::string& pattern)
{
    auto it = REGEX_CACHE.find(pattern);
    if (it == REGEX_CACHE.end()) {
        REGEX_CACHE.insert(pattern, regex_t(pattern));
    }

    return *it;
}

// FUNCTIONS
// ---------


match_t search(const std::string& pattern, const string_view& str)
{
    return compile(pattern).search(str);
}


match_t match(const std::string& pattern, const string_view& str)
{
    return compile(pattern).match(str);
}


void purge()
{
    REGEX_CACHE.clear();
}


PYCPP_END_NAMESPACE
