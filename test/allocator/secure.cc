//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/secure.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(secure, is_relocatable)
{
    using allocator_type = secure_allocator<char>;
    using resource_type = secure_resource;
    static_assert(is_relocatable<allocator_type>::value, "");
    static_assert(is_relocatable<resource_type>::value, "");
}


TEST(secure_allocator, secure_allocator)
{
    using allocator_type = secure_allocator<char>;
    allocator_type allocator;

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(secure_allocator, vector)
{
    using allocator_type = secure_allocator<char>;
    using vector = vector<char, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}


TEST(secure_allocator, polymorphic)
{
    using allocator_type = polymorphic_allocator<int>;
    using resource_type = secure_resource;
    using vector = vector<int, allocator_type>;

    resource_type resource;
    vector v1 = vector(allocator_type(&resource));
    v1.emplace_back(1);
}
