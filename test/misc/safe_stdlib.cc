//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Safe stdlib unittests.
 */

#include <pycpp/misc/safe_stdlib.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(stdlib, safe_alloc)
{
    auto *ptr = safe_malloc(100);
    ptr = safe_realloc(ptr, 200);
    safe_free(ptr);

    ptr = safe_calloc(100, 1);
    safe_free(ptr);
}
