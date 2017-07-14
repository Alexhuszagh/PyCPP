//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression match object.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/view/string.h>
#include <map>
#include <memory>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct match_impl_t;
struct match_t;
struct regex_t;

// ALIAS
// -----

using match_groups = std::vector<string_view>;
using match_groupdict = std::map<string_view, string_view>;

// OBJECTS
// -------

/**
 *  \brief Match for a single regular expression.
 */
struct match_t
{
public:
    match_t(match_t&&);
    match_t & operator=(match_t&&);
    ~match_t();

    // DATA
    const string_view& group(size_t index = 0) const;
    match_groups groups() const;
    match_groupdict groupdict() const;
    size_t start(size_t index = 0) const;
    size_t end(size_t index = 0) const;
    std::pair<size_t, size_t> span(size_t index = 0) const;
    size_t pos() const;
    size_t endpos() const;
    size_t lastindex() const;
    string_view lastgroup() const;
    const string_view& string() const;
    explicit operator bool() const;

private:
    friend class regex_t;

    match_t();
    match_t(regex_t&, const string_view&, size_t, size_t);

    std::unique_ptr<match_impl_t> ptr_;
};

PYCPP_END_NAMESPACE
