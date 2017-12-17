//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL vector alias unittests.
 */

#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(vector, vector)
{
    using vector_type = vector<char>;
    vector_type vec;
}


TEST(vector, hash)
{
    using vector_type = vector<bool>;
    using hash_type = hash<vector_type>;
    using result_type = decltype(hash_type()(declval<vector_type>()));
    static_assert(is_same<result_type, size_t>::value, "");
}
