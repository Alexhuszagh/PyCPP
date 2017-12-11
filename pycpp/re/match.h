//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression match object.
 */

#pragma once

#include <pycpp/iterator/range.h>
#include <pycpp/stl/map.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/vector.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct match_impl_t;
struct match_t;
struct match_iterator_t;
struct regexp_t;

// ALIAS
// -----

using match_groups = std::vector<string_wrapper>;
using match_groupdict = std::map<string_wrapper, string_wrapper>;
using match_range = range<match_iterator_t>;

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
    const string_wrapper& group(size_t index = 0) const;
    match_groups groups() const;
    match_groupdict groupdict() const;
    size_t start(size_t index = 0) const;
    size_t end(size_t index = 0) const;
    std::pair<size_t, size_t> span(size_t index = 0) const;
    size_t pos() const;
    size_t endpos() const;
    size_t lastindex() const;
    string_wrapper lastgroup() const;
    const string_wrapper& string() const;

    bool operator==(const match_t&) const;
    bool operator!=(const match_t&) const;
    explicit operator bool() const;

private:
    friend struct regexp_t;
    friend struct match_iterator_t;

    match_t();
    match_t(regexp_t&, const string_wrapper&, size_t, size_t);

    std::unique_ptr<match_impl_t> ptr_;
};


/**
 *  \brief Forward iterator over matches in a regex.
 */
struct match_iterator_t: std::iterator<std::forward_iterator_tag, match_t>
{
public:
    // MEMBER TYPES
    // ------------
    typedef match_iterator_t self;
    typedef std::iterator<std::forward_iterator_tag, match_t> base;
    using typename base::value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------
    match_iterator_t();
    match_iterator_t(regexp_t& regex, const string_wrapper& str);
    ~match_iterator_t();
    match_iterator_t(const self&);
    self& operator=(const self&);
    match_iterator_t(self&&);
    self& operator=(self&&);

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
    match_iterator_t& operator++();
    match_iterator_t operator++(int);
    bool operator==(const match_iterator_t&) const;
    bool operator!=(const match_iterator_t&) const;

    void swap(self&);

private:
    std::shared_ptr<match_t> match_;
    regexp_t* regex_ = nullptr;
    string_wrapper str_;
};


PYCPP_END_NAMESPACE
