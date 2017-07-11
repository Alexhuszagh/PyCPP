//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/parameter.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(parameters_t, get)
{
    parameters_t parameters = {
        {"name", "value"},
    };
    EXPECT_EQ(parameters.get(), "?name=value");
}


TEST(parameters_t, add)
{
    parameters_t parameters = {
        {"name", "value"},
    };
    parameters.add(parameter_t("name2", "value2"));
    EXPECT_EQ(parameters.get(), "?name=value&name2=value2");
}
