//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/null.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(null, is_relocatable)
{
    using allocator_type = null_allocator<char>;
    using resource_type = null_resource;
    static_assert(is_relocatable<allocator_type>::value, "");
    static_assert(is_relocatable<resource_type>::value, "");
}


TEST(null_allocator, null_allocator)
{
    using allocator_type = null_allocator<char>;
    allocator_type allocator;

    EXPECT_THROW(allocator.allocate(50), bad_alloc);
    EXPECT_THROW(allocator.allocate(1), bad_alloc);
}
