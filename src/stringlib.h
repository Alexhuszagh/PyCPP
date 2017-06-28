//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief String manipulation utilities.
 */

#pragma once

#include "string_view.h"
#include <vector>

// ALIAS
// -----

typedef std::string string_t;
typedef std::vector<std::string> string_list_t;

// CONSTANTS
// ---------

extern const string_t ascii_letters;
extern const string_t ascii_lowercase;
extern const string_t ascii_uppercase;
extern const string_t digits;
extern const string_t hexdigits;
extern const string_t letters;
extern const string_t lowercase;
extern const string_t octdigits;
extern const string_t punctuation;
extern const string_t printable;
extern const string_t uppercase;
extern const string_t whitespace;

// FUNCTIONS
// ---------

/**
 *  \brief Split characters by delimiters into dst.
 */
string_list_t split(const std::string& str, const std::string& sep);

/**
 *  \brief Capitalize first letter of word.
 */
std::string capitalize(const std::string& str);

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
std::string expandtabs(const std::string& str, size_t tabsize = 8);

/**
 *  \brief Find in substring in string.
 *
 *  \param start        First character in str to search for substr.
 *  \param end          Last character in str to search for substr.
 *  \return             Position of substr, or -1 on failure.
 */
size_t find(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);


// OBJECTS
// -------

/**
 *  \brief Zero-copy model providing string methods via member functions.
 */
struct string_wrapper: string_view
{
    using string_view::string_view;

};
