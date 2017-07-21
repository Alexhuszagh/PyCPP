//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Non-owning view of an N-dimensional array.
 */

#pragma once

#include <pycpp/ndarray/iterator.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief Unowned view for an N-dimensional array.
 */
template <typename T>
class ndarray_view
{
public:
    // MEMBER TYPES
    // ------------
    typedef ndarray_view<T> self;
    typedef vector_view<T> vector_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef ndarray_iterator<T> iterator;
    typedef ndarray_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    ndarray_view() = default;
    ndarray_view(const self&) = default;
    self& operator=(const self&) = default;
    ndarray_view(self&&) = default;
    self& operator=(self&&) = default;

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

    // CAPACITY
    size_t size() const noexcept;
    size_t max_size() const noexcept;
    size_t capacity() const noexcept;
    size_t empty() const noexcept;
    size_t ndims() const noexcept;
    const ndarray_dimension_view& shape() const noexcept;

    // ELEMENT ACCESS
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // MODIFIERS
//    void swap(self& other);

    // PROPERTIES

private:
    vector_type data_;
    ndarray_dimension_view dims_;
    size_t step_ = 1;
};


// DEFINITIONS
// -----------


template <typename T>
auto ndarray_view<T>::begin() -> iterator
{
    return iterator(data(), step_);
}


template <typename T>
auto ndarray_view<T>::begin() const -> const_iterator
{
    return const_iterator(data(), step_);
}


template <typename T>
auto ndarray_view<T>::cbegin() const -> const_iterator
{
    return const_iterator(data(), step_);
}


template <typename T>
auto ndarray_view<T>::end() -> iterator
{
    return iterator(data() + size(), step_);
}


template <typename T>
auto ndarray_view<T>::end() const -> const_iterator
{
    return const_iterator(data() + size(), step_);
}


template <typename T>
auto ndarray_view<T>::cend() const -> const_iterator
{
    return const_iterator(data() + size(), step_);
}


template <typename T>
auto ndarray_view<T>::rbegin() -> reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto ndarray_view<T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto ndarray_view<T>::crbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}


template <typename T>
auto ndarray_view<T>::rend() -> reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto ndarray_view<T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto ndarray_view<T>::crend() const -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}


template <typename T>
size_t ndarray_view<T>::size() const noexcept
{
    return data_.size();
}


template <typename T>
size_t ndarray_view<T>::max_size() const noexcept
{
    return data_.max_size();
}


template <typename T>
size_t ndarray_view<T>::capacity() const noexcept
{
    return data_.capacity();
}


template <typename T>
size_t ndarray_view<T>::empty() const noexcept
{
    return data_.empty();
}


template <typename T>
size_t ndarray_view<T>::ndims() const noexcept
{
    return dims_.size();
}

template <typename T>
const ndarray_dimension_view& ndarray_view<T>::shape() const noexcept
{
    return dims_;
}


template <typename T>
auto ndarray_view<T>::data() noexcept -> pointer
{
    return data_.data();
}


template <typename T>
auto ndarray_view<T>::data() const noexcept -> const_pointer
{
    return data_.data();
}

PYCPP_END_NAMESPACE
