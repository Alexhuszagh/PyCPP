//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Base class implementation for fixed-size containers.
 */

#pragma once

#include <pycpp/allocator/stack.h>
#include <initializer_list>
#include <vector>


PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Fixed-sized vector that preallocates memory on the stack.
 *
 *  By default, the vector preallocates ~4096 bytes, which can hold
 *  rouhgly `4096 / sizeof(T)` elements.
 */
template <typename T, size_t StackSize = 4096>
struct fixed_vector: std::vector<T, stack_allocator<T, StackSize>>
{
public:
    // MEMBER VARIABLES
    // ----------------
    static constexpr size_t stack_size = StackSize;

    // MEMBER TYPES
    // ------------
    using self_t = fixed_vector<T, StackSize>;
    using base_t = std::vector<T, stack_allocator<T, StackSize>>;
    using value_type = typename base_t::value_type;
    using allocator_type = typename base_t::allocator_type;

    // MEMBER FUNCTIONS
    // ----------------
    fixed_vector();
    fixed_vector(const self_t&);
    fixed_vector(self_t&&) = delete;
    fixed_vector(size_t n, const value_type& value = value_type());
    template <typename Iter> fixed_vector(Iter first, Iter last);
    fixed_vector(std::initializer_list<value_type> list);
    self_t& operator=(const self_t&);
    self_t& operator=(self_t&&) = delete;
    self_t& operator=(std::initializer_list<value_type> list);

private:
    void reset();

    typename allocator_type::arena_type arena;
};

// IMPLEMENTATION
// --------------


template <typename T, size_t StackSize>
fixed_vector<T, StackSize>::fixed_vector():
    base_t(allocator_type(arena))
{}


template <typename T, size_t StackSize>
fixed_vector<T, StackSize>::fixed_vector(const self_t& rhs):
    base_t(rhs, allocator_type(arena))
{}


template <typename T, size_t StackSize>
fixed_vector<T, StackSize>::fixed_vector(size_t n, const value_type& value):
    base_t(n, value, allocator_type(arena))
{}


template <typename T, size_t StackSize>
template <typename Iter>
fixed_vector<T, StackSize>::fixed_vector(Iter first, Iter last):
    base_t(first, last, allocator_type(arena))
{}


template <typename T, size_t StackSize>
fixed_vector<T, StackSize>::fixed_vector(std::initializer_list<value_type> list):
    base_t(list.begin(), list.end(), allocator_type(arena))
{}


template <typename T, size_t StackSize>
auto fixed_vector<T, StackSize>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        reset();
        this->assign(rhs.begin(), rhs.end());
    }
    return *this;
}


template <typename T, size_t StackSize>
auto fixed_vector<T, StackSize>::operator=(std::initializer_list<value_type> list) -> self_t&
{
    reset();
    this->assign(list.begin(), list.end());
    return *this;
}


template <typename T, size_t StackSize>
void fixed_vector<T, StackSize>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->shrink_to_fit();
    arena.reset();
}

PYCPP_END_NAMESPACE
