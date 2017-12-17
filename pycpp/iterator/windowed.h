//  :copyright: (c) Alex Huszagh 2017.
//  :license: MIT,see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Create a sliding window of size-N from an iterable.
 *
 *  Creates an input-iterable range from an iterator pair, returning
 *  a window N-sized chunks from the range in a custom container
 *  (by default a `deque`). If the underlying iterator pair
 *  is a forward, bidirectional, or random-access iterator, then
 *  no value copies are introduced. If the underlying iterator pair
 *  is an input iterator, then each iterator `value_type` is stored
 *  by value in the chunk.
 *
 *  The window slides over the [first, last) range, simulating a window
 *  over the range. When incrementing the window, the first item in the
 *  window is popped off the window, and the iterator value is appended
 *  to the window. Therefore, a container that supports the efficient
 *  removal of items from the front and insertion of items at the back,
 *  such as a deque or list, is highly preferrable.
 *
 *  The windowed iterator is a fully valid forward iterator, however,
 *  due to the expensive nature of increments, it masquerades as
 *  an input iterator, so STL algorithms prefer a one-pass approach
 *  whenever possible.
 *
 *  \code
 *      using vector = vector<int>;
 *      using range = windowed_range<typename vector::const_iterator>;
 *      vector v = {1, 2, 3, 1, 4, 2, 5};
 *      range r1(v.begin(), v.end());
 *      cout << r1.begin()->at(0) << endl;        // 1
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/sfinae/emplace_back.h>
#include <pycpp/sfinae/pop_front.h>
#include <pycpp/sfinae/reserve.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/optional.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    typename Iterator,
    template <typename...> class Container = deque
>
struct windowed_iterator;

template <
    typename Iterator,
    template <typename...> class Container = deque
>
struct windowed_range;

namespace windowed_detail
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
using windowed_type = Container<range_key_type<Iterator>>;

}   /* windowed_detail */

// DECLARATION
// -----------

/**
 *  \brief Windowed adaptor for an iterator.
 *
 *  Stores a pointer to the range object, the underlying iterator,
 *  and nullable chunk to store the items.
 */
template <typename Iterator, template <typename...> class Container>
struct windowed_iterator:
    iterator<
        input_iterator_tag,
        windowed_detail::windowed_type<Iterator, Container>,
        typename iterator_traits<Iterator>::difference_type,
        typename iterator_traits<Iterator>::pointer,
        typename iterator_traits<Iterator>::reference
    >
{
    static_assert(!is_output_iterator<Iterator>::value, "Cannot have output iterator.");

public:
    // MEMBER TYPES
    // ------------
    using self_t = windowed_iterator<Iterator, Container>;
    using base_t = iterator<
        input_iterator_tag,
        windowed_detail::windowed_type<Iterator, Container>,
        typename iterator_traits<Iterator>::difference_type,
        typename iterator_traits<Iterator>::pointer,
        typename iterator_traits<Iterator>::reference
    >;
    using range_t = windowed_range<Iterator, Container>;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    windowed_iterator(range_t* range = nullptr, Iterator it = Iterator(), optional<value_type>&& value = nullopt);
    windowed_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    windowed_iterator(self_t&&) = default;
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
    friend struct windowed_range;

    range_t* range_;
    Iterator it_;
    optional<value_type> value_;
};


/**
 *  \brief Range adaptor to group values in a sliding window.
 *
 *  Creates an input iterator begin/end pair from any non-output iterator.
 */
template <typename Iterator, template <typename...> class Container>
struct windowed_range
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = windowed_range<Iterator, Container>;
    using iterator = windowed_iterator<Iterator, Container>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename iterator::const_reference;
    using pointer = typename iterator::pointer;
    using const_pointer = typename iterator::const_pointer;
    using difference_type = typename iterator::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    windowed_range(Iterator first = Iterator(), Iterator last = Iterator(), size_t size = 2);
    windowed_range(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    windowed_range(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

    // ITERATORS
    iterator begin();
    iterator end();

    // OTHER
    void swap(self_t&);

private:
    template <typename I, template <typename...> class C>
    friend struct windowed_iterator;

    void fill(iterator& first, size_t size);

    iterator first_;
    iterator last_;
};


/**
 *  \brief Convenience class for a pairwise window.
 */
template <typename Iterator, template <typename...> class Container>
struct pairwise_range: windowed_range<Iterator, Container>
{
    // MEMBER TYPES
    // ------------
    using self_t = pairwise_range<Iterator, Container>;
    using base_t = windowed_range<Iterator, Container>;

    // MEMBER FUNCTIONS
    // ----------------
    pairwise_range(Iterator first = Iterator(), Iterator last = Iterator());
    pairwise_range(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    pairwise_range(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
};


// IMPLEMENTATION
// --------------


template <typename Iterator, template <typename...> class C>
windowed_iterator<Iterator, C>::windowed_iterator(range_t* range, Iterator it, optional<value_type>&& value):
    range_(range),
    it_(it),
    value_(forward<optional<value_type>>(value))
{}


template <typename Iterator, template <typename...> class C>
auto windowed_iterator<Iterator, C>::operator++() -> self_t&
{
    if (it_ == range_->last_.it_) {
        value_->clear();
    } else {
        pop_front()(*value_);
        emplace_back()(*value_, *it_);
        ++it_;
    }
    return *this;
}


template <typename Iterator, template <typename...> class C>
auto windowed_iterator<Iterator, C>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename Iterator, template <typename...> class C>
auto windowed_iterator<Iterator, C>::operator->() const -> const_pointer
{
    return &operator*();
}


template <typename Iterator, template <typename...> class C>
auto windowed_iterator<Iterator, C>::operator*() const -> const_reference
{
    return *value_;
}


template <typename Iterator, template <typename...> class C>
void windowed_iterator<Iterator, C>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(it_, rhs.it_);
    PYCPP_NAMESPACE::swap(value_, rhs.value_);
}


template <typename Iterator, template <typename...> class C>
bool windowed_iterator<Iterator, C>::operator==(const self_t& rhs) const
{
    size_t lhs_size, rhs_size;
    lhs_size = value_ ? value_->size() : 0;
    rhs_size = rhs.value_ ? rhs.value_->size() : 0;

    return lhs_size == rhs_size && it_ == rhs.it_;
}


template <typename Iterator, template <typename...> class C>
bool windowed_iterator<Iterator, C>::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


template <typename Iterator, template <typename...> class C>
windowed_range<Iterator, C>::windowed_range(Iterator first, Iterator last, size_t size):
    first_(this, first, value_type()),
    last_(this, last, nullopt)
{
    fill(first_, size);
}


template <typename Iterator, template <typename...> class C>
auto windowed_range<Iterator, C>::begin() -> iterator
{
    return first_;
}


template <typename Iterator, template <typename...> class C>
auto windowed_range<Iterator, C>::end() -> iterator
{
    return last_;
}


template <typename Iterator, template <typename...> class C>
void windowed_range<Iterator, C>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(first_, rhs.first_);
    PYCPP_NAMESPACE::swap(last_, rhs.last_);
}


template <typename Iterator, template <typename...> class C>
void windowed_range<Iterator, C>::fill(iterator& first, size_t size)
{
    reserve()(*first.value_, size);
    first.value_->clear();
    while (size-- && first.it_ != last_.it_) {
        emplace_back()(*first.value_, *first.it_);
        ++first.it_;
    }
}


template <typename Iterator, template <typename...> class C>
pairwise_range<Iterator, C>::pairwise_range(Iterator first, Iterator last):
    base_t(first, last, 2)
{}

PYCPP_END_NAMESPACE
