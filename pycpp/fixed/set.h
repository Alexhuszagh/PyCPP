//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size set and multiset implementation.
 *
 *  Based on the EASTL, this set and multiset implementation
 *  preallocates a fixed-size buffer on the stack for allocation
 *  up until a certain size, and uses dynamic memory allocation
 *  afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/set.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// set

/**
 *  \brief Fixed-sized set that preallocates memory on the stack.
 */
template <
    typename Key,
    typename Compare = less<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename> class Container = set
>
struct fixed_set:
    fixed_arena<Key, StackSize>,
    Container<Key, Compare, stack_allocator<Key, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_set<Key, Compare, StackSize, Container>;
    using container_type = Container<Key, Compare, stack_allocator<Key, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: here...

private:
    void reset();
};

// multiset

/**
 *  \brief Fixed-sized multiset that preallocates memory on the stack.
 */
template <
    typename Key,
    typename Compare = less<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename> class Container = multiset
>
struct fixed_multiset:
    fixed_arena<Key, StackSize>,
    Container<Key, Compare, stack_allocator<Key, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_multiset<Key, Compare, StackSize, Container>;
    using container_type = Container<Key, Compare, stack_allocator<Key, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: here...

private:
    void reset();
};

// IMPLEMENTATION
// --------------

// set

template <typename Key, typename Compare, size_t StackSize, template <typename, typename, typename> class _>
void fixed_set<Key, Compare, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->arena_.reset();
}

// multiset

template <typename Key, typename Compare, size_t StackSize, template <typename, typename, typename> class _>
void fixed_multiset<Key, Compare, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->arena_.reset();
}

PYCPP_END_NAMESPACE
