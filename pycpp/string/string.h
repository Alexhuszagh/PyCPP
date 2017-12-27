//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief String manipulation utilities.
 *
 *  Provides an API similar to Python's string module and string class,
 *  allowing facile string conversions and replacement with a clean
 *  API.
 *
 *  \synopsis
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/string_view.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct string_wrapper;

// ALIAS
// -----

using string_t = string;
using string_list_t = vector<string_t>;
using string_wrapper_list_t = vector<string_wrapper>;
using split_function = function<bool(char)>;

// CONSTANTS
// ---------

// Python string constants
extern const string_t ASCII_LETTERS;
extern const string_t ASCII_LOWERCASE;
extern const string_t ASCII_UPPERCASE;
extern const string_t DIGITS;
extern const string_t HEXDIGITS;
extern const string_t LETTERS;
extern const string_t LOWERCASE;
extern const string_t OCTDIGITS;
extern const string_t PUNCTUATION;
extern const string_t PRINTABLE;
extern const string_t UPPERCASE;
extern const string_t WHITESPACE;

// Extensions
extern const string_t SPACE;
extern const string_t WINDOWS_NEWLINE;
extern const string_t POSIX_NEWLINE;
extern const string_t NEWLINE;

// OBJECTS
// -------

/**
 *  \brief Zero-copy model providing string methods via member functions.
 *
 *  These routines will handle non-null terminated strings well, however,
 *  routines like split and rsplit will not modify the underlying buffer,
 *  meaning this wrapper must be copied to a new buffer for any
 *  routine expecting a null-terminated string. Each wrapper can be
 *  explicitly converted to string.
 */
struct string_wrapper: string_view
{
    // MEMBER FUNCTIONS
    // ----------------
    string_wrapper() = default;
    string_wrapper(const string_wrapper& str) = default;
    string_wrapper& operator=(const string_wrapper& str) = default;
    string_wrapper(string_wrapper&& str) = default;
    string_wrapper& operator=(string_wrapper&& str) = default;

    string_wrapper(const string_t& str);
    string_wrapper(const string_view& str);
    string_wrapper(const string_wrapper& str, size_type pos, size_type len = npos);
    string_wrapper(const string_t& str, size_type pos, size_type len = npos);
    string_wrapper(const_pointer str);
    string_wrapper(const_pointer str, size_type n);
    string_wrapper(const_pointer first, const_pointer last);
    string_wrapper& operator=(const string_t& str);
    string_wrapper& operator=(const string_view& str);
    string_wrapper& operator=(const_pointer str);

    // BASE
    string_view& view();
    const string_view& view() const;

    // MODIFIERS
    string_t capitalize() const;
    string_t lower() const;
    string_t upper() const;
    string_t replace(const string_wrapper& sub, const string_wrapper& repl, size_t maxreplace = SIZE_MAX);
    string_t expandtabs(size_t tabsize = 8);
    string_t ltrim(const string_wrapper& characters = WHITESPACE);
    string_t rtrim(const string_wrapper& characters = WHITESPACE);
    string_t trim(const string_wrapper& characters = WHITESPACE);

    // TOKENS
    string_wrapper_list_t split(split_function is_split, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t rsplit(split_function is_split, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t split(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;
    string_wrapper_list_t rsplit(const string_wrapper& sep, size_t maxsplit = SIZE_MAX) const;
    string_t join(const string_wrapper_list_t& list);
    string_list_t quoted_split(char delimiter, char quote, char escape);

    // SEARCH
    bool startswith(const string_wrapper& sub) const;
    bool endswith(const string_wrapper& sub) const;
    size_t find(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rfind(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t index(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t rindex(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
    size_t count(const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX) const;
};

// FUNCTIONS
// ---------

/**
 *  \brief Check if str starts with substr.
 */
bool startswith(const string_wrapper& str, const string_wrapper& sub);

/**
 *  \brief Check if str ends with substr.
 */
bool endswith(const string_wrapper& str, const string_wrapper& sub);

/**
 *  \brief Trim characters from left-end of string.
 */
string_t ltrim(const string_wrapper& str, const string_wrapper& characters = WHITESPACE);

/**
 *  \brief Trim characters from right-end of string.
 */
string_t rtrim(const string_wrapper& str, const string_wrapper& characters = WHITESPACE);

/**
 *  \brief Trim characters from both ends of string.
 */
string_t trim(const string_wrapper& str, const string_wrapper& characters = WHITESPACE);

/**
 *  \brief Split characters by delimiters into dst.
 */
string_list_t split(const string_wrapper& str, split_function is_split, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Same as split, except scanning in reverse order.
 */
string_list_t rsplit(const string_wrapper& str, split_function is_split, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Split characters by delimiters into dst.
 */
string_list_t split(const string_wrapper& str, const string_wrapper& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Split characters by delimiter into dst.
 *
 *  Split string by delimiter, but allow escaped or quoted characters
 *  to be successfully included.
 *
 *  \param string           String to split
 *  \param quote            Quoting character
 *  \param escape           Escape character
 *  \param delimiters       Character delimiter
 */
string_list_t quoted_split(const string_wrapper& str, char delimiter, char quote, char escape);

/**
 *  \brief Same as split, except scanning in reverse order.
 */
string_list_t rsplit(const string_wrapper& str, const string_wrapper& sep, size_t maxsplit = SIZE_MAX);

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
string_t join(initializer_list<string_wrapper> list, const string_wrapper& sep = SPACE);
string_t join(const string_list_t& list, const string_wrapper& sep = SPACE);
string_t join(const string_wrapper_list_t& list, const string_wrapper& sep = SPACE);

/**
 *  \brief Convert tabs to spaces, using the tabsize.
 */
string_t expandtabs(const string_wrapper& str, size_t tabsize = 8);

/**
 *  \brief Find in substring in string.
 *
 *  \param start        First character in str to search for substr.
 *  \param end          Last character in str to search for substr.
 *  \return             Position of substr, or -1 on failure.
 */
size_t find(const string_wrapper& str, const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but find last position of substring in string.
 */
size_t rfind(const string_wrapper& str, const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like find, but raise error if substring not found.
 */
size_t index(const string_wrapper& str, const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Like rfind, but raise error if substring not found.
 */
size_t rindex(const string_wrapper& str, const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Count instances of substring in string.
 */
size_t count(const string_wrapper& str, const string_wrapper& sub, size_t start = 0, size_t end = SIZE_MAX);

/**
 *  \brief Capitalize first letter of word.
 */
string_t capitalize(const string_wrapper& str);

/**
 *  \brief Convert all uppercase characters to lowercase in string.
 *
 *  This function **this** Unicode aware.
 */
string_t lower(const string_wrapper& str);

/**
 *  \brief Convert all uppercase characters to uppercase in string.
 *
 *  This function **this** Unicode aware.
 */
string_t upper(const string_wrapper& str);

/**
 *  \brief Replace maxreplace instances of sub with repl in str.
 */
string_t replace(const string_wrapper& str, const string_wrapper& sub, const string_wrapper& repl, size_t maxreplace = SIZE_MAX);

PYCPP_END_NAMESPACE
