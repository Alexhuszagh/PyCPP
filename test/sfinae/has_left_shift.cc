//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator<<()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_left_shift.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_left_shift, has_left_shift)
{
    using pair = std::pair<int, int>;

    static_assert(has_left_shift<int>::value, "");
    static_assert(has_left_shift<int, int>::value, "");
    static_assert(!has_left_shift<pair>::value, "");
    static_assert(!has_left_shift<pair, pair>::value, "");
    static_assert(!has_left_shift<pair, int>::value, "");
    static_assert(!has_left_shift<int, pair>::value, "");
}
