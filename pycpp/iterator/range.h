//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief C++ iterator range adaptors (inspired by Boost).
 */

#pragma once

#include <pycpp/iterator/category.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Boost-inspired range adaptor.
 */
template <typename Iterator>
struct range
{
public:
    // MEMBER TYPES
    // ------------
    using iterator = Iterator;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using value_type = typename iterator_traits<iterator>::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = typename iterator_traits<iterator>::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    range(iterator first = iterator(), iterator last = iterator());
    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;
    bool empty() const;
    size_t distance() const;

    template <typename It>
    enable_if_t<is_random_access_iterator<It>::value, reference>
    operator[](difference_type n) const
    {
        return first_[n];
    }

private:
    iterator first_;
    iterator last_;
};

// DEFINITION
// ----------


template <typename Iterator>
range<Iterator>::range(iterator first, iterator last):
    first_(first),
    last_(last)
{}


template <typename Iterator>
auto range<Iterator>::begin() const -> iterator
{
    return first_;
}


template <typename Iterator>
auto range<Iterator>::end() const -> iterator
{
    return last_;
}


template <typename Iterator>
auto range<Iterator>::rbegin() const -> reverse_iterator
{
    return reverse_iterator(last_);
}


template <typename Iterator>
auto range<Iterator>::rend() const -> reverse_iterator
{
    return reverse_iterator(first_);
}


template <typename Iterator>
bool range<Iterator>::empty() const
{
    return first_ == last_;
}


template <typename Iterator>
size_t range<Iterator>::distance() const
{
    return PYCPP_NAMESPACE::distance(first_, last_);
}

PYCPP_END_NAMESPACE
