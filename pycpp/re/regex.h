//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression object.
 */

#pragma once

#include <pycpp/re/match.h>
#include <pycpp/stl/map.h>

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
    // MEMBER FUNCTIONS
    // ----------------
    regexp_t() = delete;
    regexp_t(const string_wrapper& view);
    regexp_t(regexp_t&&);
    regexp_t & operator=(regexp_t&&);
    ~regexp_t();

    match_t search(const string_wrapper& str, size_t start = 0, size_t endpos = string_wrapper::npos);
    match_t match(const string_wrapper& str, size_t start = 0, size_t endpos = string_wrapper::npos);
    match_groups split(const string_wrapper& str, size_t maxsplit = -1);
    match_groups findall(const string_wrapper& str, size_t start = 0, size_t endpos = string_wrapper::npos);
    match_range finditer(const string_wrapper& str, size_t start = 0, size_t endpos = string_wrapper::npos);
    std::string sub(const string_wrapper& repl, const string_wrapper& str);
    size_t groups() const;
    const match_group_indexes& group_indexes() const;
    const match_group_names& group_names() const;

private:
    friend struct match_t;

    unique_ptr<regex_impl_t> ptr_;
};


PYCPP_END_NAMESPACE
