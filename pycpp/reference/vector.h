//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Reference-wrapper like vector.
 *
 *  Stores non-nullable pointers to existing objects, using a vector
 *  wrapper as the underlying structure.
 */

#pragma once

#include <pycpp/ordering.h>
#include <pycpp/reference/core.h>
#include <vector>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// DECLARATION
// -----------

/**
 *  \brief Vector wrapper mapping value pointers to values.
 */
template <typename T>
struct reference_vector_base
{
    // MEMBER TYPES
    // ------------
    typedef reference_vector_base<T> self;
    using value_type = typename std::remove_pointer<T>::type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef sequence_iterator_impl<typename std::vector<pointer>::iterator> iterator;
    typedef sequence_iterator_impl<typename std::vector<pointer>::const_iterator> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    reference_vector_base() = default;
    reference_vector_base(size_type n, reference);
    reference_vector_base(const self&) = default;
    self & operator=(const self&) = default;
    reference_vector_base(self&&) = default;
    self & operator=(self&&) = default;

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
    size_type size() const;
    size_type max_size() const;
    size_type capacity() const;
    bool empty() const noexcept;
    void reserve(size_type n);
    void shrink_to_fit();

    // ELEMENT ACCESS
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // MODIFIERS
    void assign(size_type n, reference r);
    void push_back(reference r);
    void pop_back();
    iterator insert(const_iterator position, reference r);
    iterator insert(const_iterator position, size_type n, reference r);
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void clear();
    void swap(self&);

    // RELATIONAL OPERATORS
    bool operator==(const self&) const;
    bool operator!=(const self&) const;
    bool operator<(const self&) const;
    bool operator<=(const self&) const;
    bool operator>(const self&) const;
    bool operator>=(const self&) const;

private:
    std::vector<pointer> vector_;
};


template <typename T>
using reference_vector_impl = typename std::conditional<
    std::is_reference<T>::value,
    reference_vector_base<typename std::remove_reference<T>::type*>,
    std::vector<T>
>::type;


// IMPLEMENTATION
// --------------


template <typename T>
reference_vector_base<T>::reference_vector_base(size_type n, reference r)
{
    vector_.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vector_.emplace_back(std::addressof(r));
    }
}


template <typename T>
auto reference_vector_base<T>::begin() -> iterator
{
    return iterator(vector_.begin(), [](pointer& p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::begin() const -> const_iterator
{
    return const_iterator(vector_.begin(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::cbegin() const -> const_iterator
{
    return const_iterator(vector_.begin(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::end() -> iterator
{
    return iterator(vector_.end(), [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::end() const -> const_iterator
{
    return const_iterator(vector_.end(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::cend() const -> const_iterator
{
    return const_iterator(vector_.end(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::rbegin() -> reverse_iterator
{
    return reverse_iterator(end());
}


template <typename T>
auto reference_vector_base<T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto reference_vector_base<T>::crbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto reference_vector_base<T>::rend() -> reverse_iterator
{
    return reverse_iterator(begin());
}


template <typename T>
auto reference_vector_base<T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto reference_vector_base<T>::crend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto reference_vector_base<T>::size() const -> size_type
{
    return vector_.size();
}


template <typename T>
auto reference_vector_base<T>::max_size() const -> size_type
{
    return vector_.max_size();
}


template <typename T>
auto reference_vector_base<T>::capacity() const -> size_type
{
    return vector_.capacity();
}


template <typename T>
bool reference_vector_base<T>::empty() const noexcept
{
    return vector_.empty();
}


template <typename T>
void reference_vector_base<T>::reserve(size_type n)
{
    vector_.reserve(n);
}


template <typename T>
void reference_vector_base<T>::shrink_to_fit()
{
    vector_.shrink_to_fit();
}


template <typename T>
auto reference_vector_base<T>::operator[](size_type n) -> reference
{
    return *vector_[n];
}


template <typename T>
auto reference_vector_base<T>::operator[](size_type n) const -> const_reference
{
    return *vector_[n];
}


template <typename T>
auto reference_vector_base<T>::at(size_type n) -> reference
{
    return *vector_.at(n);
}


template <typename T>
auto reference_vector_base<T>::at(size_type n) const -> const_reference
{
    return *vector_.at(n);
}


template <typename T>
auto reference_vector_base<T>::front() -> reference
{
    return *vector_.front();
}


template <typename T>
auto reference_vector_base<T>::front() const -> const_reference
{
    return *vector_.front();
}


template <typename T>
auto reference_vector_base<T>::back() -> reference
{
    return *vector_.back();
}


template <typename T>
auto reference_vector_base<T>::back() const -> const_reference
{
    return *vector_.back();
}


template <typename T>
void reference_vector_base<T>::assign(size_type n, reference r)
{
    vector_.assign(n, std::addressof(r));
}


template <typename T>
void reference_vector_base<T>::push_back(reference r)
{
    vector_.push_back(std::addressof(r));
}


template <typename T>
void reference_vector_base<T>::pop_back()
{
    vector_.pop_back();
}


template <typename T>
auto reference_vector_base<T>::insert(const_iterator position, reference r) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = vector_.insert(vector_.cbegin()+distance, std::addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::insert(const_iterator position, size_type n, reference r) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = vector_.insert(vector_.cbegin()+distance, n, std::addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::erase(const_iterator position) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = vector_.erase(vector_.cbegin()+distance);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::erase(const_iterator first, const_iterator last) -> iterator
{
    auto f = std::distance(begin(), first);
    auto l = std::distance(begin(), last);
    auto it = vector_.erase(vector_.cbegin()+f, vector_.cbegin()+l);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
void reference_vector_base<T>::clear()
{
    vector_.clear();
}


template <typename T>
void reference_vector_base<T>::swap(self& rhs)
{
    std::swap(vector_, rhs.vector_);
}


template <typename T>
bool reference_vector_base<T>::operator==(const self& rhs) const
{
    return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
}


template <typename T>
bool reference_vector_base<T>::operator!=(const self& rhs) const
{
    return not_equal_to(*this, rhs);
}


template <typename T>
bool reference_vector_base<T>::operator<(const self& rhs) const
{
    return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
}


template <typename T>
bool reference_vector_base<T>::operator<=(const self& rhs) const
{
    return less_equal(*this, rhs);
}


template <typename T>
bool reference_vector_base<T>::operator>(const self& rhs) const
{
    return greater(*this, rhs);
}


template <typename T>
bool reference_vector_base<T>::operator>=(const self& rhs) const
{
    return greater_equal(*this, rhs);
}

}   /* detail */

// OBJECTS
// -------


/**
 *  \brief Vector wrapper that handles reference values.
 */
template <typename T>
struct reference_vector: PYCPP_NAMESPACE::detail::reference_vector_impl<T>
{
    using base = PYCPP_NAMESPACE::detail::reference_vector_impl<T>;
    using base::base;
};

PYCPP_END_NAMESPACE
