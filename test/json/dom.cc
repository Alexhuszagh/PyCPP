//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON DOM unittests.
 */

#include <pycpp/json.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(json, dom)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    json_document_t d1;
    d1.loads(" { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ");

    ASSERT_TRUE(d1.has_object());
    auto& o1 = d1.get_object();
    EXPECT_EQ(o1.size(), 7);
    EXPECT_EQ(o1["hello"].get_string(), "world");
    EXPECT_EQ(o1["t"].get_boolean(), true);
    EXPECT_EQ(o1["pi"].get_number(), 3.1416);
    EXPECT_EQ(o1["a"].get_array().size(), 4);
    EXPECT_EQ(o1["a"].get_array().front().get_number(), 1.);

    auto str = d1.dumps(' ', 0);
    // only check the first character, since the order isn't defined
    EXPECT_EQ(str.substr(0, 1), "{");

    str = d1.dumps(' ', 4);
    // only check the first two to ensure newlines are added
    EXPECT_EQ(str.substr(0, 2), "{\n");

    // move constructor
    json_document_t d2(std::move(d1));
    ASSERT_TRUE(d2.has_object());
    auto& o2 = d2.get_object();
    EXPECT_EQ(o2.size(), 7);

    // move assign
    d1 = std::move(d2);
    ASSERT_TRUE(d1.has_object());
    auto& o3 = d1.get_object();
    EXPECT_EQ(o3.size(), 7);
}
