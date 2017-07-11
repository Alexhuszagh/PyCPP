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

#include <pycpp/ordering.h>
#include <pycpp/parallel.h>
#include <algorithm>
#include <cstdint>
#include <memory>

PYCPP_BEGIN_NAMESPACE

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
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    using typename base::difference_type;

    // CONSTRUCTORS
    ndarray_axis_iterator(pointer, pointer, size_t);
    ndarray_axis_iterator(const self&);
    self & operator=(const self&);
    ndarray_axis_iterator(self&&);
    self & operator=(self&&);
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
    pointer first_ = nullptr;
    pointer last_ = nullptr;
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
    ndarray_axis(pointer, size_type, size_type);
    ndarray_axis(const self&);
    self& operator=(const self&);
    ndarray_axis(self&&);
    self& operator=(self&&);
    ~ndarray_axis();

    // ITERATORS
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    // MATH OPERATORS
    self& operator+=(const value_type&);
    self& operator-=(const value_type&);
    self& operator*=(const value_type&);
    self& operator/=(const value_type&);
    self operator+(const value_type&);
    self operator-(const value_type&);
    self operator*(const value_type&);
    self operator/(const value_type&);

    // ELEMENT ACCESS
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // TODO: need size
    // Need capacity

    // OTHER
    void swap(self&);

private:
    pointer data_ = nullptr;
    size_type length_ = 0;
    size_type step_ = 1;
};


// DEFINITIONS
// -----------


template <typename T>
ndarray_axis_iterator<T>::ndarray_axis_iterator(pointer first, pointer last, size_t step):
    first_(first),
    last_(last),
    step_(step)
{}


template <typename T>
ndarray_axis_iterator<T>::ndarray_axis_iterator(const self& rhs):
    first_(rhs.first_),
    last_(rhs.last_),
    step_(rhs.step_)
{}


template <typename T>
auto ndarray_axis_iterator<T>::operator=(const self& rhs) -> self&
{
    first_ = rhs.first_;
    last_ = rhs.last_;
    step_ = rhs.step_;
    return *this;
}


template <typename T>
ndarray_axis_iterator<T>::ndarray_axis_iterator(self&& rhs)
{
    swap(rhs);
}


template <typename T>
auto ndarray_axis_iterator<T>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


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
auto ndarray_axis_iterator<T>::operator+=(difference_type n) -> self&
{
    first_ += n * step_;
    return *this;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator-=(difference_type n) -> self&
{
    first_ -= n * step_;
    return *this;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator+(difference_type n) -> self
{
    self copy(*this);
    copy += n;
    return copy;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator-(difference_type n) -> self
{
    self copy(*this);
    copy -= n;
    return copy;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator-(const self& rhs) -> difference_type
{
    return first_ - rhs.first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator[](difference_type n) const -> reference
{
    return first_[n * step_];
}


template <typename T>
auto ndarray_axis_iterator<T>::operator*() -> reference
{
    return *first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator*() const -> const_reference
{
    return *first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator->() -> pointer
{
    return first_;
}


template <typename T>
auto ndarray_axis_iterator<T>::operator->() const -> const_pointer
{
    return first_;
}


template <typename T>
void ndarray_axis_iterator<T>::swap(self& rhs)
{
    std::swap(first_, rhs.first_);
    std::swap(last_, rhs.last_);
    std::swap(step_, rhs.step_);
}


template <typename T>
ndarray_axis<T>::ndarray_axis(pointer data, size_type length, size_type step):
    data_(data),
    length_(length),
    step_(step)
{}


template <typename T>
ndarray_axis<T>::ndarray_axis(const self& rhs):
    data_(rhs.data_),
    length_(rhs.length_),
    step_(rhs.step_)
{}


template <typename T>
auto ndarray_axis<T>::operator=(const self& rhs) -> self&
{
    data_ = rhs.data_;
    length_ = rhs.length_;
    step_ = rhs.step_;
}


template <typename T>
ndarray_axis<T>::ndarray_axis(self&& rhs)
{
    swap(rhs);
}


template <typename T>
auto ndarray_axis<T>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename T>
ndarray_axis<T>::~ndarray_axis()
{}


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
auto ndarray_axis<T>::rbegin() -> reverse_iterator
{
    return reverse_iterator(end());
}


template <typename T>
auto ndarray_axis<T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto ndarray_axis<T>::crbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto ndarray_axis<T>::rend() -> reverse_iterator
{
    return reverse_iterator(begin());
}


template <typename T>
auto ndarray_axis<T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto ndarray_axis<T>::crend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto ndarray_axis<T>::operator+=(const value_type& rhs) -> self&
{
    std::for_each(PARALLEL_EXECUTION begin(), end(), [&rhs](value_type& lhs) {
        lhs += rhs;
    });

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator-=(const value_type& rhs) -> self&
{
    std::for_each(PARALLEL_EXECUTION begin(), end(), [&rhs](value_type& lhs) {
        lhs -= rhs;
    });

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator*=(const value_type& rhs) -> self&
{
    std::for_each(PARALLEL_EXECUTION begin(), end(), [&rhs](value_type& lhs) {
        lhs *= rhs;
    });

    return *this;
}


template <typename T>
auto ndarray_axis<T>::operator/=(const value_type& rhs) -> self&
{
    std::for_each(PARALLEL_EXECUTION begin(), end(), [&rhs](value_type& lhs) {
        lhs /= rhs;
    });

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


template <typename T>
auto ndarray_axis<T>::operator[](size_type n) -> reference
{
    return at(n);
}


template <typename T>
auto ndarray_axis<T>::operator[](size_type n) const -> const_reference
{
    return at(n);
}


template <typename T>
auto ndarray_axis<T>::at(size_type n) -> reference
{
    return *(begin() + n);
}


template <typename T>
auto ndarray_axis<T>::at(size_type n) const -> const_reference
{
    return *(begin() + n);
}


template <typename T>
auto ndarray_axis<T>::front() -> reference
{
    return *begin();
}


template <typename T>
auto ndarray_axis<T>::front() const -> const_reference
{
    return *begin();
}


template <typename T>
auto ndarray_axis<T>::back() -> reference
{
    return *rbegin();
}


template <typename T>
auto ndarray_axis<T>::back() const -> const_reference
{
    return *rbegin();
}


template <typename T>
void ndarray_axis<T>::swap(self& rhs)
{
    std::swap(data_, rhs.data_);
    std::swap(length_, rhs.length_);
    std::swap(step_, rhs.step_);
}

PYCPP_END_NAMESPACE
