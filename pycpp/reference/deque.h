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
    using value_type = typename std::remove_pointer<T>::type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef sequence_iterator_impl<typename std::deque<pointer>::iterator> iterator;
    typedef sequence_iterator_impl<typename std::deque<pointer>::const_iterator> const_iterator;
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

// TODO: implement...

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
