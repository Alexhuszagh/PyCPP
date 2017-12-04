//  :copyright: (c) Alex Huszagh 2017.
//  :license: MIT,see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPPs
 *  \brief Divide iterable into N-sized chunks.
 *
 *  TODO: document
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <functional>
#include <memory>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <typename Iterator>
struct chunked_iterator;

template <typename Iterator>
struct chunked_range;

namespace chunked_detail
{
// ALIAS
// -----

template <typename Iterator>
using iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

template <typename Iterator>
using range_key_type = typename std::conditional<
    is_input_iterator<Iterator>::value,
    iterator_value_type<Iterator>,
    std::reference_wrapper<const iterator_value_type<Iterator>>
>::type;

template <typename Iterator>
using chunked_type = std::vector<range_key_type<Iterator>>;

}   /* chunked_detail */

// DECLARATION
// -----------


/**
 *  \brief Chunked adaptor for an iterator.
 *
 *  Stores a pointer to the range object, the underlying iterator,
 *  and nullable chunk to store the items.
 */
template <typename Iterator>
struct chunked_iterator:
    std::iterator<
        std::input_iterator_tag,
        chunked_detail::chunked_type<Iterator>,
        typename std::iterator_traits<Iterator>::difference_type,
        typename std::iterator_traits<Iterator>::pointer,
        typename std::iterator_traits<Iterator>::reference
    >
{
    static_assert(!is_output_iterator<Iterator>::value, "Cannot have output iterator.");

public:
    // MEMBER TYPES
    // ------------
    using self_t = chunked_iterator<Iterator>;
    using base_t = std::iterator<
        std::input_iterator_tag,
        chunked_detail::chunked_type<Iterator>,
        typename std::iterator_traits<Iterator>::difference_type,
        typename std::iterator_traits<Iterator>::pointer,
        typename std::iterator_traits<Iterator>::reference
    >;
    using range_t = chunked_range<Iterator>;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    chunked_iterator(range_t* range = nullptr, Iterator it = Iterator(), value_type* value = nullptr);
    chunked_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    chunked_iterator(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    const_pointer operator->() const;
    const_reference operator*() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    template <typename I>
    friend struct chunked_range;

    range_t* range_;
    Iterator it_;
    std::shared_ptr<value_type> value_;
};


/**
 *  \brief Range adaptor to chunk values.
 *
 *  Creates an input iterator begin/end pair from any non-output iterator.
 */
template <typename Iterator>
struct chunked_range
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = chunked_range<Iterator>;
    using iterator = chunked_iterator<Iterator>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename iterator::const_reference;
    using pointer = typename iterator::pointer;
    using const_pointer = typename iterator::const_pointer;
    using difference_type = typename iterator::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    chunked_range(Iterator first = Iterator(), Iterator last = Iterator(), size_t size = 3);
    chunked_range(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    chunked_range(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

    // ITERATORS
    iterator begin();
    iterator end();

    // OTHER
    void swap(self_t&);

private:
    template <typename I>
    friend struct chunked_iterator;

    void chunk(iterator& first);

    iterator first_;
    iterator last_;
    size_t size_;
};


// IMPLEMENTATION
// --------------


template <typename Iterator>
chunked_iterator<Iterator>::chunked_iterator(range_t* range, Iterator it, value_type* value):
    range_(range),
    it_(it),
    value_(value)
{}


template <typename Iterator>
auto chunked_iterator<Iterator>::operator++() -> self_t&
{
    range_->chunk(*this);
    return *this;
}


template <typename Iterator>
auto chunked_iterator<Iterator>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename Iterator>
auto chunked_iterator<Iterator>::operator->() const -> const_pointer
{
    return &operator*();
}


template <typename Iterator>
auto chunked_iterator<Iterator>::operator*() const -> const_reference
{
    return *value_;
}


template <typename Iterator>
void chunked_iterator<Iterator>::swap(self_t& rhs)
{
    std::swap(range_, rhs.range_);
    std::swap(it_, rhs.it_);
    std::swap(value_, rhs.value_);
}


template <typename Iterator>
bool chunked_iterator<Iterator>::operator==(const self_t& rhs) const
{
    size_t lhs_size, rhs_size;
    lhs_size = value_ ? value_->size() : 0;
    rhs_size = rhs.value_ ? rhs.value_->size() : 0;

    return lhs_size == rhs_size && it_ == rhs.it_;
}


template <typename Iterator>
bool chunked_iterator<Iterator>::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


template <typename Iterator>
chunked_range<Iterator>::chunked_range(Iterator first, Iterator last, size_t size):
    first_(this, first, new value_type),
    last_(this, last, nullptr),
    size_(size)
{
    first_.value_->reserve(size);
    chunk(first_);
}


template <typename Iterator>
auto chunked_range<Iterator>::begin() -> iterator
{
    return first_;
}


template <typename Iterator>
auto chunked_range<Iterator>::end() -> iterator
{
    return last_;
}


template <typename Iterator>
void chunked_range<Iterator>::swap(self_t& rhs)
{
    std::swap(first_, rhs.first_);
    std::swap(last_, rhs.last_);
}


template <typename Iterator>
void chunked_range<Iterator>::chunk(iterator& first)
{
    first_.value_->clear();
    size_t size = size_;
    while (size-- && first.it_ != last_.it_) {
        first_.value_->emplace_back(*first.it_);
        ++first.it_;
    }
}

PYCPP_END_NAMESPACE
