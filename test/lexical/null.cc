//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical NULL conversion unittests.
 */

#include <pycpp/lexical/format.h>
#include <pycpp/lexical/null.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static vector<pair<nullptr_t, reference_wrapper<const string>>> DATA = {
    {nullptr, NULL_STRING},
};

// TESTS
// -----


TEST(lexical_null_formatter, lexical_null_formatter)
{
    auto TEST_NULL  = [](nullptr_t b, const string_view& expected) {
        EXPECT_EQ(lexical_null_formatter(b).str(), expected);
    };

    for (const auto& pair: DATA) {
        TEST_NULL(pair.first, pair.second.get());
    }
}


TEST(lexical_null_extractor, lexical_null_extractor)
{
    EXPECT_EQ(lexical_null_extractor(NULL_STRING).value(), nullptr);
    ASSERT_TRUE("nan" != NULL_STRING);
    EXPECT_THROW(lexical_null_extractor("nan"), runtime_error);
}
