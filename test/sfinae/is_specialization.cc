//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `is_specialization` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_specialization.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_specialization, is_specialization)
{
    using pair = std::pair<int, int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(is_specialization<pair, std::pair>::value, "");
    static_assert(is_specialization<t1, std::tuple>::value, "");
    static_assert(is_specialization<t2, std::tuple>::value, "");
    static_assert(is_specialization<t3, std::tuple>::value, "");
    static_assert(!is_specialization<pair, std::tuple>::value, "");
    static_assert(!is_specialization<t1, std::pair>::value, "");
    static_assert(!is_specialization<t2, std::pair>::value, "");
    static_assert(!is_specialization<t3, std::pair>::value, "");
}
