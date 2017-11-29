//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member type detection unittests.
 */

#include <pycpp/sfinae/has_member_type.h>
#include <gtest/gtest.h>
#include <map>
#include <vector>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_TYPE(mapped_type, has_mapped_type);

// TESTS
// -----

TEST(has_member_type, has_mapped_type)
{
    using map = std::map<int, int>;
    using vec = std::vector<int>;

    static_assert(has_mapped_type<map>::value, "");
    static_assert(!has_mapped_type<vec>::value, "");
}
