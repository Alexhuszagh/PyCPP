//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Axis view into the N-dimensional array.
 *
 *  Use indexing to wrap an N-dimensional array into an axis view.
 *  For example, an array of (5, 10, 10) has 500 elements, ...
 *  // TODO: need to expand the documentation on how to use the axis view.
 */

#pragma once

#include <ordering.h>
#include <cstdint>
#include <memory>

// DECLARATIONS
// ------------


/**
 *  \brief Random-access iterator over an axis.
 */
template <typename T>
struct ndarray_axis_iterator: std::iterator<std::random_access_iterator_tag, T>
{
public:
    typedef ndarray_axis_iterator<T> self;
    typedef std::iterator<std::random_access_iterator_tag, T> base;
    using typename base::value_type;

    // CONSTRUCTORS
    ndarray_axis_iterator(value_type*, value_type*, size_t);
    ~ndarray_axis_iterator();

    // RELATIONAL OPERATORS
    bool operator==(const self&) const;
    bool operator!=(const self&) const;
    bool operator<(const self&) const;
    bool operator<=(const self&) const;
    bool operator>(const self&) const;
    bool operator>=(const self&) const;

    // INCREMENTORS
    self& operator++();
    self operator++(int);
    self& operator--();
    self operator--(int);
    // TODO: need random access

    // DEREFERENCE
    value_type & operator*();
    const value_type & operator*() const;
    value_type * operator->();
    const value_type * operator->() const;

private:
    value_type* first_ = nullptr;
    value_type* last_ = nullptr;
    size_t step_ = 1;
};


/**
 *  \brief Axis for N-dimensional array.
 */
template <typename T>
class ndarray_axis
{
public:
    // MEMBER TYPES
    // ------------
    typedef ndarray_axis<T> self;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef ndarray_axis_iterator<T> iterator;
    typedef ndarray_axis_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t size_type;

    // MEMBER FUNCTIONS
    // ----------------
//    ndarray_axis() = default;
//    ndarray_axis(const self& arr);
//    self& operator=(const self& arr);
//    ndarray_axis(self&& arr);
//    self& operator=(self&& arr);

    // ITERATORS
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    // TODO: rbegin(), etc...

    // MATH OPERATORS
    self& operator+=(const value_type&);
    self& operator-=(const value_type&);
    self& operator*=(const value_type&);
    self& operator/=(const value_type&);
    self operator+(const value_type&);
    self operator-(const value_type&);
    self operator*(const value_type&);
    self operator/(const value_type&);

    // NEED ITERATORS

private:
    value_type* data_ = nullptr;
    size_t length_ = 0;
    size_type step_ = 1;
};


// DEFINITIONS
// -----------


template <typename T>
ndarray_axis_iterator<T>::ndarray_axis_iterator(value_type* first, value_type* last, size_t step):
    first_(first),
    last_(last),
    step_(step)
{}


template <typename T>
ndarray_axis_iterator<T>::~ndarray_axis_iterator()
{}


template <typename T>
bool ndarray_axis_iterator<T>::operator==(const self& other) const
{
    return std::tie(first_, last_, step_) == std::tie(other.first_, other.last_, other.step_);
}


template <typename T>
bool ndarray_axis_iterator<T>::operator!=(const self& other) const
{
    return not_equal_to(*this, other);
}


template <typename T>
bool ndarray_axis_iterator<T>::operator<(const self& other) const
{
    return std::tie(first_, last_, step_) < std::tie(other.first_, other.last_, other.step_);
}


template <typename T>
bool ndarray_axis_iterator<T>::operator<=(const self& other) const
{
    return less_equal(*this, other);
}


template <typename T>
bool ndarray_axis_iterator<T>::operator>(const self& other) const
{
    return greater(*this, other);
}


template <typename T>
bool ndarray_axis_iterator<T>::operator>=(const self& other) const
{
    return greater_equal(*this, other);
}


template <typename T>
auto ndarray_axis_iterator<T>::operator++() -> self&
{
    first_ += step_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator++(int) -> self
{
    self copy(*this);
    operator++();
    return copy;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator--() -> self&
{
    first_ -= step_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator--(int) -> self
{
    self copy(*this);
    operator--();
    return copy;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator*() -> value_type &
{
    return *first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator*() const -> const value_type &
{
    return *first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator->() -> value_type *
{
    return first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator->() const -> const value_type *
{
    return first_;
}


template <typename T>
auto ndarray_axis<T>::begin() -> iterator
{
    return iterator(data_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::begin() const -> const_iterator
{
    return const_iterator(data_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::cbegin() const -> const_iterator
{
    return const_iterator(data_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::end() -> iterator
{
    return iterator(data_+length_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::end() const -> const_iterator
{
    return const_iterator(data_+length_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::cend() const -> const_iterator
{
    return const_iterator(data_+length_, data_+length_, step_);
}


template <typename T>
auto ndarray_axis<T>::operator+=(const value_type& rhs) -> self&
{
    for (auto &value: *this) {
        value += rhs;
    }

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator-=(const value_type& rhs) -> self&
{
    for (auto &value: *this) {
        value -= rhs;
    }

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator*=(const value_type& rhs) -> self&
{
    for (auto &value: *this) {
        value *= rhs;
    }

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator/=(const value_type& rhs) -> self&
{
    for (auto &value: *this) {
        value /= rhs;
    }

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator+(const value_type& value) -> self
{
    self copy(*this);
    copy += value;
    return copy;
}


template <typename T>
auto ndarray_axis<T>::operator-(const value_type& value) -> self
{
    self copy(*this);
    copy -= value;
    return copy;
}


template <typename T>
auto ndarray_axis<T>::operator*(const value_type& value) -> self
{
    self copy(*this);
    copy *= value;
    return copy;
}


template <typename T>
auto ndarray_axis<T>::operator/(const value_type& value) -> self
{
    self copy(*this);
    copy /= value;
    return copy;
}
