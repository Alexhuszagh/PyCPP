//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for the stack allocator arena.
 *
 *  Base class to ensure arena destruction occurs **after** STL
 *  container destruction
 */

#pragma once

#include <pycpp/allocator/stack.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

template <typename T, size_t StackSize = 4096>
struct fixed_arena
{
    // MEMBER TYPES
    // ------------
    using allocator_type = stack_allocator<T, StackSize>;
    using arena_type = typename allocator_type::arena_type;

    // OBSERVERS
    const arena_type& get_arena() const noexcept;

    // STACK
    size_t stack_used() const noexcept;
    static size_t stack_size() noexcept;
    double stack_percent_used() const noexcept;

protected:
    static_assert(StackSize > 0, "Stack size must be a positive value.");
    arena_type arena_;
};

// SPECIALIZATION
// --------------

template <typename T, size_t StackSize>
struct is_relocatable<fixed_arena<T, StackSize>>:
    is_relocatable<typename fixed_arena<T, StackSize>::arena_type>
{};

// IMPLEMENTATION
// --------------

template <typename T, size_t StackSize>
auto fixed_arena<T, StackSize>::get_arena() const noexcept -> const arena_type&
{
    return arena_;
}


template <typename T, size_t StackSize>
size_t fixed_arena<T, StackSize>::stack_used() const noexcept
{
    return get_arena().used();
}


template <typename T, size_t StackSize>
size_t fixed_arena<T, StackSize>::stack_size() noexcept
{
    return StackSize;
}


template <typename T, size_t StackSize>
double fixed_arena<T, StackSize>::stack_percent_used() const noexcept
{
    return static_cast<double>(stack_used()) / stack_size();
}

PYCPP_END_NAMESPACE
