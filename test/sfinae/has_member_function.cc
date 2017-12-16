//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member function detection unittests.
 */

#include <pycpp/sfinae/has_member_function.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/unordered_set.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_FUNCTION(reserve, has_reserve);
PYCPP_HAS_MEMBER_FUNCTION3(push_back, has_push_back, void (C::*)(typename C::const_reference));

// TESTS
// -----

TEST(has_member_function, has_reserve)
{
    using deq = deque<int>;
    using vec = vector<int>;

    static_assert(!has_reserve<deq>::value, "");
    static_assert(has_reserve<vec>::value, "");
}


TEST(has_member_function, has_push_back)
{
    using set = unordered_set<int>;
    using vec = vector<int>;

    static_assert(!has_push_back<set>::value, "");
    static_assert(has_push_back<vec>::value, "");
}
