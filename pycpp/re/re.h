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
 *  separate `regexp_t` object per thread.
 */

#pragma once

#include <pycpp/re/match.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Search for pattern within string.
 */
match_t re_search(const string_wrapper& pattern, const string_wrapper& str);

/**
 *  \brief Match pattern within string.
 */
match_t re_match(const string_wrapper& pattern, const string_wrapper& str);

/**
 *  \brief Split string by pattern.
 */
match_groups re_split(const string_wrapper& pattern, const string_wrapper& str, size_t maxsplit = -1);

/**
 *  \brief Get all matches from pattern within a string.
 */
match_groups re_findall(const string_wrapper& pattern, const string_wrapper& str);

/**
 *  \brief Iteratively find matches within string.
 */
match_range re_finditer(const string_wrapper& pattern, const string_wrapper& str);

/**
 *  \brief Replace all matches of pattern in str with repl.
 */
std::string re_sub(const string_wrapper& pattern, const string_wrapper& repl, const string_wrapper& str);

/**
 *  \brief Escape all characters except for ASCII numbers and letters.
 *
 *  Escapes all ASCII non-alphanumeric characters, assuming
 *  UTF-8 source encoding.
 */
std::string re_escape(const string_wrapper& str);

/**
 *  \brief Purge the regex cache.
 */
void re_purge();

PYCPP_END_NAMESPACE
