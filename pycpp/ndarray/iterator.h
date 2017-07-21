//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator over the N-dimensional array.
 */

#pragma once

#include <pycpp/ndarray/array_fwd.h>
#include <pycpp/ordering.h>
#include <iterator>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Random-access iterator over an N-dimensional array.
 */
template <typename T>
class ndarray_iterator: std::iterator<std::random_access_iterator_tag, T>
{
    // MEMBER TYPES
    // ------------
    typedef ndarray_iterator<T> self;
    typedef std::iterator<std::random_access_iterator_tag, T> base;
    using typename base::value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    using typename base::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    ndarray_iterator() = default;
    ndarray_iterator(pointer, size_t);
    ndarray_iterator(const self&);
    self & operator=(const self&);
    ndarray_iterator(self&&);
    self & operator=(self&&);

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
    self& operator+=(difference_type n);
    self& operator-=(difference_type n);
    self operator+(difference_type n);
    self operator-(difference_type n);
    difference_type operator-(const self&);
    reference operator[](difference_type n) const;

    // DEREFERENCE
    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;

    // OTHER
    void swap(self&);

private:
    pointer data_ = nullptr;
    size_t step_ = 1;
};


// DEFINITIONS
// -----------


template <typename T>
ndarray_iterator<T>::ndarray_iterator(pointer data, size_t step):
    data_(data),
    step_(step)
{}


template <typename T>
ndarray_iterator<T>::ndarray_iterator(const self& rhs):
    data_(rhs.data_),
    step_(rhs.step_)
{}


template <typename T>
auto ndarray_iterator<T>::operator=(const self& rhs) -> self&
{
    data_ = rhs.data_;
    step_ = rhs.step_;
    return *this;
}


template <typename T>
ndarray_iterator<T>::ndarray_iterator(self&& rhs)
{
    swap(rhs);
}


template <typename T>
auto ndarray_iterator<T>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename T>
bool ndarray_iterator<T>::operator==(const self& rhs) const
{
    return std::tie(data_, step_) == std::tie(rhs.data_, rhs.step_);
}


template <typename T>
bool ndarray_iterator<T>::operator!=(const self& rhs) const
{
    return not_equal_to(*this, rhs);
}


template <typename T>
bool ndarray_iterator<T>::operator<(const self& rhs) const
{
    return std::tie(data_, step_) < std::tie(rhs.data_, rhs.step_);
}


template <typename T>
bool ndarray_iterator<T>::operator<=(const self& rhs) const
{
    return less_equal(*this, rhs);
}


template <typename T>
bool ndarray_iterator<T>::operator>(const self& rhs) const
{
    return greater(*this, rhs);
}


template <typename T>
bool ndarray_iterator<T>::operator>=(const self& rhs) const
{
    return greater_equal(*this, rhs);
}


template <typename T>
auto ndarray_iterator<T>::operator++() -> self&
{
    data_ += step_;
    return *this;
}


template <typename T>
auto ndarray_iterator<T>::operator++(int) -> self
{
    self copy(*this);
    operator++();
    return copy;
}


template <typename T>
auto ndarray_iterator<T>::operator--() -> self&
{
    data_ -= step_;
    return *this;
}


template <typename T>
auto ndarray_iterator<T>::operator--(int) -> self
{
    self copy(*this);
    operator--();
    return copy;
}


template <typename T>
auto ndarray_iterator<T>::operator+=(difference_type n) -> self&
{
    data_ += n * step_;
    return *this;
}


template <typename T>
auto ndarray_iterator<T>::operator-=(difference_type n) -> self&
{
    data_ -= n * step_;
    return *this;
}


template <typename T>
auto ndarray_iterator<T>::operator+(difference_type n) -> self
{
    self copy(*this);
    copy += n;
    return copy;
}


template <typename T>
auto ndarray_iterator<T>::operator-(difference_type n) -> self
{
    self copy(*this);
    copy -= n;
    return copy;
}


template <typename T>
auto ndarray_iterator<T>::operator-(const self& rhs) -> difference_type
{
    return data_ - rhs.data_;
}


template <typename T>
auto ndarray_iterator<T>::operator[](difference_type n) const -> reference
{
    return data_[n * step_];
}


template <typename T>
auto ndarray_iterator<T>::operator*() -> reference
{
    return *data_;
}


template <typename T>
auto ndarray_iterator<T>::operator*() const -> const_reference
{
    return *data_;
}


template <typename T>
auto ndarray_iterator<T>::operator->() -> pointer
{
    return data_;
}


template <typename T>
auto ndarray_iterator<T>::operator->() const -> const_pointer
{
    return data_;
}


template <typename T>
void ndarray_iterator<T>::swap(self& rhs)
{
    std::swap(data_, rhs.data_);
    std::swap(step_, rhs.step_);
}

PYCPP_END_NAMESPACE
