//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator*()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_multiplies.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_multiplies, has_multiplies)
{
    using pair = std::pair<int, int>;

    static_assert(has_multiplies<int>::value, "");
    static_assert(has_multiplies<int, int>::value, "");
    static_assert(!has_multiplies<pair>::value, "");
    static_assert(!has_multiplies<pair, pair>::value, "");
    static_assert(!has_multiplies<pair, int>::value, "");
    static_assert(!has_multiplies<int, pair>::value, "");
}
