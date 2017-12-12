//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size unordered_set implementation.
 *
 *  Based on the EASTL, this unordered_set and unordered_multiset implementation
 *  preallocates a fixed-size buffer on the stack for allocation
 *  up until a certain size, and uses dynamic memory allocation
 *  afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/unordered_set.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// unordered_set

/**
 *  \brief Fixed-sized unordered_set that preallocates memory on the stack.
 */
template <
    typename Key,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename, typename> class Container = unordered_set
>
struct fixed_unordered_set:
    fixed_arena<Key, StackSize>,
    Container<Key, Hash, Pred, stack_allocator<Key, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_unordered_set<Key, Hash, Pred, StackSize, Container>;
    using container_type = Container<Key, Hash, Pred, stack_allocator<Key, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: here...

private:
    void reset();
};

// unordered_multiset

/**
 *  \brief Fixed-sized unordered_multiset that preallocates memory on the stack.
 */
template <
    typename Key,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename, typename> class Container = unordered_multiset
>
struct fixed_unordered_multiset:
    fixed_arena<Key, StackSize>,
    Container<Key, Hash, Pred, stack_allocator<Key, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_unordered_multiset<Key, Hash, Pred, StackSize, Container>;
    using container_type = Container<Key, Hash, Pred, stack_allocator<Key, StackSize>>;
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

// unordered_set

template <typename Key, typename Hash, typename Pred, size_t StackSize, template <typename, typename, typename, typename> class _>
void fixed_unordered_set<Key, Hash, Pred, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->arena_.reset();
}

// unordered_multiset

template <typename Key, typename Hash, typename Pred, size_t StackSize, template <typename, typename, typename, typename> class _>
void fixed_unordered_multiset<Key, Hash, Pred, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    this->clear();
    this->arena_.reset();
}

PYCPP_END_NAMESPACE
