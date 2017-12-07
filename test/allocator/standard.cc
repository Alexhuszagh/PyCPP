//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/standard.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(standard_allocator, standard_allocator)
{
    using allocator_type = standard_allocator<char>;
    allocator_type allocator;

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(standard_allocator, vector)
{
    using allocator_type = standard_allocator<char>;
    using vector = std::vector<char, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}
