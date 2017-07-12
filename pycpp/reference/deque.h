//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Reference-wrapper like deque.
 *
 *  Stores non-nullable pointers to existing objects, using a deque
 *  wrapper as the underlying structure.
 */

#pragma once

#include <pycpp/ordering.h>
#include <pycpp/reference/core.h>
#include <deque>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// DECLARATION
// -----------

/**
 *  \brief Deque wrapper mapping value pointers to values.
 */
template <typename T>
struct reference_deque_base
{
    // MEMBER TYPES
    // ------------
    typedef reference_deque_base<T> self;
    using value_type = typename std::remove_pointer<T>::type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef sequence_iterator_impl<typename std::deque<pointer>::iterator> iterator;
    typedef sequence_iterator_impl<typename std::deque<pointer>::const_iterator> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    reference_deque_base() = default;
    reference_deque_base(size_type n, reference);
    reference_deque_base(const self&) = default;
    self & operator=(const self&) = default;
    reference_deque_base(self&&) = default;
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
    void push_front(reference r);
    void pop_front();
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
    std::deque<pointer> deque_;
};


template <typename T>
using reference_deque_impl = typename std::conditional<
    std::is_reference<T>::value,
    reference_deque_base<typename std::remove_reference<T>::type*>,
    std::deque<T>
>::type;

// IMPLEMENTATION
// --------------


template <typename T>
reference_deque_base<T>::reference_deque_base(size_type n, reference r)
{
    for (size_t i = 0; i < n; ++i) {
        deque_.emplace_back(std::addressof(r));
    }
}


template <typename T>
auto reference_deque_base<T>::begin() -> iterator
{
    return iterator(deque_.begin(), [](pointer& p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::begin() const -> const_iterator
{
    return const_iterator(deque_.begin(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::cbegin() const -> const_iterator
{
    return const_iterator(deque_.begin(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::end() -> iterator
{
    return iterator(deque_.end(), [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::end() const -> const_iterator
{
    return const_iterator(deque_.end(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::cend() const -> const_iterator
{
    return const_iterator(deque_.end(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::rbegin() -> reverse_iterator
{
    return reverse_iterator(end());
}


template <typename T>
auto reference_deque_base<T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto reference_deque_base<T>::crbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto reference_deque_base<T>::rend() -> reverse_iterator
{
    return reverse_iterator(begin());
}


template <typename T>
auto reference_deque_base<T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto reference_deque_base<T>::crend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto reference_deque_base<T>::size() const -> size_type
{
    return deque_.size();
}


template <typename T>
auto reference_deque_base<T>::max_size() const -> size_type
{
    return deque_.max_size();
}


template <typename T>
auto reference_deque_base<T>::capacity() const -> size_type
{
    return deque_.capacity();
}


template <typename T>
bool reference_deque_base<T>::empty() const noexcept
{
    return deque_.empty();
}


template <typename T>
void reference_deque_base<T>::shrink_to_fit()
{
    deque_.shrink_to_fit();
}


template <typename T>
auto reference_deque_base<T>::operator[](size_type n) -> reference
{
    return *deque_[n];
}


template <typename T>
auto reference_deque_base<T>::operator[](size_type n) const -> const_reference
{
    return *deque_[n];
}


template <typename T>
auto reference_deque_base<T>::at(size_type n) -> reference
{
    return *deque_.at(n);
}


template <typename T>
auto reference_deque_base<T>::at(size_type n) const -> const_reference
{
    return *deque_.at(n);
}


template <typename T>
auto reference_deque_base<T>::front() -> reference
{
    return *deque_.front();
}


template <typename T>
auto reference_deque_base<T>::front() const -> const_reference
{
    return *deque_.front();
}


template <typename T>
auto reference_deque_base<T>::back() -> reference
{
    return *deque_.back();
}


template <typename T>
auto reference_deque_base<T>::back() const -> const_reference
{
    return *deque_.back();
}


template <typename T>
void reference_deque_base<T>::assign(size_type n, reference r)
{
    deque_.assign(n, std::addressof(r));
}


template <typename T>
void reference_deque_base<T>::push_front(reference r)
{
    deque_.push_front(std::addressof(r));
}


template <typename T>
void reference_deque_base<T>::pop_front()
{
    deque_.pop_front();
}


template <typename T>
void reference_deque_base<T>::push_back(reference r)
{
    deque_.push_back(std::addressof(r));
}


template <typename T>
void reference_deque_base<T>::pop_back()
{
    deque_.pop_back();
}


template <typename T>
auto reference_deque_base<T>::insert(const_iterator position, reference r) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = deque_.insert(deque_.cbegin()+distance, std::addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::insert(const_iterator position, size_type n, reference r) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = deque_.insert(deque_.cbegin()+distance, n, std::addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::erase(const_iterator position) -> iterator
{
    auto distance = std::distance(begin(), position);
    auto it = deque_.erase(deque_.cbegin()+distance);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_deque_base<T>::erase(const_iterator first, const_iterator last) -> iterator
{
    auto f = std::distance(begin(), first);
    auto l = std::distance(begin(), last);
    auto it = deque_.erase(deque_.cbegin()+f, deque_.cbegin()+l);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
void reference_deque_base<T>::clear()
{
    deque_.clear();
}


template <typename T>
void reference_deque_base<T>::swap(self& rhs)
{
    std::swap(deque_, rhs.deque_);
}


template <typename T>
bool reference_deque_base<T>::operator==(const self& rhs) const
{
    return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
}


template <typename T>
bool reference_deque_base<T>::operator!=(const self& rhs) const
{
    return not_equal_to(*this, rhs);
}


template <typename T>
bool reference_deque_base<T>::operator<(const self& rhs) const
{
    return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
}


template <typename T>
bool reference_deque_base<T>::operator<=(const self& rhs) const
{
    return less_equal(*this, rhs);
}


template <typename T>
bool reference_deque_base<T>::operator>(const self& rhs) const
{
    return greater(*this, rhs);
}


template <typename T>
bool reference_deque_base<T>::operator>=(const self& rhs) const
{
    return greater_equal(*this, rhs);
}

}   /* detail */

// OBJECTS
// -------


/**
 *  \brief Deque wrapper that handles reference values.
 */
template <typename T>
struct reference_deque: PYCPP_NAMESPACE::detail::reference_deque_impl<T>
{
    using base = PYCPP_NAMESPACE::detail::reference_deque_impl<T>;
    using base::base;
};

PYCPP_END_NAMESPACE
