//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL map alias unittests.
 */

#include <pycpp/stl/map.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(map, map)
{
    using map_type = map<int, int>;
    map_type mp;
}


TEST(multimap, multimap)
{
    using multimap_type = multimap<int, int>;
    multimap_type mmp;
}
