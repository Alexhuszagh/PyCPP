//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Zero-copy vector view container.
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <vector>


// DECLARATION
// -----------


/** \brief STL vector wrapper.
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
    // MEMBER TEMPLATES
    template <typename U>
    using view = vector_view<U>;

    template <typename U>
    using vector = std::vector<U>;

    // MEMBER TYPES
    // ------------
    typedef vector_view<T> self;
    typedef std::vector<T> stl_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const_pointer> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    vector_view() = default;
    vector_view(const self& vec);
    self& operator=(const self& vec);
    vector_view(self&& vec);
    self& operator=(self&& vec);

    vector_view(const stl_type& vec);
    self& operator=(const stl_type& vec);
    vector_view(const_pointer t, size_type n);
    vector_view(const_pointer first, const_pointer last);

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
    const_pointer data() const noexcept;

    // MODIFIERS
    void swap(self& other);

    // CONVERSIONS
    explicit operator bool() const;
    explicit operator stl_type() const;

private:
    const_pointer data_ = nullptr;
    size_t size_ = 0;

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename U>
    friend void swap(view<U> &lhs, view<U> &right);

    // RELATIONAL OPERATORS
    template <typename U>
    friend bool operator==(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator==(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator==(const view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator!=(const view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator<(const view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator<=(const view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator>(const view<U>& lhs, const vector<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const view<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const vector<U>& lhs, const view<U>& right) noexcept;

    template <typename U>
    friend bool operator>=(const view<U>& lhs, const vector<U>& right) noexcept;
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
vector_view<T>::vector_view(const self& vec):
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
auto vector_view<T>::operator=(const self& vec) -> self&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(self&& vec)
{
    swap(vec);
}


template <typename T>
auto vector_view<T>::operator=(self&& vec) -> self&
{
    swap(vec);
    return *this;
}

template <typename T>
vector_view<T>::vector_view(const std::vector<T>& vec):
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
auto vector_view<T>::operator=(const std::vector<T>& vec) -> self&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(const_pointer t, size_type n):
    data_(t),
    size_(n)
{}


template <typename T>
vector_view<T>::vector_view(const_pointer first, const_pointer last):
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
auto vector_view<T>::data() const noexcept -> const_pointer
{
    return data_;
}


template <typename T>
void vector_view<T>::swap(self& other)
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}


template <typename T>
vector_view<T>::operator bool() const
{
    return !empty();
}


template <typename T>
vector_view<T>::operator stl_type() const
{
    return stl_type(begin(), end());
}
