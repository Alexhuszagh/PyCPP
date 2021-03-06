//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Semi-intrusive vector.
 *
 *  Stores non-nullable pointers to existing objects, using a vector
 *  wrapper as the underlying structure.
 */

#pragma once

#include <pycpp/misc/ordering.h>
#include <pycpp/intrusive/core.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

namespace sequence_detail
{
// MACROS
// ------

/**
 *  \brief Macro wrapper to automate iterator construction.
 */
#define REFERENCE_VECTOR_ITERATOR(it)                                   \
    iterator(it, [](pointer& p) -> reference                            \
    {                                                                   \
        return *p;                                                      \
    })

/**
 *  \brief Macro wrapper to automate const_iterator construction.
 */
#define REFERENCE_VECTOR_CONST_ITERATOR(it)                             \
    const_iterator(it, [](const_pointer p) -> const_reference           \
    {                                                                   \
        return *p;                                                      \
    })

// DECLARATION
// -----------

/**
 *  \brief Vector wrapper mapping value pointers to values.
 */
template <
    typename T,
    typename Alloc = allocator<T*>,
    template <typename, typename> class Container = vector
>
struct intrusive_vector_base
{
    // MEMBER TYPES
    // ------------
    using self_t = intrusive_vector_base<T, Alloc, Container>;
    using container_type = Container<T*, Alloc>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using iterator = sequence_iterator_impl<typename container_type::iterator>;
    using const_iterator = sequence_const_iterator_impl<typename container_type::const_iterator>;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    intrusive_vector_base() = default;
    intrusive_vector_base(size_type n, reference);
    intrusive_vector_base(const self_t&) = default;
    self_t & operator=(const self_t&) = default;
    intrusive_vector_base(self_t&&) = default;
    self_t & operator=(self_t&&) = default;

    // ITERATORS
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // CAPACITY
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;
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
    void swap(self_t&) noexcept;

    // RELATIONAL OPERATORS
    bool operator==(const self_t&) const noexcept;
    bool operator!=(const self_t&) const noexcept;
    bool operator<(const self_t&) const noexcept;
    bool operator<=(const self_t&) const noexcept;
    bool operator>(const self_t&) const noexcept;
    bool operator>=(const self_t&) const noexcept;

private:
    container_type vector_;
};

// IMPLEMENTATION
// --------------


template <typename T, typename A, template <typename, typename> class _>
intrusive_vector_base<T, A, _>::intrusive_vector_base(size_type n, reference r)
{
    vector_.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vector_.emplace_back(addressof(r));
    }
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::begin() noexcept -> iterator
{
    return REFERENCE_VECTOR_ITERATOR(vector_.begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::begin() const noexcept -> const_iterator
{
    return REFERENCE_VECTOR_CONST_ITERATOR(vector_.begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::cbegin() const noexcept -> const_iterator
{
    return REFERENCE_VECTOR_CONST_ITERATOR(vector_.begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::end() noexcept -> iterator
{
    return REFERENCE_VECTOR_ITERATOR(vector_.end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::end() const noexcept -> const_iterator
{
    return REFERENCE_VECTOR_CONST_ITERATOR(vector_.end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::cend() const noexcept -> const_iterator
{
    return REFERENCE_VECTOR_CONST_ITERATOR(vector_.end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::rbegin() noexcept -> reverse_iterator
{
    return reverse_iterator(end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::rbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::crbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::rend() noexcept -> reverse_iterator
{
    return reverse_iterator(begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::rend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::crend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::size() const noexcept -> size_type
{
    return vector_.size();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::max_size() const noexcept -> size_type
{
    return vector_.max_size();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::capacity() const noexcept -> size_type
{
    return vector_.capacity();
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::empty() const noexcept
{
    return vector_.empty();
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::reserve(size_type n)
{
    vector_.reserve(n);
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::shrink_to_fit()
{
    vector_.shrink_to_fit();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::operator[](size_type n) -> reference
{
    return *vector_[n];
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::operator[](size_type n) const -> const_reference
{
    return *vector_[n];
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::at(size_type n) -> reference
{
    return *vector_.at(n);
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::at(size_type n) const -> const_reference
{
    return *vector_.at(n);
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::front() -> reference
{
    return *vector_.front();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::front() const -> const_reference
{
    return *vector_.front();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::back() -> reference
{
    return *vector_.back();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::back() const -> const_reference
{
    return *vector_.back();
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::assign(size_type n, reference r)
{
    vector_.assign(n, addressof(r));
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::push_back(reference r)
{
    vector_.push_back(addressof(r));
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::pop_back()
{
    vector_.pop_back();
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::insert(const_iterator position, reference r) -> iterator
{
    auto dist = distance(cbegin(), position);
    auto it = vector_.insert(vector_.cbegin()+dist, addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::insert(const_iterator position, size_type n, reference r) -> iterator
{
    auto dist = distance(cbegin(), position);
    auto it = vector_.insert(vector_.cbegin()+dist, n, addressof(r));
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::erase(const_iterator position) -> iterator
{
    auto dist = distance(cbegin(), position);
    auto it = vector_.erase(vector_.cbegin()+dist);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T, typename A, template <typename, typename> class _>
auto intrusive_vector_base<T, A, _>::erase(const_iterator first, const_iterator last) -> iterator
{
    auto f = distance(cbegin(), first);
    auto l = distance(cbegin(), last);
    auto it = vector_.erase(vector_.cbegin()+f, vector_.cbegin()+l);
    return iterator(it, [](pointer p) -> reference {
        return *p;
    });
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::clear()
{
    vector_.clear();
}


template <typename T, typename A, template <typename, typename> class _>
void intrusive_vector_base<T, A, _>::swap(self_t& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(vector_, rhs.vector_);
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator==(const self_t& rhs) const noexcept
{
    return size() == rhs.size() && equal(begin(), end(), rhs.begin());
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator!=(const self_t& rhs) const noexcept
{
    return ordering::not_equal_to(*this, rhs);
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator<(const self_t& rhs) const noexcept
{
    return lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator<=(const self_t& rhs) const noexcept
{
    return ordering::less_equal(*this, rhs);
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator>(const self_t& rhs) const noexcept
{
    return ordering::greater(*this, rhs);
}


template <typename T, typename A, template <typename, typename> class _>
bool intrusive_vector_base<T, A, _>::operator>=(const self_t& rhs) const noexcept
{
    return ordering::greater_equal(*this, rhs);
}

// CLEANUP
// -------

#undef REFERENCE_VECTOR_ITERATOR
#undef REFERENCE_VECTOR_CONST_ITERATOR

}   /* sequence_detail */

// SPECIALIZATION
// --------------

template <
    typename T,
    typename Alloc,
    template <typename, typename> class Container
>
struct is_relocatable<sequence_detail::intrusive_vector_base<T, Alloc, Container>>:
    is_relocatable<typename sequence_detail::intrusive_vector_base<T, Alloc, Container>::container_type>
{};

// OBJECTS
// -------

/**
 *  \brief Vector wrapper that handles reference values.
 */
template <
    typename T,
    typename Alloc = allocator<T*>,
    template <typename, typename> class Container = vector
>
using intrusive_vector = sequence_detail::intrusive_vector_base<T, Alloc, Container>;


PYCPP_END_NAMESPACE
