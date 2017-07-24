//  :copyright: (c) 2011-2012 Andrzej Krzemienski.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Optional type unittests.
 */

#include <pycpp/collections/optional.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(optional, disengaged_ctor)
{
    optional<int> o1;
    EXPECT_FALSE(o1);

    optional<int> o2 = nullopt;
    EXPECT_FALSE(o2);

    optional<int> o3 = o2;
    EXPECT_FALSE(o3);

    EXPECT_EQ(o1, nullopt);
    EXPECT_EQ(o1, optional<int>{});
    EXPECT_FALSE(o1);
    EXPECT_FALSE(bool(o1));

    EXPECT_EQ(o2, nullopt);
    EXPECT_EQ(o2, optional<int>{});
    EXPECT_FALSE(o2);
    EXPECT_FALSE(bool(o2));

    EXPECT_EQ(o3, nullopt);
    EXPECT_EQ(o3, optional<int>{});
    EXPECT_FALSE(o3);
    EXPECT_FALSE(bool(o3));

    EXPECT_EQ(o1, o2);
    EXPECT_EQ(o2, o1);
    EXPECT_EQ(o1, o3);
    EXPECT_EQ(o3, o1);
    EXPECT_EQ(o2, o3);
    EXPECT_EQ(o3, o2);
}


// TODO: finish the rest of the unittests
// https://github.com/akrzemi1/Optional/blob/master/test_optional.cpp
