//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Compiler detection unittests.
 */

#include <pycpp/compiler.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(compiler, compiler)
{
#ifndef COMPILER_DETECTED
#   error Compiler not detected.
#endif

#ifndef COMPILER_MAJOR_VERSION
#   error Compiler major version not detected.
#endif

#ifndef COMPILER_MINOR_VERSION
#   error Compiler minor version not detected.
#endif

#ifndef COMPILER_PATCH_VERSION
#   error Compiler patch version not detected.
#endif
}
