//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator>>()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_right_shift.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_right_shift, has_right_shift)
{
    using pair = std::pair<int, int>;

    static_assert(has_right_shift<int>::value, "");
    static_assert(has_right_shift<int, int>::value, "");
    static_assert(!has_right_shift<pair>::value, "");
    static_assert(!has_right_shift<pair, pair>::value, "");
    static_assert(!has_right_shift<pair, int>::value, "");
    static_assert(!has_right_shift<int, pair>::value, "");
}
