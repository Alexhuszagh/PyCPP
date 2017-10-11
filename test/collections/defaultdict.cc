//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Default dict unittests.
 */

#include <pycpp/collections/defaultdict.h>
#include <gtest/gtest.h>
#include <memory>

PYCPP_USING_NAMESPACE

// DATA
// ----

using int_ptr = std::shared_ptr<int>;


struct nonconstructable_int
{
public:
    nonconstructable_int() = delete;
    nonconstructable_int(int x): x(x) {};

private:
    int x;
};


int_ptr default_int()
{
    return std::make_shared<int>(0);
}


nonconstructable_int default_nonconstructable_int()
{
    return nonconstructable_int(0);
}

// TESTS
// -----

// DEFAULT MAP

TEST(default_map, constructor)
{
    using intmap = default_map<int, int_ptr>;
    using classmap = default_map<int, nonconstructable_int>;

    intmap m1;
    intmap m2(default_int);
    classmap m3(default_nonconstructable_int);
    intmap m4(m2);
    intmap m5 = m4;
    intmap m6(std::move(m4));
    intmap m7 = std::move(m5);
    classmap m8(m3);
    classmap m9 = m8;
    classmap m10(std::move(m8));
    classmap m11 = std::move(m9);

    intmap m12({{0, default_int()}}, default_int);
    intmap m13(m12.begin(), m12.end(), default_int);
    classmap m14({{0, default_nonconstructable_int()}}, default_nonconstructable_int);
    classmap m15(m14.begin(), m14.end(), default_nonconstructable_int);
}


TEST(default_map, capacity)
{
    using intmap = default_map<int, int_ptr>;

    intmap m1;
    EXPECT_EQ(m1.size(), 0);
    EXPECT_TRUE(m1.empty());
    EXPECT_GT(m1.max_size(), 0);

    intmap m2({{0, default_int()}}, default_int);
    EXPECT_EQ(m2.size(), 1);
    EXPECT_FALSE(m2.empty());
    EXPECT_GT(m2.max_size(), 1);
}


TEST(default_map, iterators)
{}


TEST(default_map, element_access)
{}


TEST(default_map, modifiers)
{}


TEST(default_map, observers)
{}


TEST(default_map, operators)
{}


// DEFAULT UNORDERED MAP