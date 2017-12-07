//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/stack.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(stack_allocator, stack_allocator)
{
    using allocator_type = stack_allocator<char, 200>;
    using arena_type = typename allocator_type::arena_type;
    arena_type arena;
    allocator_type allocator(arena);

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    // allocate larger than buffer
    ptr = allocator.allocate(250);
    ASSERT_NE(ptr, nullptr);
    allocator.deallocate(ptr, 250);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(stack_allocator, vector)
{
    using allocator_type = stack_allocator<char, 200>;
    using vector = std::vector<char, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}
