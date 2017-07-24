//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief any unittests.
 */

#include <pycpp/collections/any.h>
#include <gtest/gtest.h>
#include <tuple>

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
    any x = 4;
    any y = big_type();
    any z = 6;

    EXPECT_FALSE(any().has_value());
    EXPECT_TRUE(any(1).has_value());
    EXPECT_TRUE(any(big_type()).has_value());

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
    EXPECT_EQ(any(4).type(), typeid(int));
    EXPECT_EQ(any(big_type()).type(), typeid(big_type));
    EXPECT_EQ(any(1.5f).type(), typeid(float));
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
        any_cast<int>(any(4.0f));
    } catch(const bad_any_cast&) {
        except1 = true;
    }

    try {
        any_cast<float>(any(4.0f));
    } catch(const bad_any_cast&) {
        except2 = true;
    }

    try {
        any_cast<float>(any(big_type()));
    } catch(const bad_any_cast&) {
        except3 = true;
    }

    try {
        any_cast<big_type>(any(big_type()));
    } catch(const bad_any_cast&) {
        except4 = true;
    }

    EXPECT_TRUE(except0);
    EXPECT_TRUE(except1 == true && except2 == false);
    EXPECT_TRUE(except3 == true && except4 == false);
}


TEST(any, any_cast)
{
    any i4 = 4;
    any i5 = 5;
    any f6 = 6.0f;
    any big1 = big_type();
    any big2 = big_type();
    any big3 = big_type();

    EXPECT_NE(any_cast<int>(&i4), nullptr);
    EXPECT_EQ(any_cast<float>(&i4), nullptr);
    EXPECT_EQ(any_cast<int>(i5), 5);
    EXPECT_EQ(any_cast<float>(f6), 6.0f);
}


TEST(any, reference_count)
{
    std::shared_ptr<int> ptr_count(new int);
    std::weak_ptr<int> weak = ptr_count;
    any p0 = 0;

    EXPECT_EQ(weak.use_count(), 1);
    any p1 = ptr_count;
    EXPECT_EQ(weak.use_count(), 2);
    any p2 = p1;
    EXPECT_EQ(weak.use_count(), 3);
    p0 = p1;
    EXPECT_EQ(weak.use_count(), 4);
    p0 = 0;
    EXPECT_EQ(weak.use_count(), 3);
    p0 = std::move(p1);
    EXPECT_EQ(weak.use_count(), 3);
    p0.swap(p1);
    EXPECT_EQ(weak.use_count(), 3);
    p0 = 0;
    EXPECT_EQ(weak.use_count(), 3);
    p1.reset();
    EXPECT_EQ(weak.use_count(), 2);
    p2 = any(big_type());
    EXPECT_EQ(weak.use_count(), 1);
    p1 = ptr_count;
    EXPECT_EQ(weak.use_count(), 2);
    ptr_count = nullptr;
    EXPECT_EQ(weak.use_count(), 1);
    p1 = any();
    EXPECT_EQ(weak.use_count(), 0);
}
