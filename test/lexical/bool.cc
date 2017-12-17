//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical bool conversion unittests.
 */

#include <pycpp/lexical/bool.h>
#include <pycpp/lexical/format.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static vector<pair<bool, reference_wrapper<const std::string>>> DATA = {
    {true, TRUE_STRING},
    {false, FALSE_STRING},
};

// TESTS
// -----


TEST(lexical_bool_formatter, lexical_bool_formatter)
{
    auto TEST_BOOL  = [](bool b, const string_view& expected) {
        EXPECT_EQ(lexical_bool_formatter(b).string(), expected);
    };
    auto TEST_CONST_BOOL  = [](const bool b, const string_view& expected) {
        EXPECT_EQ(lexical_bool_formatter(b).string(), expected);
    };
    auto TEST_VOLATILE_BOOL  = [](volatile bool b, const string_view& expected) {
        EXPECT_EQ(lexical_bool_formatter(b).string(), expected);
    };
    auto TEST_CV_BOOL  = [](volatile bool b, const string_view& expected) {
        EXPECT_EQ(lexical_bool_formatter(b).string(), expected);
    };

    for (const auto& pair: DATA) {
        TEST_BOOL(pair.first, pair.second.get());
        TEST_CONST_BOOL(pair.first, pair.second.get());
        TEST_VOLATILE_BOOL(pair.first, pair.second.get());
        TEST_CV_BOOL(pair.first, pair.second.get());
    }
}


TEST(lexical_bool_extractor, lexical_bool_extractor)
{
    EXPECT_EQ(bool(lexical_bool_extractor(TRUE_STRING)), true);
    EXPECT_EQ(bool(lexical_bool_extractor(FALSE_STRING)), false);
    EXPECT_THROW(lexical_bool_extractor(""), runtime_error);
    ASSERT_TRUE("nan" != TRUE_STRING && "nan" != FALSE_STRING);
    EXPECT_THROW(lexical_bool_extractor("nan"), runtime_error);
}
