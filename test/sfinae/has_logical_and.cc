//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator&&()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_logical_and.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_logical_and, has_logical_and)
{
    using pair = std::pair<int, int>;

    static_assert(has_logical_and<int>::value, "");
    static_assert(has_logical_and<int, int>::value, "");
    static_assert(!has_logical_and<pair>::value, "");
    static_assert(!has_logical_and<pair, pair>::value, "");
    static_assert(!has_logical_and<pair, int>::value, "");
    static_assert(!has_logical_and<int, pair>::value, "");
}
