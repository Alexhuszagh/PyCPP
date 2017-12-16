//  :copyright: (c) Alex Huszagh 2017.
//  :license: MIT,see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPPs
 *  \brief Divide iterable into N-sized chunks.
 *
 *  Creates an input-iterable range from an iterator pair, returning
 *  discrete N-sized chunks from the range in a custom container
 *  (by default a `vector`). If the underlying iterator pair
 *  is a forward, bidirectional, or random-access iterator, then
 *  no value copies are introduced. If the underlying iterator pair
 *  is an input iterator, then each iterator `value_type` is stored
 *  by value in the chunk.
 *
 *  The chunked iterator is a fully valid forward iterator, however,
 *  due to the expensive nature of increments, it masquerades as
 *  an input iterator, so STL algorithms prefer a one-pass approach
 *  whenever possible.
 *
 *  \code
 *      using vector = vector<int>;
 *      using range = chunked_range<typename vector::const_iterator>;
 *      vector v = {1, 2, 3, 1, 4, 2, 5};
 *      range r1(v.begin(), v.end());
 *      cout << r1.begin()->at(0) << endl;        // 1
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/sfinae/emplace_back.h>
#include <pycpp/sfinae/reserve.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/optional.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    typename Iterator,
    template <typename...> class Container = vector
>
struct chunked_iterator;

template <
    typename Iterator,
    template <typename...> class Container = vector
>
struct chunked_range;

namespace chunked_detail
{
// ALIAS
// -----

template <typename Iterator>
using iterator_value_type = typename iterator_traits<Iterator>::value_type;

template <typename Iterator>
using range_key_type = conditional_t<
    is_input_iterator<Iterator>::value,
    iterator_value_type<Iterator>,
    reference_wrapper<const iterator_value_type<Iterator>>
>;

template <typename Iterator, template <typename...> class Container = vector>
using chunked_type = Container<range_key_type<Iterator>>;

}   /* chunked_detail */

// DECLARATION
// -----------


/**
 *  \brief Chunked adaptor for an iterator.
 *
 *  Stores a pointer to the range object, the underlying iterator,
 *  and nullable chunk to store the items.
 */
template <typename Iterator, template <typename...> class Container>
struct chunked_iterator:
    iterator<
        input_iterator_tag,
        chunked_detail::chunked_type<Iterator, Container>,
        typename iterator_traits<Iterator>::difference_type,
        typename iterator_traits<Iterator>::pointer,
        typename iterator_traits<Iterator>::reference
    >
{
    static_assert(!is_output_iterator<Iterator>::value, "Cannot have output iterator.");

public:
    // MEMBER TYPES
    // ------------
    using self_t = chunked_iterator<Iterator, Container>;
    using base_t = iterator<
        input_iterator_tag,
        chunked_detail::chunked_type<Iterator, Container>,
        typename iterator_traits<Iterator>::difference_type,
        typename iterator_traits<Iterator>::pointer,
        typename iterator_traits<Iterator>::reference
    >;
    using range_t = chunked_range<Iterator, Container>;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    chunked_iterator(range_t* range = nullptr, Iterator it = Iterator(), optional<value_type>&& value = nullopt);
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
    template <typename I, template <typename...> class C>
    friend struct chunked_range;

    range_t* range_;
    Iterator it_;
    optional<value_type> value_;
};


/**
 *  \brief Range adaptor to chunk values.
 *
 *  Creates an input iterator begin/end pair from any non-output iterator.
 */
template <typename Iterator, template <typename...> class Container>
struct chunked_range
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = chunked_range<Iterator, Container>;
    using iterator = chunked_iterator<Iterator, Container>;
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
    template <typename I, template <typename...> class C>
    friend struct chunked_iterator;

    void chunk(iterator& first);

    iterator first_;
    iterator last_;
    size_t size_;
};

// IMPLEMENTATION
// --------------


template <typename Iterator, template <typename...> class C>
chunked_iterator<Iterator, C>::chunked_iterator(range_t* range, Iterator it, optional<value_type>&& value):
    range_(range),
    it_(it),
    value_(forward<optional<value_type>>(value))
{}


template <typename Iterator, template <typename...> class C>
auto chunked_iterator<Iterator, C>::operator++() -> self_t&
{
    range_->chunk(*this);
    return *this;
}


template <typename Iterator, template <typename...> class C>
auto chunked_iterator<Iterator, C>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename Iterator, template <typename...> class C>
auto chunked_iterator<Iterator, C>::operator->() const -> const_pointer
{
    return &operator*();
}


template <typename Iterator, template <typename...> class C>
auto chunked_iterator<Iterator, C>::operator*() const -> const_reference
{
    return *value_;
}


template <typename Iterator, template <typename...> class C>
void chunked_iterator<Iterator, C>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(range_, rhs.range_);
    PYCPP_NAMESPACE::swap(it_, rhs.it_);
    PYCPP_NAMESPACE::swap(value_, rhs.value_);
}


template <typename Iterator, template <typename...> class C>
bool chunked_iterator<Iterator, C>::operator==(const self_t& rhs) const
{
    size_t lhs_size, rhs_size;
    lhs_size = value_ ? value_->size() : 0;
    rhs_size = rhs.value_ ? rhs.value_->size() : 0;

    return lhs_size == rhs_size && it_ == rhs.it_;
}


template <typename Iterator, template <typename...> class C>
bool chunked_iterator<Iterator, C>::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


template <typename Iterator, template <typename...> class C>
chunked_range<Iterator, C>::chunked_range(Iterator first, Iterator last, size_t size):
    first_(this, first, value_type()),
    last_(this, last, nullopt),
    size_(size)
{
    reserve()(*first_.value_, size);
    chunk(first_);
}


template <typename Iterator, template <typename...> class C>
auto chunked_range<Iterator, C>::begin() -> iterator
{
    return first_;
}


template <typename Iterator, template <typename...> class C>
auto chunked_range<Iterator, C>::end() -> iterator
{
    return last_;
}


template <typename Iterator, template <typename...> class C>
void chunked_range<Iterator, C>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(first_, rhs.first_);
    PYCPP_NAMESPACE::swap(last_, rhs.last_);
}


template <typename Iterator, template <typename...> class C>
void chunked_range<Iterator, C>::chunk(iterator& first)
{
    first.value_->clear();
    size_t size = size_;
    while (size-- && first.it_ != last_.it_) {
        emplace_back()(*first.value_, *first.it_);
        ++first.it_;
    }
}

PYCPP_END_NAMESPACE
