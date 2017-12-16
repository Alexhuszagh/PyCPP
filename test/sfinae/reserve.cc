//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `reserve` and `has_reserve` unittests.
 */

#include <pycpp/sfinae/reserve.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(reserve, reserve)
{
    deque<int> deq;
    vector<int> vec;

    reserve()(deq, 10);
    reserve()(vec, 10);
}


TEST(reserve, has_reserve)
{
    using deque_type = deque<int>;
    using vector_type = vector<int>;

    static_assert(!has_reserve<deque_type>::value, "");
    static_assert(has_reserve<vector_type>::value, "");
}
