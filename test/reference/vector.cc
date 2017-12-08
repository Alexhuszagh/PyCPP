//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Reference vector unittests.
 */

#include <pycpp/reference/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::vector<int> DATA = {1, 2, 3, 4, 5};

// TESTS
// -----


TEST(reference_vector, constructor)
{
    using reference = reference_vector<int>;

    reference vector;
    EXPECT_EQ(vector.size(), 0);

    vector = reference(5, DATA[0]);
    EXPECT_EQ(vector.size(), 5);

    reference copy(vector);
    EXPECT_EQ(copy.size(), 5);

    copy = vector;
    EXPECT_EQ(copy.size(), 5);

    reference moved(std::move(vector));
    EXPECT_EQ(moved.size(), 5);

    moved = std::move(copy);
    EXPECT_EQ(moved.size(), 5);
}


TEST(reference_vector, iterator)
{
    using reference = reference_vector<int>;

    // create vectors
    reference vector;
    reference reversed;
    for (auto &item: DATA) {
        vector.push_back(item);
    }
    for (auto it =  DATA.rbegin(); it != DATA.rend(); ++it) {
        reversed.push_back(*it);
    }

    // check expected equality
    EXPECT_TRUE(std::equal(vector.begin(), vector.end(), DATA.begin()));
    EXPECT_TRUE(std::equal(vector.rbegin(), vector.rend(), DATA.rbegin()));
    EXPECT_TRUE(std::equal(reversed.rbegin(), reversed.rend(), DATA.begin()));
    EXPECT_TRUE(std::equal(reversed.begin(), reversed.end(), DATA.rbegin()));
}


TEST(reference_vector, capacity)
{
    using reference = reference_vector<int>;
    reference vector(5, DATA[0]);

    EXPECT_EQ(vector.size(), 5);
    EXPECT_GE(vector.capacity(), 5);
    EXPECT_GE(vector.max_size(), 5);
    EXPECT_FALSE(vector.empty());

    vector.shrink_to_fit();
    EXPECT_EQ(vector.size(), 5);
    EXPECT_GE(vector.capacity(), 5);
}


TEST(reference_vector, element)
{
    using reference = reference_vector<int>;

    reference vector;
    for (auto &item: DATA) {
        vector.push_back(item);
    }

    EXPECT_EQ(vector.at(0), 1);
    EXPECT_EQ(vector.at(1), 2);
    EXPECT_EQ(vector[0], 1);
    EXPECT_EQ(vector[1], 2);
    EXPECT_EQ(vector.front(), 1);
    EXPECT_EQ(vector.back(), 5);
}


TEST(reference_vector, modifiers)
{
    using reference = reference_vector<int>;
    std::vector<int> data(DATA);

    // push_back
    reference vector;
    reference empty;
    for (auto &item: data) {
        vector.push_back(item);
    }
    EXPECT_EQ(vector.size(), 5);
    EXPECT_EQ(vector.back(), 5);

    // pop_back
    vector.pop_back();
    EXPECT_EQ(vector.size(), 4);
    EXPECT_EQ(vector.back(), 4);

    // insert
    vector.insert(vector.cbegin(), data[0]);
    EXPECT_EQ(vector.size(), 5);
    EXPECT_EQ(vector[0], 1);
    EXPECT_EQ(vector[1], 1);

    // erase
    vector.erase(vector.cbegin());
    EXPECT_EQ(vector.size(), 4);
    EXPECT_EQ(vector[0], 1);
    EXPECT_EQ(vector[1], 2);

    // swap
    vector.swap(empty);
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(empty.size(), 4);

    // clear
    vector.clear();
    EXPECT_EQ(vector.size(), 0);
}


TEST(reference_vector, relational)
{
    using reference = reference_vector<int>;

    // create vectors
    reference vector;
    reference reversed;
    reference duplicate(5, DATA[0]);
    for (auto &item: DATA) {
        vector.push_back(item);
    }
    for (auto it =  DATA.rbegin(); it != DATA.rend(); ++it) {
        reversed.push_back(*it);
    }

    // operator==
    EXPECT_EQ(vector, vector);
    EXPECT_EQ(reversed, reversed);
    EXPECT_EQ(duplicate, duplicate);

    // operator!=
    EXPECT_NE(vector, reversed);
    EXPECT_NE(vector, duplicate);
    EXPECT_NE(reversed, duplicate);

    // operator<
    EXPECT_LT(duplicate, vector);
    EXPECT_LT(vector, reversed);

    // operator<=
    EXPECT_LE(duplicate, duplicate);
    EXPECT_LE(duplicate, vector);
    EXPECT_LE(vector, vector);
    EXPECT_LE(vector, reversed);
    EXPECT_LE(reversed, reversed);

    // operator>
    EXPECT_GT(vector, duplicate);
    EXPECT_GT(reversed, vector);

    // operator>=
    EXPECT_GE(duplicate, duplicate);
    EXPECT_GE(vector, duplicate);
    EXPECT_GE(vector, vector);
    EXPECT_GE(reversed, vector);
    EXPECT_GE(reversed, reversed);
}
