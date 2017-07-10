//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Safe stdlib unittests.
 */

#include <pycpp/safe/stdlib.h>
#include <gtest/gtest.h>


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
