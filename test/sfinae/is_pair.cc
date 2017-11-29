//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `std::pair` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_pair.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_pair, is_pair)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(is_pair<pair>::value, "");
    static_assert(!is_pair<t1>::value, "");
    static_assert(!is_pair<t2>::value, "");
    static_assert(!is_pair<t3>::value, "");
    static_assert(!is_pair<int>::value, "");
    static_assert(!is_pair<char>::value, "");
}
