//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance Python xrange/range implementation.
 */

#pragma once

#include <pycpp/iterator/range.h>
#include <pycpp/misc/ordering.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/utility.h>
#include <math.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief xrange iterator adaptor.
 */
template <typename T>
struct xrange_iterator: iterator<random_access_iterator_tag, T>
{
    // MEMBER TYPES
    // ------------
    using self_t = xrange_iterator<T>;
    using base_t = iterator<random_access_iterator_tag, T>;
    using typename base_t::value_type;
    using typename base_t::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    xrange_iterator(T value = 0, T step = 1);
    xrange_iterator(const xrange_iterator&) = default;
    xrange_iterator& operator=(const xrange_iterator&) = default;
    xrange_iterator(xrange_iterator&&) = default;
    xrange_iterator& operator=(xrange_iterator&&) = default;

    // RELATIONAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;
    bool operator<(const self_t&) const;
    bool operator<=(const self_t&) const;
    bool operator>(const self_t&) const;
    bool operator>=(const self_t&) const;

    // INCREMENTORS
    self_t& operator++();
    self_t operator++(int);
    self_t& operator--();
    self_t operator--(int);
    self_t& operator+=(difference_type);
    self_t& operator-=(difference_type);
    self_t operator+(difference_type);
    self_t operator-(difference_type);
    difference_type operator-(const self_t&);
    reference operator[](difference_type) const;

    // DEREFERENCE
    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;

    // OTHER
    void swap(self_t&);

private:
    T value = 0;
    T step = 1;
};

// DEFINITION
// ----------


template <typename T>
xrange_iterator<T>::xrange_iterator(T value, T step):
    value(value),
    step(step)
{}


template <typename T>
bool xrange_iterator<T>::operator==(const self_t& rhs) const
{
    return value == rhs.value;
}


template <typename T>
bool xrange_iterator<T>::operator!=(const self_t& rhs) const
{
    return ordering::not_equal_to(*this, rhs);
}


template <typename T>
bool xrange_iterator<T>::operator<(const self_t& rhs) const
{
    return value < rhs.value;
}


template <typename T>
bool xrange_iterator<T>::operator<=(const self_t& rhs) const
{
    return ordering::less_equal(*this, rhs);
}


template <typename T>
bool xrange_iterator<T>::operator>(const self_t& rhs) const
{
    return ordering::greater(*this, rhs);
}


template <typename T>
bool xrange_iterator<T>::operator>=(const self_t& rhs) const
{
    return ordering::greater_equal(*this, rhs);
}


template <typename T>
auto xrange_iterator<T>::operator++() -> self_t&
{
    value += step;
    return *this;
}


template <typename T>
auto xrange_iterator<T>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename T>
auto xrange_iterator<T>::operator--() -> self_t&
{
    value -= step;
    return *this;
}


template <typename T>
auto xrange_iterator<T>::operator--(int) -> self_t
{
    self_t copy(*this);
    operator--();
    return copy;
}


template <typename T>
auto xrange_iterator<T>::operator+=(difference_type n) -> self_t&
{
    value += step * n;
    return *this;
}


template <typename T>
auto xrange_iterator<T>::operator-=(difference_type n) -> self_t&
{
    value -= step * n;
    return *this;
}


template <typename T>
auto xrange_iterator<T>::operator+(difference_type n) -> self_t
{
    self_t copy(*this);
    copy += n;
    return copy;
}


template <typename T>
auto xrange_iterator<T>::operator-(difference_type n) -> self_t
{
    self_t copy(*this);
    copy -= n;
    return copy;
}


template <typename T>
auto xrange_iterator<T>::operator-(const self_t& rhs) -> difference_type
{
    return (value - rhs.value) / step;
}


template <typename T>
auto xrange_iterator<T>::operator[](difference_type n) const -> reference
{
    return *(*this + n);
}


template <typename T>
auto xrange_iterator<T>::operator*() -> reference
{
    return value;
}


template <typename T>
auto xrange_iterator<T>::operator*() const -> const_reference
{
    return value;
}


template <typename T>
auto xrange_iterator<T>::operator->() -> pointer
{
    return &value;
}


template <typename T>
auto xrange_iterator<T>::operator->() const -> const_pointer
{
    return &value;
}


template <typename T>
void xrange_iterator<T>::swap(self_t& rhs)
{
    PYCPP_NAMESPACE::swap(value, rhs.value);
    PYCPP_NAMESPACE::swap(step, rhs.step);
}


// FUNCTIONS
// ---------


/**
 *  \brief Create generator from range [0, stop).
 */
template <typename T = int>
range<xrange_iterator<T>> xrange(T start, T stop, T step = 1)
{
    using iterator = xrange_iterator<T>;
    using range_type = range<iterator>;

    // WARNING
    // Need to sanitize our input, since auto-ranges use `==`
    // for comparison, and we can't override comparison to suddenly
    // be unidirectional.
    T distance = stop - start;
    if (step == 0) {
        throw runtime_error("Cannot have a 0-step range.");
    } else if ((step > 0) ^ (distance > 0)) {
        // we have a negative distance, or a negative step, 0 range
        return range_type();
    }

    // force exactly step_count steps.
    T step_count = static_cast<T>(ceil(double(distance) / step));
    stop = start + (step * step_count);

    return {iterator(start, step), iterator(stop, step)};
}


/**
 *  \brief Create generator from range [0, stop).
 */
template <typename T = int>
range<xrange_iterator<T>> xrange(T stop)
{
    return xrange(0, stop, 1);
}

PYCPP_END_NAMESPACE
