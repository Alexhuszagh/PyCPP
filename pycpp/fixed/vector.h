//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size vector implementation.
 *
 *  Based on the EASTL, this vector implementation preallocates a fixed-
 *  size buffer on the stack for allocation up until a certain size,
 *  and uses dynamic memory allocation afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Fixed-sized vector that preallocates memory on the stack.
 *
 *  By default, the vector preallocates ~4096 bytes, which can hold
 *  rouhgly `4096 / sizeof(T)` elements.
 */
template <
    typename T,
    size_t StackSize = 4096,
    template <typename, typename> class Container = vector
>
struct fixed_vector:
    fixed_arena<T, StackSize>,
    Container<T, stack_allocator<T, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_vector<T, StackSize, Container>;
    using base_t = fixed_arena<T, StackSize>;
    using container_type = Container<T, stack_allocator<T, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    fixed_vector();
    fixed_vector(const self_t&);
    fixed_vector(self_t&&) = delete;
    fixed_vector(size_t n, const value_type& value = value_type());
    template <typename Iter> fixed_vector(Iter first, Iter last);
    fixed_vector(initializer_list<value_type> list);
    self_t& operator=(const self_t&);
    self_t& operator=(self_t&&) = delete;
    self_t& operator=(initializer_list<value_type> list);

private:
    void reset();
};

// IMPLEMENTATION
// --------------

template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_vector<T, StackSize, _>::fixed_vector():
    base_t(),
    container_type(allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_vector<T, StackSize, _>::fixed_vector(const self_t& rhs):
    base_t(),
    container_type(rhs, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_vector<T, StackSize, _>::fixed_vector(size_t n, const value_type& value):
    container_type(n, value, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
template <typename Iter>
fixed_vector<T, StackSize, _>::fixed_vector(Iter first, Iter last):
    container_type(first, last, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_vector<T, StackSize, _>::fixed_vector(initializer_list<value_type> list):
    container_type(list.begin(), list.end(), allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_vector<T, StackSize, _>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        reset();
        this->assign(rhs.begin(), rhs.end());
    }
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_vector<T, StackSize, _>::operator=(initializer_list<value_type> list) -> self_t&
{
    reset();
    this->assign(list.begin(), list.end());
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
void fixed_vector<T, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->shrink_to_fit();
    this->arena_.reset();
}

PYCPP_END_NAMESPACE
