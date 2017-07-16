//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression object.
 */

#pragma once

#include <pycpp/re/match.h>
#include <map>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct regex_impl_t;
struct regexp_t;

// OBJECTS
// -------

/**
 *  \brief Regular expression object.
 */
struct regexp_t
{
public:
    regexp_t() = delete;
    regexp_t(const string_view& view);
    regexp_t(regexp_t&&);
    regexp_t & operator=(regexp_t&&);
    ~regexp_t();

    match_t search(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    match_t match(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    match_groups split(const string_view& str, size_t maxsplit = -1);
    match_groups findall(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    match_range finditer(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    std::string sub(const string_view& repl, const string_view& str);
    size_t groups() const;
    const std::map<std::string, int>& groupindex() const;

private:
    friend class match_t;

    std::unique_ptr<regex_impl_t> ptr_;
};


PYCPP_END_NAMESPACE
