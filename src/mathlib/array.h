//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-performance array, analogous to a NumPy array.
 */

#pragma once

#include <mathlib/axis.h>
#include <vector>

// DECLARATIONS
// ------------

// Need to consider how to create an NDArray


/**
 *  \brief N-dimensional array type.
 */
template <typename T, typename Allocator = std::allocator<T>>
class ndarray
{
public:
    // MEMBER TYPES
    // ------------
    typedef ndarray<T, Allocator> self;
    typedef ndarray_axis<T> axis_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
    // TODO: need iterators...
    typedef Allocator allocator_type;

    // MEMBER FUNCTIONS
    // ----------------
    ndarray() = default;
    ndarray(const self& arr);
    self& operator=(const self& arr);
    ndarray(self&& arr);
    self& operator=(self&& arr);

    // TODO: need operator*
    // TODO: need operator/
    // TODO: need operator+
    // TODO: need operator-

    // TODO: need to be able to get by axis...

private:
    std::vector<value_type, allocator_type> vector_;
    std::vector<size_t> dims_ = {0};
};
