//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size unordered_map implementation.
 *
 *  Based on the EASTL, this unordered_map and unordered_multimap implementation
 *  preallocates a fixed-size buffer on the stack for allocation
 *  up until a certain size, and uses dynamic memory allocation
 *  afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/unordered_map.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

// unordered_map

/**
 *  \brief Fixed-sized unordered_map that preallocates memory on the stack.
 */
template <
    typename Key,
    typename T,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename, typename, typename> class Container = unordered_map
>
struct fixed_unordered_map:
    fixed_arena<pair<const Key, T>, StackSize>,
    Container<Key, T, Hash, Pred, stack_allocator<pair<const Key, T>, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_unordered_map<Key, T, Hash, Pred, StackSize, Container>;
    using container_type = Container<Key, T, Hash, Pred, stack_allocator<pair<const Key, T>, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: here...

private:
    void reset();
};

// unordered_multimap

/**
 *  \brief Fixed-sized unordered_multimap that preallocates memory on the stack.
 */
template <
    typename Key,
    typename T,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    size_t StackSize = 4096,
    template <typename, typename, typename, typename, typename> class Container = unordered_multimap
>
struct fixed_unordered_multimap:
    fixed_arena<pair<const Key, T>, StackSize>,
    Container<Key, T, Hash, Pred, stack_allocator<pair<const Key, T>, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_unordered_multimap<Key, T, Hash, Pred, StackSize, Container>;
    using container_type = Container<Key, T, Hash, Pred, stack_allocator<pair<const Key, T>, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: here...

private:
    void reset();
};

// SPECIALIZATION
// --------------

template <
    typename Key,
    typename T,
    typename Hash,
    typename Pred,
    size_t StackSize,
    template <typename, typename, typename, typename, typename> class Container
>
struct is_relocatable<fixed_unordered_map<Key, T, Hash, Pred, StackSize, Container>>: bool_constant<
        is_relocatable<typename fixed_unordered_map<Key, T, Hash, Pred, StackSize, Container>::arena_type>::value &&
        is_relocatable<typename fixed_unordered_map<Key, T, Hash, Pred, StackSize, Container>::container_type>::value
    >
{};

template <
    typename Key,
    typename T,
    typename Hash,
    typename Pred,
    size_t StackSize,
    template <typename, typename, typename, typename, typename> class Container
>
struct is_relocatable<fixed_unordered_multimap<Key, T, Hash, Pred, StackSize, Container>>: bool_constant<
        is_relocatable<typename fixed_unordered_multimap<Key, T, Hash, Pred, StackSize, Container>::arena_type>::value &&
        is_relocatable<typename fixed_unordered_multimap<Key, T, Hash, Pred, StackSize, Container>::container_type>::value
    >
{};

// IMPLEMENTATION
// --------------

// unordered_map

template <typename Key, typename T, typename Hash, typename Pred, size_t StackSize, template <typename, typename, typename, typename, typename> class _>
void fixed_unordered_map<Key, T, Hash, Pred, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    // do not reset the arena, since that is **undefined behavior**
    this->clear();
}

// unordered_multimap

template <typename Key, typename T, typename Hash, typename Pred, size_t StackSize, template <typename, typename, typename, typename, typename> class _>
void fixed_unordered_multimap<Key, T, Hash, Pred, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    // do not reset the arena, since that is **undefined behavior**
    this->clear();
}

PYCPP_END_NAMESPACE
