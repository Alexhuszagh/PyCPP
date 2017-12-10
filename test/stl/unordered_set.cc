//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL unordered_set alias unittests.
 */

#include <pycpp/stl/unordered_set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(unordered_set, unordered_set)
{
    using unordered_set_type = unordered_set<int>;
    unordered_set_type uset;
}


TEST(unordered_multiset, unordered_multiset)
{
    using unordered_multiset_type = unordered_multiset<int>;
    unordered_multiset_type umset;
}
