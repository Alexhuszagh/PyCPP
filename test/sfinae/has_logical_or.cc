//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator||()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_logical_or.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_logical_or, has_logical_or)
{
    using pair = std::pair<int, int>;

    static_assert(has_logical_or<int>::value, "");
    static_assert(has_logical_or<int, int>::value, "");
    static_assert(!has_logical_or<pair>::value, "");
    static_assert(!has_logical_or<pair, pair>::value, "");
    static_assert(!has_logical_or<pair, int>::value, "");
    static_assert(!has_logical_or<int, pair>::value, "");
}
