//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance array, analogous to a NumPy array.
 */

#pragma once

#include <mathlib/axis.h>
#include <vector>

// DECLARATIONS
// ------------

// Need to consider how to create an NDArray


/**
 *  \brief N-dimensional array type.
 */
template <typename T, typename Allocator = std::allocator<T>>
class ndarray
{
public:
    // MEMBER TYPES
    // ------------
    typedef ndarray<T, Allocator> self;
    typedef ndarray_axis<T> axis_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef ndarray_axis_iterator<T> iterator;
    typedef ndarray_axis_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef Allocator allocator_type;

    // MEMBER FUNCTIONS
    // ----------------
    ndarray() = default;
    ndarray(const self& arr);
    self& operator=(const self& arr);
    ndarray(self&& arr);
    self& operator=(self&& arr);

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
// TODO: need operators for ndarrays too

    // VIEWS
    axis_type view();
    // TODO: need to be able to get by axis...

    void swap(self& arr);

private:
    std::vector<value_type, allocator_type> vector_;
    std::vector<size_t> dims_ = {0};
};


// DEFINITIONS
// -----------


template <typename T, typename A>
ndarray<T, A>::ndarray(const self& arr):
    vector_(arr.vector_),
    dims_(arr.dims_)
{}


template <typename T, typename A>
auto ndarray<T, A>::operator=(const self& arr) -> self&
{
    vector_ = arr.vector_;
    dims_ = arr.dims_;
    return *this;
}


template <typename T, typename A>
ndarray<T, A>::ndarray(self&& arr):
    vector_(std::move(arr.vector_)),
    dims_(std::move(arr.dims_))
{}


template <typename T, typename A>
auto ndarray<T, A>::operator=(self&& arr) -> self&
{
    swap(arr);
    return *this;
}


template <typename T, typename A>
auto ndarray<T, A>::begin() -> iterator
{
    return view().begin();
}


template <typename T, typename A>
auto ndarray<T, A>::begin() const -> const_iterator
{
    return view().begin();
}


template <typename T, typename A>
auto ndarray<T, A>::cbegin() const -> const_iterator
{
    return view().cbegin();
}


template <typename T, typename A>
auto ndarray<T, A>::end() -> iterator
{
    return view().end();
}


template <typename T, typename A>
auto ndarray<T, A>::end() const -> const_iterator
{
    return view().end();
}


template <typename T, typename A>
auto ndarray<T, A>::cend() const -> const_iterator
{
    return view().cend();
}


template <typename T, typename A>
auto ndarray<T, A>::rbegin() -> reverse_iterator
{
    return view().rbegin();
}


template <typename T, typename A>
auto ndarray<T, A>::rbegin() const -> const_reverse_iterator
{
    return view().rbegin();
}


template <typename T, typename A>
auto ndarray<T, A>::crbegin() const -> const_reverse_iterator
{
    return view().crbegin();
}


template <typename T, typename A>
auto ndarray<T, A>::rend() -> reverse_iterator
{
    return view().rend();
}


template <typename T, typename A>
auto ndarray<T, A>::rend() const -> const_reverse_iterator
{
    return view().rend();
}


template <typename T, typename A>
auto ndarray<T, A>::crend() const -> const_reverse_iterator
{
    return view().crend();
}


template <typename T, typename A>
auto ndarray<T, A>::operator+=(const value_type& rhs) -> self&
{
    view() += rhs;
    return *this;
}


template <typename T, typename A>
auto ndarray<T, A>::operator-=(const value_type& rhs) -> self&
{
    view() -= rhs;
    return *this;
}


template <typename T, typename A>
auto ndarray<T, A>::operator*=(const value_type& rhs) -> self&
{
    view() *= rhs;
    return *this;
}


template <typename T, typename A>
auto ndarray<T, A>::operator/=(const value_type& rhs) -> self&
{
    view() /= rhs;
    return *this;
}


template <typename T, typename A>
auto ndarray<T, A>::operator+(const value_type& rhs) -> self
{
    self copy(*this);
    copy += rhs;
    return copy;
}


template <typename T, typename A>
auto ndarray<T, A>::operator-(const value_type& rhs) -> self
{
    self copy(*this);
    copy -= rhs;
    return copy;
}


template <typename T, typename A>
auto ndarray<T, A>::operator*(const value_type& rhs) -> self
{
    self copy(*this);
    copy *= rhs;
    return copy;
}


template <typename T, typename A>
auto ndarray<T, A>::operator/(const value_type& rhs) -> self
{
    self copy(*this);
    copy /= rhs;
    return copy;
}


template <typename T, typename A>
auto ndarray<T, A>::view() -> axis_type
{
    return axis_type(vector_.data(), vector_.size(), 1);
}


template <typename T, typename A>
void ndarray<T, A>::swap(self& arr)
{
    std::swap(vector_, arr.vector_);
    std::swap(dims_, arr.dims_);
}
