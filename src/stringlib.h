//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief String manipulation utilities.
 *
 *  Provides an API similar to Python's string module and string class,
 *  allowing facile string conversions and replacement with a clean
 *  API.
 */

#pragma once

#include <view/string.h>
#include <functional>
#include <vector>

// FORWARD
// -------

class string_wrapper;

// ALIAS
// -----

typedef std::string string_t;
typedef std::vector<std::string> string_list_t;
typedef std::vector<string_wrapper> string_wrapper_list_t;
typedef std::function<bool(char)> split_function;

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
string_list_t split(const string_t& str, split_function is_split, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Same as split, except scanning in reverse order.
 */
string_list_t rsplit(const string_t& str, split_function is_split, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Split characters by delimiters into dst.
 */
string_list_t split(const string_t& str, const string_t& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Same as split, except scanning in reverse order.
 */
string_list_t rsplit(const string_t& str, const string_t& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
string_t join(const string_list_t& list, const std::string& sep = " ");

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
string_t expandtabs(const string_t& str, size_t tabsize = 8);

/**
 *  \brief Find in substring in string.
 *
 *  \param start        First character in str to search for substr.
 *  \param end          Last character in str to search for substr.
 *  \return             Position of substr, or -1 on failure.
 */
size_t find(const string_t& str, const string_t& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but find last position of substring in string.
 */
size_t rfind(const string_t& str, const string_t& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but raise error if substring not found.
 */
size_t index(const string_t& str, const string_t& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like rfind, but raise error if substring not found.
 */
size_t rindex(const string_t& str, const string_t& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Count instances of substring in string.
 */
size_t count(const string_t& str, const string_t& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Capitalize first letter of word.
 */
string_t capitalize(const string_t& str);

/**
 *  \brief Convert all uppercase characters to lowercase in string.
 *
 *  This function **this** Unicode aware.
 */
string_t lower(const string_t& str);

/**
 *  \brief Convert all uppercase characters to uppercase in string.
 *
 *  This function **this** Unicode aware.
 */
string_t upper(const string_t& str);

/**
 *  \brief Replace maxreplace instances of sub with repl in str.
 */
string_t replace(const string_t& str, const string_t& sub, const string_t& repl, size_t maxreplace = SIZE_MAX);

// OBJECTS
// -------

/**
 *  \brief Zero-copy model providing string methods via member functions.
 *
 *  These routines will handle non-null terminated strings well, however,
 *  routines like split and rsplit will not modify the underlying buffer,
 *  meaning this wrapper must be copied to a new buffer for any
 *  routine expecting a null-terminated string. Each wrapper can be
 *  explicitly converted to std::string.
 */
struct string_wrapper: string_view
{
    using string_view::string_view;

    // BASE
    string_view& view();
    const string_view& view() const;

    // MODIFIERS
    string_t capitalize() const;
    string_t lower() const;
    string_t upper() const;
    string_t replace(const string_wrapper& sub, const string_wrapper& repl, size_t maxreplace = SIZE_MAX);
    string_t expandtabs(size_t tabsize = 8);

    // TOKENS
    string_wrapper_list_t split(split_function is_split, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t rsplit(split_function is_split, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t split(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t rsplit(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;
    string_t join(const string_wrapper_list_t& list);

    // SEARCH
    size_t find(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rfind(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t index(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rindex(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t count(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
};
