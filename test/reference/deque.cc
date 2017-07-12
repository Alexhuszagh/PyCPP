//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Reference deque unittests.
 */

#include <pycpp/reference/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::deque<int> DATA = {1, 2, 3, 4, 5};

// TESTS
// -----


TEST(reference_deque, constructor)
{
    using reference = reference_deque<int&>;

    reference deque;
    EXPECT_EQ(deque.size(), 0);

    deque = reference(5, DATA[0]);
    EXPECT_EQ(deque.size(), 5);

    reference copy(deque);
    EXPECT_EQ(copy.size(), 5);

    copy = deque;
    EXPECT_EQ(copy.size(), 5);

    reference moved(std::move(deque));
    EXPECT_EQ(moved.size(), 5);

    moved = std::move(copy);
    EXPECT_EQ(moved.size(), 5);
}


TEST(reference_deque, iterator)
{
    using reference = reference_deque<int&>;

    // create deques
    reference deque;
    reference reversed;
    for (auto &item: DATA) {
        deque.push_back(item);
        reversed.push_front(item);
    }

    // check expected equality
    EXPECT_TRUE(std::equal(deque.begin(), deque.end(), DATA.begin()));
    EXPECT_TRUE(std::equal(deque.rbegin(), deque.rend(), DATA.rbegin()));
    EXPECT_TRUE(std::equal(reversed.rbegin(), reversed.rend(), DATA.begin()));
    EXPECT_TRUE(std::equal(reversed.begin(), reversed.end(), DATA.rbegin()));
}


TEST(reference_deque, capacity)
{
    using reference = reference_deque<int&>;

}


TEST(reference_deque, element)
{
    using reference = reference_deque<int&>;

}


TEST(reference_deque, modifiers)
{
    using reference = reference_deque<int&>;
    reference deque(5, DATA[0]);

    EXPECT_EQ(deque.size(), 5);
    EXPECT_GE(deque.max_size(), 5);
    EXPECT_FALSE(deque.empty());

    deque.shrink_to_fit();
    EXPECT_EQ(deque.size(), 5);
}


TEST(reference_deque, relational)
{
    using reference = reference_deque<int&>;

    // create deques
    reference deque;
    reference reversed;
    reference duplicate(5, DATA[0]);
    for (auto &item: DATA) {
        deque.push_back(item);
        reversed.push_front(item);
    }

    EXPECT_EQ(deque, deque);
    EXPECT_NE(deque, reversed);
    EXPECT_NE(deque, duplicate);
    EXPECT_NE(reversed, duplicate);
}
