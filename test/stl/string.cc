//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL basic_string alias unittests.
 */

#include <pycpp/stl/string.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(basic_string, basic_string)
{
    using basic_string_type = basic_string<char>;
    basic_string_type str;
}
