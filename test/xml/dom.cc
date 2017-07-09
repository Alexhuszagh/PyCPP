//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON DOM unittests.
 */

#include <xml.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(xml, dom)
{
    xml_document_t document;
    document.loads("<?xml version=\"1.0\" encoding=\"UTF-8\"?><note><to>Tove</to><from>Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>");

//    ASSERT_TRUE(document.has_object());
//    auto &object = document.get_object();
//    EXPECT_EQ(object.size(), 7);
//    EXPECT_EQ(object["hello"].get_string(), "world");
//    EXPECT_EQ(object["t"].get_boolean(), true);
//    EXPECT_EQ(object["pi"].get_number(), 3.1416);
//    EXPECT_EQ(object["a"].get_array().size(), 4);
//    EXPECT_EQ(object["a"].get_array().front().get_number(), 1.);
//
//    auto str = document.dumps(' ', 0);
//    // only check the first character, since the order isn't defined
//    EXPECT_EQ(str.substr(0, 1), "{");
//
//    str = document.dumps(' ', 4);
//    // only check the first two to ensure newlines are added
//    EXPECT_EQ(str.substr(0, 2), "{\n");
}
