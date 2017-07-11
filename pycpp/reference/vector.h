//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Reference-wrapper like vector.
 */

#pragma once

#include <type_traits>
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
    using value_type = typename std::remove_pointer<T>::type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
// TODO: need to transform it..
//    typedef pointer iterator;
//    typedef const_pointer const_iterator;
//    typedef std::reverse_iterator<pointer> reverse_iterator;
//    typedef std::reverse_iterator<const_pointer> const_reverse_iterator;

//    // ITERATORS
//    iterator begin() const;
//    const_iterator begin() const;
//    const_iterator end() const;
//    const_reverse_iterator rbegin() const;
//    const_reverse_iterator rend() const;
//    const_iterator cbegin() const;
//    const_iterator cend() const;
//    const_reverse_iterator crbegin() const;
//    const_reverse_iterator crend() const;

    // CAPACITY
    size_type size() const;
    size_type max_size() const;
    // resize
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
//    data
//    Access data (public member function )

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
