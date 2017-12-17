//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member variable detection unittests.
 */

#include <pycpp/sfinae/has_member_variable.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_VARIABLE(npos, has_npos);

// TESTS
// -----


TEST(has_member_variable, has_npos)
{
    using string_type = string;
    using vector_type = vector<int>;

    static_assert(has_npos<string_type>::value, "");
    static_assert(!has_npos<vector_type>::value, "");
}
