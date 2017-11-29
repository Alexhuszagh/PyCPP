//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator/()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_divides.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_divides, has_divides)
{
    using pair = std::pair<int, int>;

    static_assert(has_divides<int>::value, "");
    static_assert(has_divides<int, int>::value, "");
    static_assert(!has_divides<pair>::value, "");
    static_assert(!has_divides<pair, pair>::value, "");
    static_assert(!has_divides<pair, int>::value, "");
    static_assert(!has_divides<int, pair>::value, "");
}
