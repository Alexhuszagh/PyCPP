//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Sorted sequence unittests.
 */

#include <pycpp/collections/sorted_sequence.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(sorted_sequence, constructor)
{
    using sequence_type = sorted_sequence<int>;

    sequence_type s1;
    sequence_type s2(s1);
    sequence_type s3 = s2;
    sequence_type s4(std::move(s2));
    sequence_type s5 = std::move(s3);

    sequence_type s6({4, 5});
    s6 = {3};
}


TEST(sorted_sequence, capacity)
{
    using sequence_type = sorted_sequence<int>;

    sequence_type s1;
    EXPECT_EQ(s1.size(), 0);
    EXPECT_TRUE(s1.empty());
    EXPECT_GT(s1.max_size(), 0);

    sequence_type s2({4, 5});
    EXPECT_EQ(s2.size(), 2);
    EXPECT_FALSE(s2.empty());
    EXPECT_GT(s2.max_size(), 2);
}


TEST(sorted_sequence, iterators)
{
    using sequence_type = sorted_sequence<int>;

    sequence_type s1({5, 4});
    EXPECT_EQ(*s1.begin(), 4);
    EXPECT_EQ(*s1.cbegin(), 4);
    EXPECT_EQ(*s1.rbegin(), 5);
    EXPECT_EQ(*s1.crbegin(), 5);
    EXPECT_EQ(std::distance(s1.begin(), s1.end()), 2);
    EXPECT_EQ(std::distance(s1.cbegin(), s1.cend()), 2);
    EXPECT_EQ(std::distance(s1.rbegin(), s1.rend()), 2);
    EXPECT_EQ(std::distance(s1.crbegin(), s1.crend()), 2);

    // should re-balance
    s1.emplace(1);
    EXPECT_EQ(*s1.begin(), 1);
    EXPECT_EQ(*s1.cbegin(), 1);
    EXPECT_EQ(*s1.rbegin(), 5);
    EXPECT_EQ(*s1.crbegin(), 5);
    EXPECT_EQ(std::distance(s1.begin(), s1.end()), 3);
    EXPECT_EQ(std::distance(s1.cbegin(), s1.cend()), 3);
    EXPECT_EQ(std::distance(s1.rbegin(), s1.rend()), 3);
    EXPECT_EQ(std::distance(s1.crbegin(), s1.crend()), 3);
}


TEST(sorted_sequence, element_access)
{
    using sequence_type = sorted_sequence<int>;
    sequence_type s1({5, 1, 4});

    EXPECT_EQ(s1.at(0), 1);
    EXPECT_THROW(s1.at(3), std::out_of_range);
    EXPECT_EQ(s1[0], 1);
    EXPECT_EQ(s1[1], 4);
    EXPECT_EQ(s1[2], 5);
    // operator[3] is undefined behavior
    EXPECT_EQ(s1.front(), 1);
    EXPECT_EQ(s1.back(), 5);
}


TEST(sorted_sequence, element_lookup)
{
    using sequence_type = sorted_sequence<int>;
    sequence_type s1({5, 1, 4});

    EXPECT_EQ(s1.find(0), s1.end());
    EXPECT_NE(s1.find(1), s1.end());
    EXPECT_EQ(s1.count(0), 0);
    EXPECT_EQ(s1.count(1), 1);
    EXPECT_NE(s1.lower_bound(5), s1.end());
    EXPECT_EQ(s1.upper_bound(5), s1.end());
    auto range = s1.equal_range(1);
    EXPECT_EQ(std::distance(range.first, range.second), 1);
}


TEST(sorted_sequence, modifiers)
{
    using sequence_type = sorted_sequence<int>;
    sequence_type s1;
    sequence_type s2({5, 1, 4});

    s2.emplace(1);
    s2.emplace_hint(s2.end(), 2);
    s2.insert(3);
    s2.insert(s2.end(), 4);
    s2.erase(s2.begin());
    s2.erase(1);
    s2.erase(s2.begin(), s2.end());
    s2.clear();
    s1.swap(s2);
}


TEST(sorted_sequence, observers)
{
    using sequence_type = sorted_sequence<int>;

    sequence_type s1;
    s1.get_allocator();
    s1.key_comp();
    s1.value_comp();
}


TEST(sorted_sequence, operators)
{
    using sequence_type = sorted_sequence<int>;
    sequence_type s1;
    sequence_type s2({5, 1, 4});

    EXPECT_FALSE(s1 == s2);
    EXPECT_TRUE(s1 != s2);
    EXPECT_TRUE(s1 < s2);
    EXPECT_TRUE(s1 <= s2);
    EXPECT_FALSE(s1 > s2);
    EXPECT_FALSE(s1 >= s2);
}
