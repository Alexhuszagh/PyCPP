//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance array, analogous to a NumPy array.
 */

#pragma once

#include <pycpp/ndarray/array_fwd.h>
#include <pycpp/ndarray/view.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATIONS
// ------------


/**
 *  \brief N-dimensional array type.
 */
template <typename T, typename Allocator>
class ndarray
{
public:
    // MEMBER TYPES
    // ------------
    typedef ndarray<T, Allocator> self;
    typedef std::vector<T, Allocator> vector_type;
    typedef ndarray_view<T> view_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef typename view_type::iterator iterator;
    typedef typename view_type::const_iterator const_iterator;
    typedef typename view_type::reverse_iterator reverse_iterator;
    typedef typename view_type::const_reverse_iterator const_reverse_iterator;
    typedef Allocator allocator_type;

    // MEMBER FUNCTIONS
    // ----------------
    ndarray() = default;
    ndarray(const self&) = default;
    self& operator=(const self&) = default;
    ndarray(self&&) = default;
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

    // PROPERTIES

    // OBSERVERS
    view_type view() const;
    allocator_type get_allocator() const;

private:
    vector_type data_;
    ndarray_dimensions dims_;
};


// DEFINITIONS
// -----------


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
size_t ndarray<T, A>::size() const noexcept
{
    return view().size();
}


template <typename T, typename A>
size_t ndarray<T, A>::max_size() const noexcept
{
    return view().max_size();
}


template <typename T, typename A>
size_t ndarray<T, A>::capacity() const noexcept
{
    return view().capacity();
}


template <typename T, typename A>
size_t ndarray<T, A>::empty() const noexcept
{
    return view().empty();
}


template <typename T, typename A>
size_t ndarray<T, A>::ndims() const noexcept
{
    return view().ndims();
}

template <typename T, typename A>
const ndarray_dimension_view& ndarray<T, A>::shape() const noexcept
{
    return view().shape();
}

PYCPP_END_NAMESPACE
