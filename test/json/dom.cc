//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON DOM unittests.
 */

#include <json/dom.h>
#include <gtest/gtest.h>
#include <sstream>

// TESTS
// -----


TEST(json, dom)
{
    json_document_t document;
    std::istringstream stream(" { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ");
    document.parse(stream);

    ASSERT_TRUE(document.has_object());
    auto &object = document.get_object();
    EXPECT_EQ(object.size(), 7);
    EXPECT_EQ(object["hello"]->get_string(), "world");
    // TODO: add more tests...
}
