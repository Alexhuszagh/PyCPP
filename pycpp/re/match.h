//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression match object.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/view/string.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct match_impl_t;
struct match_t;
struct regex_t;

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
    // expand
    const string_view& group(size_t index = 0) const;
    // groups
    // groupdict
    size_t start(size_t index = 0) const;
    size_t end(size_t index = 0) const;
    // span
    // pos
    // endpos
    // lastindex
    // lastgroup
    // re
    // string
    explicit operator bool() const;

private:
    friend class regex_t;

    match_t();
    match_t(regex_t&, const string_view&);

    std::unique_ptr<match_impl_t> ptr_;
};

PYCPP_END_NAMESPACE
