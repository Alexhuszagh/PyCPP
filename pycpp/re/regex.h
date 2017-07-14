//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression object.
 */

#pragma once

#include <pycpp/re/match.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct regex_impl_t;
struct regex_t;

// OBJECTS
// -------

/**
 *  \brief Regular expression object.
 */
struct regex_t
{
public:
    regex_t() = delete;
    regex_t(const string_view& view);
    regex_t(regex_t&&);
    regex_t & operator=(regex_t&&);
    ~regex_t();

    match_t search(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    match_t match(const string_view& str, size_t start = 0, size_t endpos = string_view::npos);
    // split
    // findall
    // finditer
    // sub
    // subn
    // groups
    // groupindex

private:
    friend class match_t;

    std::unique_ptr<regex_impl_t> ptr_;
};


PYCPP_END_NAMESPACE
