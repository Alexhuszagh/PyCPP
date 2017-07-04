//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON DOM unittests.
 */

#include <json.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(json, dom)
{
    json_document_t document;
    document.loads(" { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ");

    ASSERT_TRUE(document.has_object());
    auto &object = document.get_object();
    EXPECT_EQ(object.size(), 7);
    EXPECT_EQ(object["hello"].get_string(), "world");
    EXPECT_EQ(object["t"].get_boolean(), true);
    EXPECT_EQ(object["pi"].get_number(), 3.1416);
    EXPECT_EQ(object["a"].get_array().size(), 4);
    EXPECT_EQ(object["a"].get_array().front().get_number(), 1.);

    auto str = document.dumps(' ', 0);
    EXPECT_EQ(str, "{\"a\":[1.0,2.0,3.0,4.0],\"i\":123.0,\"pi\":3.1416,\"hello\":\"world\",\"t\":true,\"f\":false,\"n\":null}");

    str = document.dumps(' ', 4);
    EXPECT_EQ(str, "{\n    \"a\": [\n        1.0,\n        2.0,\n        3.0,\n        4.0\n    ],\n    \"i\": 123.0,\n    \"pi\": 3.1416,\n    \"hello\": \"world\",\n    \"t\": true,\n    \"f\": false,\n    \"n\": null\n}");
}
