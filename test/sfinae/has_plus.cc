//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator+()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_plus.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_plus, has_plus)
{
    using pair = std::pair<int, int>;

    static_assert(has_plus<int>::value, "");
    static_assert(has_plus<int, int>::value, "");
    static_assert(!has_plus<pair>::value, "");
    static_assert(!has_plus<pair, pair>::value, "");
    static_assert(!has_plus<pair, int>::value, "");
    static_assert(!has_plus<int, pair>::value, "");
}
