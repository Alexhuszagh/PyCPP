//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Reference-wrapper like vector.
 */

#pragma once

#include <pycpp/iterator/transform.h>
#include <functional>
#include <type_traits>
#include <vector>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// DECLARATION
// -----------

template <typename P>
using deref = decltype(*std::declval<P>());

template <typename P>
using double_deref = deref<deref<P>>;

template <typename P>
using iterator_impl = transform_iterator<P, std::function<double_deref<P>(deref<P>)>>;


/**
 *  \brief Vector wrapper mapping value pointers to values.
 */
template <typename T>
struct reference_vector_base
{
    // MEMBER TYPES
    // ------------
    using value_type = typename std::remove_pointer<T>::type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef iterator_impl<pointer*> iterator;
    typedef iterator_impl<const_pointer*> const_iterator;
    typedef std::reverse_iterator<pointer> reverse_iterator;
    typedef std::reverse_iterator<const_pointer> const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    // TODO: here...

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
    void resize(size_type n);
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
    // assign
    void push_back(reference r);
    void pop_back();
    // insert
    // erase
    // swap

    // RELATIONAL OPERATORS
    // TODO: here...

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
auto reference_vector_base<T>::begin() -> iterator
{
    return iterator(vector_.data(), [](pointer& p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::begin() const -> const_iterator
{
    return const_iterator(vector_.data(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::cbegin() const -> const_iterator
{
    return const_iterator(vector_.data(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::end() -> iterator
{
    return iterator(vector_.data(), [](pointer p) -> reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::end() const -> const_iterator
{
    return const_iterator(vector_.data(), [](const_pointer p) -> const_reference {
        return *p;
    });
}


template <typename T>
auto reference_vector_base<T>::cend() const -> const_iterator
{
    return const_iterator(vector_.data(), [](const_pointer p) -> const_reference {
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
void reference_vector_base<T>::resize(size_type n)
{
    vector_.resize(n);
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
void reference_vector_base<T>::push_back(reference r)
{
    vector_.push_back(std::addressof(r));
}


template <typename T>
void reference_vector_base<T>::pop_back()
{
    vector_.pop_back();
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
};


PYCPP_END_NAMESPACE
