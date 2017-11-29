//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member function detection unittests.
 */

#include <pycpp/sfinae/has_member_function.h>
#include <gtest/gtest.h>
#include <deque>
#include <vector>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_FUNCTION(reserve, has_reserve);

// TESTS
// -----

TEST(has_member_function, has_reserve)
{
    using deq = std::deque<int>;
    using vec = std::vector<int>;

    static_assert(!has_reserve<deq>::value, "");
    static_assert(has_reserve<vec>::value, "");
}
