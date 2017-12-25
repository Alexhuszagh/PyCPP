//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Regular expression match object.
 */

#pragma once

#include <pycpp/iterator/range.h>
#include <pycpp/misc/heap_pimpl.h>
#include <pycpp/stl/map.h>
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

// Custom wrappers for our high-level interface.
using match_groups = vector<string_wrapper>;
using match_groupdict = map<string_wrapper, string_wrapper>;
using match_range = range<match_iterator_t>;

// Aliases for RE2 types.
using match_group_indexes = std::map<std::string, int>;
using match_group_names = std::map<int, std::string>;

// OBJECTS
// -------

/**
 *  \brief Match for a single regular expression.
 */
struct match_t
{
public:
    // MEMBER TYPES
    // ------------
    using allocator_type = allocator<match_impl_t>;
    using deleter_type = unique_heap_pimpl_manager<match_impl_t, allocator_type, true>;

    // MEMBER FUNCTIONS
    // ----------------
    match_t(const match_t&) = delete;
    match_t & operator=(const match_t&) = delete;
    match_t(match_t&&) noexcept;
    match_t & operator=(match_t&&) noexcept;
    ~match_t() noexcept;

    // DATA
    const string_wrapper& group(size_t index = 0) const noexcept;
    match_groups groups() const;
    match_groupdict groupdict() const;
    size_t start(size_t index = 0) const noexcept;
    size_t end(size_t index = 0) const noexcept;
    pair<size_t, size_t> span(size_t index = 0) const noexcept;
    size_t pos() const noexcept;
    size_t endpos() const noexcept;
    size_t lastindex() const noexcept;
    string_wrapper lastgroup() const noexcept;
    const string_wrapper& str() const noexcept;

    bool operator==(const match_t&) const noexcept;
    bool operator!=(const match_t&) const noexcept;
    explicit operator bool() const noexcept;

private:
    friend struct regexp_t;
    friend struct match_iterator_t;

    match_t() noexcept;
    match_t(regexp_t&, const string_wrapper&, size_t, size_t);

    unique_ptr<match_impl_t, deleter_type> ptr_;
};


/**
 *  \brief Forward iterator over matches in a regex.
 */
struct match_iterator_t: iterator<forward_iterator_tag, match_t>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = match_iterator_t;
    using base_t = iterator<forward_iterator_tag, match_t>;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    match_iterator_t() noexcept;
    match_iterator_t(regexp_t& regex, const string_wrapper& str);
    match_iterator_t(const self_t&) noexcept;
    self_t& operator=(const self_t&) noexcept;
    match_iterator_t(self_t&&) noexcept;
    self_t& operator=(self_t&&) noexcept;
    ~match_iterator_t() noexcept;

    reference operator*() noexcept;
    const_reference operator*() const noexcept;
    pointer operator->() noexcept;
    const_pointer operator->() const noexcept;
    match_iterator_t& operator++();
    match_iterator_t operator++(int);
    bool operator==(const match_iterator_t&) const noexcept;
    bool operator!=(const match_iterator_t&) const noexcept;

    void swap(self_t&) noexcept;

private:
    shared_ptr<match_t> match_;
    regexp_t* regex_ = nullptr;
    string_wrapper str_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<match_t>: true_type
{};

template <>
struct is_relocatable<match_iterator_t>: true_type
{};

PYCPP_END_NAMESPACE
