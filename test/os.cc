//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Operating system detection unittests.
 */

#include <pycpp/config.h>
#include <pycpp/os.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(os, os_detected)
{
#ifndef OS_DETECTED
#   error Operating system not detected.
#endif
}
