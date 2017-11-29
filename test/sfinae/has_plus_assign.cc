//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator+=()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_plus_assign.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_plus_assign, has_plus_assign)
{
    using pair = std::pair<int, int>;

    static_assert(has_plus_assign<int>::value, "");
    static_assert(has_plus_assign<int, int>::value, "");
    static_assert(!has_plus_assign<pair>::value, "");
    static_assert(!has_plus_assign<pair, pair>::value, "");
    static_assert(!has_plus_assign<pair, int>::value, "");
    static_assert(!has_plus_assign<int, pair>::value, "");
}
