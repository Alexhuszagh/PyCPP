//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Semi-intrusive deque unittests.
 */

#include <pycpp/intrusive/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::deque<int> DATA = {1, 2, 3, 4, 5};

// TESTS
// -----


TEST(intrusive_deque, constructor)
{
    using intrusive = intrusive_deque<int>;

    intrusive deque;
    EXPECT_EQ(deque.size(), 0);

// TODO: this is segfaulting
//    deque = intrusive(5, DATA[0]);
//    EXPECT_EQ(deque.size(), 5);

//    intrusive copy(deque);
//    EXPECT_EQ(copy.size(), 5);
//
//    copy = deque;
//    EXPECT_EQ(copy.size(), 5);

//    intrusive moved(move(deque));
//    EXPECT_EQ(moved.size(), 5);
//
//    moved = move(copy);
//    EXPECT_EQ(moved.size(), 5);
}


TEST(intrusive_deque, iterator)
{
    using intrusive = intrusive_deque<int>;

    // create deques
    intrusive deque;
    intrusive reversed;
    for (auto &item: DATA) {
        deque.push_back(item);
        reversed.push_front(item);
    }

    // check expected equality
    EXPECT_TRUE(equal(deque.begin(), deque.end(), DATA.begin()));
    EXPECT_TRUE(equal(deque.rbegin(), deque.rend(), DATA.rbegin()));
    EXPECT_TRUE(equal(reversed.rbegin(), reversed.rend(), DATA.begin()));
    EXPECT_TRUE(equal(reversed.begin(), reversed.end(), DATA.rbegin()));
}


TEST(intrusive_deque, capacity)
{
    using intrusive = intrusive_deque<int>;
    intrusive deque(5, DATA[0]);

    EXPECT_EQ(deque.size(), 5);
    EXPECT_GE(deque.max_size(), 5);
    EXPECT_FALSE(deque.empty());

    deque.shrink_to_fit();
    EXPECT_EQ(deque.size(), 5);
}


TEST(intrusive_deque, element)
{
    using intrusive = intrusive_deque<int>;

    intrusive deque;
    for (auto &item: DATA) {
        deque.push_back(item);
    }

    EXPECT_EQ(deque.at(0), 1);
    EXPECT_EQ(deque.at(1), 2);
    EXPECT_EQ(deque[0], 1);
    EXPECT_EQ(deque[1], 2);
    EXPECT_EQ(deque.front(), 1);
    EXPECT_EQ(deque.back(), 5);
}


TEST(intrusive_deque, modifiers)
{
    using intrusive = intrusive_deque<int>;
    std::deque<int> data(DATA);

    // push_back
    intrusive deque;
    intrusive empty;
    for (auto &item: data) {
        deque.push_back(item);
    }
    EXPECT_EQ(deque.size(), 5);
    EXPECT_EQ(deque.back(), 5);

    // pop_back
    deque.pop_back();
    EXPECT_EQ(deque.size(), 4);
    EXPECT_EQ(deque.back(), 4);

    // push_front
    deque.push_front(data[0]);
    EXPECT_EQ(deque.size(), 5);
    EXPECT_EQ(deque[0], 1);
    EXPECT_EQ(deque[1], 1);

    // pop_front
    deque.pop_front();
    EXPECT_EQ(deque.size(), 4);
    EXPECT_EQ(deque.front(), 1);

    // insert
    deque.insert(deque.cbegin(), data[0]);
    EXPECT_EQ(deque.size(), 5);
    EXPECT_EQ(deque[0], 1);
    EXPECT_EQ(deque[1], 1);

    // erase
    deque.erase(deque.cbegin());
    EXPECT_EQ(deque.size(), 4);
    EXPECT_EQ(deque[0], 1);
    EXPECT_EQ(deque[1], 2);

    // swap
    deque.swap(empty);
    EXPECT_EQ(deque.size(), 0);
    EXPECT_EQ(empty.size(), 4);

    // clear
    deque.clear();
    EXPECT_EQ(deque.size(), 0);
}


TEST(intrusive_deque, relational)
{
    using intrusive = intrusive_deque<int>;

    // create deques
    intrusive deque;
    intrusive reversed;
    intrusive duplicate(5, DATA[0]);
    for (auto &item: DATA) {
        deque.push_back(item);
        reversed.push_front(item);
    }

    // operator==
    EXPECT_EQ(deque, deque);
    EXPECT_EQ(reversed, reversed);
    EXPECT_EQ(duplicate, duplicate);

    // operator!=
    EXPECT_NE(deque, reversed);
    EXPECT_NE(deque, duplicate);
    EXPECT_NE(reversed, duplicate);

    // operator<
    EXPECT_LT(duplicate, deque);
    EXPECT_LT(deque, reversed);

    // operator<=
    EXPECT_LE(duplicate, duplicate);
    EXPECT_LE(duplicate, deque);
    EXPECT_LE(deque, deque);
    EXPECT_LE(deque, reversed);
    EXPECT_LE(reversed, reversed);

    // operator>
    EXPECT_GT(deque, duplicate);
    EXPECT_GT(reversed, deque);

    // operator>=
    EXPECT_GE(duplicate, duplicate);
    EXPECT_GE(deque, duplicate);
    EXPECT_GE(deque, deque);
    EXPECT_GE(reversed, deque);
    EXPECT_GE(reversed, reversed);
}
