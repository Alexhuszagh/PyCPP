//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator-()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_minus.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_minus, has_minus)
{
    using pair = std::pair<int, int>;

    static_assert(has_minus<int>::value, "");
    static_assert(has_minus<int, int>::value, "");
    static_assert(!has_minus<pair>::value, "");
    static_assert(!has_minus<pair, pair>::value, "");
    static_assert(!has_minus<pair, int>::value, "");
    static_assert(!has_minus<int, pair>::value, "");
}
