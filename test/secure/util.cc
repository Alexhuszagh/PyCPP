//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/secure/util.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(secure_util, secure_malloc)
{
    auto* ptr = secure_malloc(50);
    secure_free(ptr);
}


TEST(secure_util, secure_calloc)
{
    auto* ptr = secure_calloc(50, 1);
    secure_free(ptr);
}
