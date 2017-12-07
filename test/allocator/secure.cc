//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/secure.h>
#include <gtest/gtest.h>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----


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
    using vector = std::vector<char, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}
