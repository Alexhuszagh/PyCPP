//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL unordered_map alias unittests.
 */

#include <pycpp/stl/unordered_map.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(unordered_map, unordered_map)
{
    using unordered_map_type = unordered_map<int, int>;
    unordered_map_type ump;
}


TEST(unordered_multimap, unordered_multimap)
{
    using unordered_multimap_type = unordered_multimap<int, int>;
    unordered_multimap_type ummp;
}
