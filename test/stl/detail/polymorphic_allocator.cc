//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(polymorphic, is_relocatable)
{
    using allocator_type = polymorphic_allocator<char>;
    static_assert(is_relocatable<memory_resource>::value, "");
    static_assert(is_relocatable<allocator_type>::value, "");
}


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

    EXPECT_THROW(allocator.allocate(50), bad_alloc);
}


TEST(polymorphic_allocator, vector)
{
    using allocator_type = polymorphic_allocator<int>;
    using vector_type = vector<int, allocator_type>;

    vector_type v1;
    v1.emplace_back(1);
}
