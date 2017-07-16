//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief C++ iterator range adaptors (inspired by Boost).
 */

#pragma once

#include <pycpp/config.h>
#include <iterator>

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
    using reverse_iterator = std::reverse_iterator<iterator>;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------
    range(iterator first = iterator(), iterator last = iterator());
    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;
    bool empty() const;
    size_t distance() const;

private:
    iterator first;
    iterator last;
};

// DEFINITION
// ----------


template <typename Iterator>
range<Iterator>::range(iterator first, iterator last):
    first(first),
    last(last)
{}


template <typename Iterator>
auto range<Iterator>::begin() const -> iterator
{
    return first;
}


template <typename Iterator>
auto range<Iterator>::end() const -> iterator
{
    return last;
}


template <typename Iterator>
auto range<Iterator>::rbegin() const -> reverse_iterator
{
    return reverse_iterator(last);
}


template <typename Iterator>
auto range<Iterator>::rend() const -> reverse_iterator
{
    return reverse_iterator(first);
}


template <typename Iterator>
bool range<Iterator>::empty() const
{
    return first == last;
}


template <typename Iterator>
size_t range<Iterator>::distance() const
{
    return std::distance(first, last);
}

PYCPP_END_NAMESPACE
