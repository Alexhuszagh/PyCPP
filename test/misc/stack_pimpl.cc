//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast Pimpl unittests.
 */

#include <pycpp/misc/stack_pimpl.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// FORWARD
// -------

struct structure;
struct int_wrapper;
using pstructure = stack_pimpl<structure, 8>;
using pint_wrapper = stack_pimpl<int_wrapper, sizeof(uintptr_t)>;

// OBJECTS
// -------

struct structure
{
    uint64_t x;
};


// Provide a class managing a resource.
// If the class properly initializes for the copy and move
// constructors and assignment operators, `x_` will properly
// have a new pointer when copied and therefore will properly
// copy any managed resources. If `x_` is not copied properly,
// and rather uses the same address before `stack_pimpl` copies
// it, then the copy is treating the memory block as a series
// of dumb bytes.
struct int_wrapper
{
    uint64_t* x_ = nullptr;
    int_wrapper(uint64_t x = 0);
    int_wrapper(const int_wrapper&);
    int_wrapper& operator=(const int_wrapper&);
    int_wrapper(int_wrapper&&);
    int_wrapper& operator=(int_wrapper&&);
    ~int_wrapper();
};


int_wrapper::int_wrapper(uint64_t x):
    x_(new uint64_t(x))
{}


int_wrapper::int_wrapper(const int_wrapper& rhs):
    x_(rhs.x_ ? new uint64_t(*rhs.x_) : nullptr)
{}


int_wrapper& int_wrapper::operator=(const int_wrapper& rhs)
{
    delete x_;
    x_ = rhs.x_ ? new uint64_t(*rhs.x_) : nullptr;
    return *this;
}


int_wrapper::int_wrapper(int_wrapper&& rhs)
{
    swap(x_, rhs.x_);
}


int_wrapper& int_wrapper::operator=(int_wrapper&& rhs)
{
    swap(x_, rhs.x_);
    return *this;
}


int_wrapper::~int_wrapper()
{
    delete x_;
#ifndef NDEBUG
    x_ = nullptr;
#endif
}

// TESTS
// -----


TEST(stack_pimpl, integer)
{
    using pint = stack_pimpl<int>;

    pint x(2);
    pint y;
    EXPECT_EQ(x, 2);

    x = 5;
    EXPECT_EQ(x, 5);

    y = x;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 5);

    y = 2;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 2);

    swap(x, y);
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 5);
}


TEST(stack_pimpl, structure)
{
    pstructure a = structure {2};
    pstructure b;
    EXPECT_EQ(a->x, 2);

    a->x = 5;
    EXPECT_EQ(a->x, 5);

    b = a;
    EXPECT_EQ(a->x, 5);
    EXPECT_EQ(b->x, 5);

    b->x = 2;
    EXPECT_EQ(a->x, 5);
    EXPECT_EQ(b->x, 2);

    swap(a, b);
    EXPECT_EQ(a->x, 2);
    EXPECT_EQ(b->x, 5);
}


TEST(stack_pimpl, int_wrapper)
{
    pint_wrapper a(int_wrapper {2});    // move ctor
    pint_wrapper b;

    // check null value is properly initalized
    ASSERT_TRUE(b->x_ != nullptr);
    b = int_wrapper {1};                // move assignment

    // check values are properly initialized
    ASSERT_TRUE(a->x_ != nullptr);
    ASSERT_TRUE(b->x_ != nullptr);
    EXPECT_EQ(*a->x_, 2);
    EXPECT_EQ(*b->x_, 1);
    EXPECT_NE(a->x_, b->x_);

    // check copy assignment and ctor
    b = a;
    pint_wrapper c(b);
    ASSERT_TRUE(b->x_ != nullptr);
    ASSERT_TRUE(c->x_ != nullptr);
    EXPECT_EQ(*b->x_, 2);
    EXPECT_EQ(*c->x_, 2);
    EXPECT_NE(a->x_, b->x_);
    EXPECT_NE(a->x_, c->x_);
    EXPECT_NE(b->x_, c->x_);
}
