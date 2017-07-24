//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Operating system detection unittests.
 */

#include <pycpp/preprocessor/os.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(os, os_detected)
{
#ifndef OS_DETECTED
#   error Operating system not detected.
#endif
}
