//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL set alias unittests.
 */

#include <pycpp/stl/set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(set, set)
{
    using set_type = set<int, int>;
    set_type st;
}


TEST(multiset, multiset)
{
    using multiset_type = multiset<int, int>;
    multiset_type mst;
}
