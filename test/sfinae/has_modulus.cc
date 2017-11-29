//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator%()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_modulus.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_modulus, has_modulus)
{
    using pair = std::pair<int, int>;

    static_assert(has_modulus<int>::value, "");
    static_assert(has_modulus<int, int>::value, "");
    static_assert(!has_modulus<pair>::value, "");
    static_assert(!has_modulus<pair, pair>::value, "");
    static_assert(!has_modulus<pair, int>::value, "");
    static_assert(!has_modulus<int, pair>::value, "");
}
