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
    allocator.construct(ptr, 0);
    allocator.destroy(ptr);
    allocator.deallocate(ptr, 50);
}
