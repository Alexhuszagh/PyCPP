//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief B-tree map unittests.
 */

#include <pycpp/collections/btree_map.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(btree_map, constructor_null)
{
    using map = btree_map<int, int>;
    map m1, m2;

    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_TRUE(m1.find(1) == m1.end());
    EXPECT_TRUE(m2.find(1) == m2.end());

    m1.insert(std::make_pair(1, 1));
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_TRUE(m1.find(1) != m1.end());
    EXPECT_TRUE(m2.find(1) == m2.end());
}
