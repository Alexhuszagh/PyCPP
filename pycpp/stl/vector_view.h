//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Zero-copy vector view container.
 */

#pragma once

#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/vector.h>
#include <assert.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------


/**
 *  \brief STL vector wrapper.
 *
 *  Binds a pointer and the vector length, accepting either a C++
 *  vector, a null-terminated vector, a character array and length,
 *  or an begin/end pointer pair.
 *
 *  \warning The lifetime of the source data must outlive the wrapper.
 *  The wrapper has **no** ownership, and is merely an STL-like wrapper
 *  for performance reasons.
 */
template <typename T>
class vector_view
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<pointer>;
    using const_reverse_iterator = std::reverse_iterator<const_pointer>;

    // MEMBER FUNCTIONS
    // ----------------
    vector_view() = default;
    vector_view(const vector_view<T>& vec);
    vector_view<T>& operator=(const vector_view<T>& vec);
    vector_view(vector_view<T>&& vec);
    vector_view<T>& operator=(vector_view<T>&& vec);

    template <typename A> vector_view(const vector<T, A>& vec);
    template <typename A> vector_view<T>& operator=(const vector<T, A>& vec);
    vector_view(pointer t, size_type n);
    vector_view(pointer first, pointer last);

    // ITERATORS
    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    // CAPACITY
    size_t size() const;
    bool empty() const noexcept;

    // ELEMENT ACCESS
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // MODIFIERS
    void swap(vector_view<T>& rhs);
    void remove_prefix(size_type n);
    void remove_suffix(size_type n);

    // OPERATIONS
    size_type copy(value_type* dst, size_type count, size_type pos = 0);

    // CONVERSIONS
    explicit operator bool() const;
    template <typename A> explicit operator vector<T, A>() const;

private:
    const_pointer data_ = nullptr;
    size_t size_ = 0;

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename U>
    friend void swap(vector_view<U> &lhs, vector_view<U> &right);

    // RELATIONAL OPERATORS
    template <typename U>
    friend bool operator==(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator==(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator==(const vector_view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const vector_view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const vector_view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const vector_view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const vector_view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const vector<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const vector_view<U>& lhs, const vector<U>& right) noexcept;
};

// IMPLEMENTATION
// --------------


template <typename T>
void swap(vector_view<T>& lhs, vector_view<T>& rhs)
{
    lhs.swap(rhs);
}


template <typename T>
bool operator==(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}


template <typename T>
bool operator==(const std::vector<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) == rhs;
}


template <typename T>
bool operator==(const vector_view<T>& lhs, const std::vector<T>& rhs) noexcept
{
    return lhs == vector_view<T>(rhs);
}


template <typename T>
bool operator!=(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename T>
bool operator!=(const std::vector<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) != rhs;
}


template <typename T>
bool operator!=(const vector_view<T>& lhs, const std::vector<T>& rhs) noexcept
{
    return lhs != vector_view<T>(rhs);
}


template <typename T>
bool operator<(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}


template <typename T>
bool operator<(const std::vector<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) < rhs;
}


template <typename T>
bool operator<(const vector_view<T>& lhs, const std::vector<T> &rhs) noexcept
{
    return lhs < vector_view<T>(rhs);
}


template <typename T>
bool operator<=(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename T>
bool operator<=(const std::vector<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) <= rhs;
}


template <typename T>
bool operator<=(const vector_view<T>& lhs, const std::vector<T>& rhs) noexcept
{
    return lhs <= vector_view<T>(rhs);
}


template <typename T>
bool operator>(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return rhs < lhs;
}


template <typename T>
bool operator>(const std::vector<T> &lhs, const vector_view<T> &rhs) noexcept
{
    return vector_view<T>(lhs) > rhs;
}


template <typename T>
bool operator>(const vector_view<T> &lhs, const std::vector<T> &rhs) noexcept
{
    return lhs > vector_view<T>(rhs);
}


template <typename T>
bool operator>=(const vector_view<T> &lhs, const vector_view<T> &rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename T>
bool operator>=(const std::vector<T> &lhs, const vector_view<T> &rhs) noexcept
{
    return vector_view<T>(lhs) >= rhs;
}


template <typename T>
bool operator>=(const vector_view<T> &lhs, const std::vector<T> &rhs) noexcept
{
    return lhs >= vector_view<T>(rhs);
}


template <typename T>
vector_view<T>::vector_view(const vector_view<T>& vec):
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
auto vector_view<T>::operator=(const vector_view<T>& vec) -> vector_view<T>&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(vector_view<T>&& vec)
{
    swap(vec);
}


template <typename T>
auto vector_view<T>::operator=(vector_view<T>&& vec) -> vector_view<T>&
{
    swap(vec);
    return *this;
}

template <typename T>
template <typename A>
vector_view<T>::vector_view(const vector<T, A>& vec):
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
template <typename A>
auto vector_view<T>::operator=(const vector<T, A>& vec) -> vector_view<T>&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(pointer t, size_type n):
    data_(t),
    size_(n)
{}


template <typename T>
vector_view<T>::vector_view(pointer first, pointer last):
    data_(first),
    size_(last - first)
{}


template <typename T>
auto vector_view<T>::begin() const -> const_iterator
{
    return data_;
}


template <typename T>
auto vector_view<T>::end() const -> const_iterator
{
    return data_ + size_;
}


template <typename T>
auto vector_view<T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto vector_view<T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto vector_view<T>::cbegin() const -> const_iterator
{
    return begin();
}


template <typename T>
auto vector_view<T>::cend() const -> const_iterator
{
    return end();
}


template <typename T>
auto vector_view<T>::crbegin() const -> const_reverse_iterator
{
    return rbegin();
}


template <typename T>
auto vector_view<T>::crend() const -> const_reverse_iterator
{
    return rend();
}


template <typename T>
size_t vector_view<T>::size() const
{
    return size_;
}


template <typename T>
bool vector_view<T>::empty() const noexcept
{
    return size_ == 0;
}


template <typename T>
auto vector_view<T>::operator[](size_type pos) -> reference
{
    assert(pos <= size() && "vector index out of bounds");
    return *const_cast<pointer>(data_ + pos);
}


template <typename T>
auto vector_view<T>::operator[](size_type pos) const -> const_reference
{
    assert(pos <= size() && "vector index out of bounds");
    return *(data_ + pos);
}


template <typename T>
auto vector_view<T>::at(size_type pos) -> reference
{
    return operator[](pos);
}


template <typename T>
auto vector_view<T>::at(size_type pos) const -> const_reference
{
    return operator[](pos);
}


template <typename T>
auto vector_view<T>::front() -> reference
{
    assert(!empty() && "vector::front(): vector is empty");
    return *const_cast<pointer>(data_);
}


template <typename T>
auto vector_view<T>::front() const -> const_reference
{
    assert(!empty() && "vector::front(): vector is empty");
    return *data_;
}


template <typename T>
auto vector_view<T>::back() -> reference
{
    assert(!empty() && "vector::back(): vector is empty");
    return *const_cast<pointer>(data_ + size_ - 1);
}


template <typename T>
auto vector_view<T>::back() const -> const_reference
{
    assert(!empty() && "vector::back(): vector is empty");
    return *(data_ + size_ - 1);
}


template <typename T>
auto vector_view<T>::data() noexcept -> pointer
{
    return const_cast<pointer>(data_);
}


template <typename T>
auto vector_view<T>::data() const noexcept -> const_pointer
{
    return data_;
}


template <typename T>
void vector_view<T>::swap(vector_view<T>& rhs)
{
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
}


template <typename T>
void vector_view<T>::remove_prefix(size_type n)
{
    assert(n <= size() && "vector_view::remove_prefix greater than size.");
    data_ += n;
    size_ -= n;
}


template <typename T>
void vector_view<T>::remove_suffix(size_type n)
{
    assert(n <= size() && "vector_view::remove_suffix greater than size.");
    size_ -= n;
}


template <typename T>
auto vector_view<T>::copy(value_type* dst, size_type count, size_type pos) -> size_type
{
    if (pos > size()) {
        throw std::out_of_range("vector_view::copy");
    }

    size_type length = std::min(count, size() - pos);
    // can pass 0 to memcpy
    memcpy(dst, data() + pos, length * sizeof(value_type));

    return length;
}


template <typename T>
vector_view<T>::operator bool() const
{
    return !empty();
}


template <typename T>
template <typename A>
vector_view<T>::operator vector<T, A>() const
{
    return vector<T, A>(begin(), end());
}

PYCPP_END_NAMESPACE
