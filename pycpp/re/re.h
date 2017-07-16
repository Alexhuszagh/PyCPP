//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-level regular expression methods.
 *
 *  \warning These global functions are not rentrant, since they
 *  rely on a global cache to store compiled regex objects,
 *  storing the last N (typically 100) compiled regular expression
 *  objects. If you need thread safety, you should instantiate a
 *  separate `regex_t` object per thread.
 */

#pragma once

#include <pycpp/re/match.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Search for pattern within string.
 */
match_t search(const std::string& pattern, const string_view& str);

/**
 *  \brief Match pattern within string.
 */
match_t match(const std::string& pattern, const string_view& str);

// TODO: This could use an LRU cache...

// TODO: need to implement a cache here..
// Abstract away the actual pattern/regex compilation.

// re.split(pattern, string, maxsplit=0, flags=0)
// re.findall(pattern, string, flags=0)
// re.finditer(pattern, string, flags=0)
// re.sub(pattern, repl, string, count=0, flags=0)
// re.subn(pattern, repl, string, count=0, flags=0)

/**
 *  \brief Escape all characters except for ASCII numbers and letters.
 *
 *  Escapes all ASCII non-alphanumeric characters, assuming
 *  UTF-8 source encoding.
 */
std::string escape(const string_view& str);

/**
 *  \brief Purge the regex cache.
 */
void purge();

PYCPP_END_NAMESPACE
