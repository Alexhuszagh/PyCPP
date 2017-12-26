//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `any` unittests.
 */

#include <pycpp/stl/any.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct big_type
{
    char value[256];
};

// TESTS
// -----


TEST(any, ctor)
{
    any x = make_any<int>(4);
    any y = make_any<big_type>();
    any z = make_any<int>(6);

    EXPECT_FALSE(any().has_value());
    EXPECT_TRUE(make_any<int>(1).has_value());
    EXPECT_TRUE(make_any<big_type>().has_value());

    EXPECT_TRUE(x.has_value() && y.has_value() && z.has_value());
    y.reset();
    EXPECT_TRUE(x.has_value() && !y.has_value() && z.has_value());
    x = y;
    EXPECT_TRUE(!x.has_value() && !y.has_value() && z.has_value());
    z = any();
    EXPECT_TRUE(!x.has_value() && !y.has_value() && !z.has_value());
}


TEST(any, type)
{
    EXPECT_EQ(any().type(), typeid(void));
    EXPECT_EQ(make_any<int>(4).type(), typeid(int));
    EXPECT_EQ(make_any<big_type>().type(), typeid(big_type));
    EXPECT_EQ(make_any<float>(1.5f).type(), typeid(float));
}


TEST(any, bad_any_cast)
{
    bool except0 = false;
    bool except1 = false;
    bool except2 = false;
    bool except3 = false;
    bool except4 = false;

    try {
        any_cast<int>(any());
    } catch(const bad_any_cast&) {
        except0 = true;
    }

    try {
        any_cast<int>(make_any<float>(4.0f));
    } catch(const bad_any_cast&) {
        except1 = true;
    }

    try {
        any_cast<float>(make_any<float>(4.0f));
    } catch(const bad_any_cast&) {
        except2 = true;
    }

    try {
        any_cast<float>(make_any<big_type>());
    } catch(const bad_any_cast&) {
        except3 = true;
    }

    try {
        any_cast<big_type>(make_any<big_type>());
    } catch(const bad_any_cast&) {
        except4 = true;
    }

    EXPECT_TRUE(except0);
    EXPECT_TRUE(except1 == true && except2 == false);
    EXPECT_TRUE(except3 == true && except4 == false);
}


TEST(any, any_cast)
{
    any i4 = make_any<int>(4);
    any i5 = make_any<int>(5);
    any f6 = make_any<float>(6.0f);
    any big1 = make_any<big_type>();
    any big2 = make_any<big_type>();
    any big3 = make_any<big_type>();

    EXPECT_NE(any_cast<int>(&i4), nullptr);
    EXPECT_EQ(any_cast<float>(&i4), nullptr);
    EXPECT_EQ(any_cast<int>(i5), 5);
    EXPECT_EQ(any_cast<float>(f6), 6.0f);
}


TEST(any, reference_count)
{
    shared_ptr<int> ptr_count(new int);
    weak_ptr<int> weak = ptr_count;
    any p0 = make_any<int>(0);

    EXPECT_EQ(weak.use_count(), 1);
    any p1 = make_any<shared_ptr<int>>(ptr_count);
    EXPECT_EQ(weak.use_count(), 2);
    any p2 = p1;
    EXPECT_EQ(weak.use_count(), 3);
    p0 = p1;
    EXPECT_EQ(weak.use_count(), 4);
    p0.emplace<int>(0);
    EXPECT_EQ(weak.use_count(), 3);
    p0 = move(p1);
    EXPECT_EQ(weak.use_count(), 3);
    p0.swap(p1);
    EXPECT_EQ(weak.use_count(), 3);
    p0.emplace<int>(0);
    EXPECT_EQ(weak.use_count(), 3);
    p1.reset();
    EXPECT_EQ(weak.use_count(), 2);
    p2 = make_any<big_type>();
    EXPECT_EQ(weak.use_count(), 1);
    p1.emplace<shared_ptr<int>>(ptr_count);
    EXPECT_EQ(weak.use_count(), 2);
    ptr_count = nullptr;
    EXPECT_EQ(weak.use_count(), 1);
    p1 = any();
    EXPECT_EQ(weak.use_count(), 0);

    exit(0);
}
