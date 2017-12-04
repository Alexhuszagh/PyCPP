//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `shrink_to_fit` and `has_shrink_to_fit` unittests.
 */

#include <pycpp/sfinae/shrink_to_fit.h>
#include <gtest/gtest.h>
#include <list>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(shrink_to_fit, shrink_to_fit)
{
    std::list<int> lst;
    std::vector<int> vec;

    shrink_to_fit()(lst);
    shrink_to_fit()(vec);
}


TEST(shrink_to_fit, has_shrink_to_fit)
{
    using lst = std::list<int>;
    using vec = std::vector<int>;

    static_assert(!has_shrink_to_fit<lst>::value, "");
    static_assert(has_shrink_to_fit<vec>::value, "");
}
