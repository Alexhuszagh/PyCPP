//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size list implementation.
 *
 *  Based on the EASTL, this list implementation preallocates a fixed-
 *  size buffer on the stack for allocation up until a certain size,
 *  and uses dynamic memory allocation afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/list.h>
#include <pycpp/stl/initializer_list.h>


PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Fixed-sized list that preallocates memory on the stack.
 *
 *  By default, the list preallocates ~4096 bytes, which can hold
 *  roughly `4096 / (sizeof(T) + 2*sizeof(uintrptr_t))` elements.
 */
template <
    typename T,
    size_t StackSize = 4096,
    template <typename, typename> class Container = list
>
struct fixed_list:
    fixed_arena<T, StackSize>,
    Container<T, stack_allocator<T, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_list<T, StackSize, Container>;
    using container_type = Container<T, stack_allocator<T, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    fixed_list();
    fixed_list(const self_t&);
    fixed_list(self_t&&) = delete;
    fixed_list(size_t n, const value_type& value = value_type());
    template <typename Iter> fixed_list(Iter first, Iter last);
    fixed_list(initializer_list<value_type> list);
    self_t& operator=(const self_t&);
    self_t& operator=(self_t&&) = delete;
    self_t& operator=(initializer_list<value_type> list);

private:
    void reset();
};

// IMPLEMENTATION
// --------------

template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_list<T, StackSize, _>::fixed_list():
    container_type(allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_list<T, StackSize, _>::fixed_list(const self_t& rhs):
    container_type(rhs, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_list<T, StackSize, _>::fixed_list(size_t n, const value_type& value):
    container_type(n, value, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
template <typename Iter>
fixed_list<T, StackSize, _>::fixed_list(Iter first, Iter last):
    container_type(first, last, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_list<T, StackSize, _>::fixed_list(initializer_list<value_type> list):
    container_type(list.begin(), list.end(), allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_list<T, StackSize, _>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        reset();
        this->assign(rhs.begin(), rhs.end());
    }
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_list<T, StackSize, _>::operator=(initializer_list<value_type> list) -> self_t&
{
    reset();
    this->assign(list.begin(), list.end());
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
void fixed_list<T, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->arena_.reset();
}

PYCPP_END_NAMESPACE
