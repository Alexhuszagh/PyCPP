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
    using reference = reference_vector<int&>;

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

}


TEST(reference_vector, capacity)
{

}


TEST(reference_vector, element)
{

}


TEST(reference_vector, modifiers)
{

}


TEST(reference_vector, relational)
{

}
