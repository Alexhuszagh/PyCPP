//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/crt.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct non_relocatable
{
    int x = 5;
    int* ptr = &x;

    non_relocatable(int y = 5):
        x(y), ptr(&x)
    {}

    non_relocatable(const non_relocatable& rhs):
        non_relocatable(rhs.x)
    {}

    non_relocatable& operator=(const non_relocatable& rhs)
    {
        x = rhs.x;
        return *this;
    }

    non_relocatable(non_relocatable&& rhs):
        non_relocatable(rhs.x)
    {}

    non_relocatable& operator=(non_relocatable&& rhs)
    {
        x = rhs.x;
        return *this;
    }
};

// TESTS
// -----


TEST(crt_allocator, crt_allocator)
{
    using allocator_type = crt_allocator<char>;
    allocator_type allocator;

    char* ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);

    ptr = allocator.allocate(50);
    allocator.deallocate(ptr, 50);
}


TEST(crt_allocator, relocatable)
{
    using allocator_type = crt_allocator<char>;
    allocator_type allocator;

    char* ptr = allocator.allocate(50);
    new (static_cast<void*>(ptr)) char('c');
    ptr = allocator.reallocate(ptr, 50, 100);
    EXPECT_EQ(ptr[0], 'c');
    allocator.deallocate(ptr, 100);
}


TEST(crt_allocator, non_relocatable)
{
    using allocator_type = crt_allocator<non_relocatable>;
    allocator_type allocator;

    // check allocate
    non_relocatable* ptr = allocator.allocate(50);
    new (static_cast<void*>(ptr)) non_relocatable();
    EXPECT_EQ(ptr[0].x, 5);
    EXPECT_EQ(ptr[0].ptr, &ptr[0].x);

    // check reallocate
    ptr = allocator.reallocate(ptr, 50, 100);
    EXPECT_EQ(ptr[0].x, 5);
    EXPECT_EQ(ptr[0].ptr, &ptr[0].x);
    allocator.deallocate(ptr, 100);
}


TEST(crt_allocator, vector)
{
    using allocator_type = crt_allocator<char>;
    using vector = vector<char, allocator_type>;

    vector v1;
    v1.emplace_back(1);
}


TEST(crt_allocator, polymorphic)
{
    using allocator_type = polymorphic_allocator<int>;
    using resource_type = crt_resource;
    using vector = vector<int, allocator_type>;

    resource_type resource;
    vector v1 = vector(allocator_type(&resource));
    v1.emplace_back(1);
}
