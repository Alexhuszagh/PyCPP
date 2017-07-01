//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <secure/allocator.h>
#include <gtest/gtest.h>


// TESTS
// -----


TEST(secure_allocator, secure_allocator)
{
    typedef secure_allocator<char> alloc;
    char* ptr = alloc().allocate(50);
    alloc().deallocate(ptr, 50);

    ptr = alloc().allocate(50);
    alloc().construct(ptr, 0);
    alloc().destroy(ptr);
    alloc().deallocate(ptr, 50);
}
