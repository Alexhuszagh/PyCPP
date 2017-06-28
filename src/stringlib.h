//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief String manipulation utilities.
 */

#pragma once

#include <string>
#include <vector>

// CONSTANTS
// ---------

extern const std::string ascii_letters;
extern const std::string ascii_lowercase;
extern const std::string ascii_uppercase;
extern const std::string digits;
extern const std::string hexdigits;
extern const std::string letters;
extern const std::string lowercase;
extern const std::string octdigits;
extern const std::string punctuation;
extern const std::string printable;
extern const std::string uppercase;
extern const std::string whitespace;

// FUNCTIONS
// ---------

/**
 *  \brief Split characters by delimiters into dst.
 */
std::vector<std::string> split(const std::string& str, const std::string& sep);

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
