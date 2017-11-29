//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Member variable detection unittests.
 */

#include <pycpp/sfinae/has_member_variable.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

PYCPP_USING_NAMESPACE

// DATA
// ----

PYCPP_HAS_MEMBER_VARIABLE(npos, has_npos);

// TESTS
// -----


TEST(has_member_variable, has_npos)
{
    using str = std::string;
    using vec = std::vector<int>;

    static_assert(has_npos<str>::value, "");
    static_assert(!has_npos<vec>::value, "");
}
