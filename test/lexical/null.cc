//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical NULL conversion unittests.
 */

#include <pycpp/lexical/format.h>
#include <pycpp/lexical/null.h>
#include <gtest/gtest.h>
#include <functional>
#include <utility>
#include <vector>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::vector<std::pair<std::nullptr_t, std::reference_wrapper<const std::string>>> DATA = {
    {nullptr, NULL_STRING},
};

// TESTS
// -----


TEST(lexical_null_formatter, lexical_null_formatter)
{
    auto TEST_NULL  = [](std::nullptr_t b, const std::string &expected) {
        EXPECT_EQ(lexical_null_formatter(b).string(), expected);
    };

    for (const auto& pair: DATA) {
        TEST_NULL(pair.first, pair.second);
    }
}


TEST(lexical_null_extractor, lexical_null_extractor)
{
    EXPECT_EQ(lexical_null_extractor(NULL_STRING).value(), nullptr);
    ASSERT_TRUE("nan" != NULL_STRING);
    EXPECT_THROW(lexical_null_extractor("nan"), std::runtime_error);
}
