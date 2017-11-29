//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator|()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_bit_or.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_bit_or, has_bit_or)
{
    using pair = std::pair<int, int>;

    static_assert(has_bit_or<int>::value, "");
    static_assert(has_bit_or<int, int>::value, "");
    static_assert(!has_bit_or<pair>::value, "");
    static_assert(!has_bit_or<pair, pair>::value, "");
    static_assert(!has_bit_or<pair, int>::value, "");
    static_assert(!has_bit_or<int, pair>::value, "");
}
