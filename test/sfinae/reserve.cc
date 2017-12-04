//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `reserve` and `has_reserve` unittests.
 */

#include <pycpp/sfinae/reserve.h>
#include <gtest/gtest.h>
#include <deque>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(reserve, reserve)
{
    std::deque<int> deq;
    std::vector<int> vec;

    reserve()(deq, 10);
    reserve()(vec, 10);
}


TEST(reserve, has_reserve)
{
    using deq = std::deque<int>;
    using vec = std::vector<int>;

    static_assert(!has_reserve<deq>::value, "");
    static_assert(has_reserve<vec>::value, "");
}
