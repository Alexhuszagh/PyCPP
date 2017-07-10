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

#include <pycpp/config.h>
#include <pycpp/view/string.h>
#include <functional>
#include <vector>

PYCPP_BEGIN_NAMESPACE

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
 *  \brief Check if str starts with substr.
 */
bool startswith(const string_t& str, const string_t& sub);

/**
 *  \brief Check if str ends with substr.
 */
bool endswith(const string_t& str, const string_t& sub);

/**
 *  \brief Trim characters from left-end of string.
 */
string_t ltrim(const string_t& str, const string_t& characters = " \n\r\t");

/**
 *  \brief Trim characters from right-end of string.
 */
string_t rtrim(const string_t& str, const string_t& characters = " \n\r\t");

/**
 *  \brief Trim characters from both ends of string.
 */
string_t trim(const string_t& str, const string_t& characters = " \n\r\t");

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
string_list_t quoted_split(const string_t& str, char delimiter, char quote, char escape);

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
    string_t ltrim(const string_wrapper& characters = " \n\r\t");
    string_t rtrim(const string_wrapper& characters = " \n\r\t");
    string_t trim(const string_wrapper& characters = " \n\r\t");

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

PYCPP_END_NAMESPACE
