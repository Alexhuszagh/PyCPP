//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member type detection unittests.
 */

#include <pycpp/sfinae/has_member_type.h>
#include <pycpp/stl/map.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_TYPE(mapped_type, has_mapped_type);

// TESTS
// -----

TEST(has_member_type, has_mapped_type)
{
    using map_type = map<int, int>;
    using vector_type = vector<int>;

    static_assert(has_mapped_type<map_type>::value, "");
    static_assert(!has_mapped_type<vector_type>::value, "");
}
