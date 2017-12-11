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

using regex_cache = lru_cache<std::string, regexp_t>;
regex_cache REGEX_CACHE(REGEX_CACHE_SIZE);

// HELPERS
// -------


/**
 *  Compile regex if not previously present in the cache.
 */
static regexp_t& compile(const string_wrapper& view)
{
    std::string pattern(view);
    auto it = REGEX_CACHE.find(pattern);
    if (it == REGEX_CACHE.end()) {
        it = REGEX_CACHE.insert(pattern, regexp_t(pattern)).first;
    }

    return *it;
}

// FUNCTIONS
// ---------


match_t re_search(const string_wrapper& pattern, const string_wrapper& str)
{
    return compile(pattern).search(str);
}


match_t re_match(const string_wrapper& pattern, const string_wrapper& str)
{
    return compile(pattern).match(str);
}


match_groups re_findall(const string_wrapper& pattern, const string_wrapper& str)
{
    return compile(pattern).findall(str);
}


match_range re_finditer(const string_wrapper& pattern, const string_wrapper& str)
{
    return compile(pattern).finditer(str);
}


match_groups re_split(const string_wrapper& pattern, const string_wrapper& str, size_t maxsplit)
{
    return compile(pattern).split(str);
}


std::string re_sub(const string_wrapper& pattern, const string_wrapper& repl, const string_wrapper& str)
{
    return compile(pattern).sub(repl, str);
}


std::string re_escape(const string_wrapper& str)
{
    std::string output;
    output.reserve(str.size() * 2 + 1);

    for (const char c: str) {
        if ((c >= 0 && c <= 47) ||      // Null - /
            (c >= 58 && c <= 64) ||     // : - @
            (c >= 91 && c <= 96) ||     // [ - `]
            (c >= 123 && c <= 126)) {   // ( - ~
            output.push_back('\\');
        }
        output.push_back(c);
    }

    output.shrink_to_fit();
    return output;
}


void re_purge()
{
    REGEX_CACHE.clear();
}


PYCPP_END_NAMESPACE
