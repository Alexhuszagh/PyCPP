//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/detail/polymorphic_allocator.h>
#include <gtest/gtest.h>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(polymorphic_allocator, polymorphic_allocator)
{
    using allocator_type = polymorphic_allocator<char>;
    allocator_type allocator;

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(polymorphic_allocator, new_delete_resource)
{
    using allocator_type = polymorphic_allocator<char>;
    allocator_type allocator(new_delete_resource());

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(polymorphic_allocator, null_memory_resource)
{
    using allocator_type = polymorphic_allocator<char>;
    allocator_type allocator(null_memory_resource());

    EXPECT_THROW(allocator.allocate(50), std::bad_alloc);
}


TEST(polymorphic_allocator, vector)
{
    using allocator_type = polymorphic_allocator<int>;
    using vector = std::vector<int, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}
