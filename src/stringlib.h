//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief String manipulation utilities.
 */

#pragma once

#include <view/string.h>
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
string_list_t split(const std::string& str, const std::string& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Same as split, except scanning in reverse order.
 */
string_list_t rsplit(const std::string& str, const std::string& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Capitalize first letter of word.
 */
std::string capitalize(const std::string& str);

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
//std::string expandtabs(const std::string& str, size_t tabsize = 8);

/**
 *  \brief Find in substring in string.
 *
 *  \param start        First character in str to search for substr.
 *  \param end          Last character in str to search for substr.
 *  \return             Position of substr, or -1 on failure.
 */
size_t find(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but find last position of substring in string.
 */
size_t rfind(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but raise error if substring not found.
 */
size_t index(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like rfind, but raise error if substring not found.
 */
size_t rindex(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Count instances of substring in string.
 */
size_t count(const std::string& str, const std::string& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Convert all uppercase characters to lowercase in string.
 *
 *  This function **this** Unicode aware.
 */
std::string lower(const std::string& str);

/**
 *  \brief Convert all uppercase characters to uppercase in string.
 *
 *  This function **this** Unicode aware.
 */
//std::string upper(const std::string& str);

// OBJECTS
// -------

/**
 *  \brief Zero-copy model providing string methods via member functions.
 */
struct string_wrapper: string_view
{
    using string_view::string_view;

    // BASE
    string_view& view();
    const string_view& view() const;

    // MODIFIERS
    void capitalize();
    void lower();
    //void upper();

    // TOKENS
    std::vector<string_wrapper> split(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;
    std::vector<string_wrapper> rsplit(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;

    // SEARCH
    size_t find(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rfind(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t index(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rindex(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t count(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
};
