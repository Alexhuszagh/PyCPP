//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON DOM unittests.
 */

#include <pycpp/xml.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(xml, dom)
{
    xml_document_t d1;
    d1.loads("<?xml version=\"1.0\" encoding=\"UTF-8\"?><note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>");

    // check document root
    ASSERT_EQ(d1.get_children().size(), 1);

    // check note
    auto &note = d1.get_children().front();
    EXPECT_EQ(note.get_tag(), "note");
    ASSERT_EQ(note.get_attrs().size(), 0);
    ASSERT_EQ(note.get_children().size(), 4);
    auto it = note.begin();

    // check to
    auto &to = *it++;
    EXPECT_EQ(to.get_tag(), "to");
    ASSERT_EQ(to.get_attrs().size(), 1);
    ASSERT_EQ(to.get_attrs().at("email"), "tove@tove.com");
    EXPECT_EQ(to.get_text(), "Tove");

    // check from
    auto &from = *it++;
    EXPECT_EQ(from.get_tag(), "from");
    ASSERT_EQ(from.get_attrs().size(), 1);
    ASSERT_EQ(from.get_attrs().at("email"), "jani@jani.com");
    EXPECT_EQ(from.get_text(), "Jani");

    // check heading
    auto &heading = *it++;
    EXPECT_EQ(heading.get_tag(), "heading");
    ASSERT_EQ(heading.get_attrs().size(), 0);
    EXPECT_EQ(heading.get_text(), "Reminder");

    // check body
    auto &body = *it++;
    EXPECT_EQ(body.get_tag(), "body");
    ASSERT_EQ(body.get_attrs().size(), 0);
    EXPECT_EQ(body.get_text(), "Don't forget me this weekend!");

    auto str = d1.dumps(' ', 0);
    EXPECT_EQ(str, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>\n");

    str = d1.dumps(' ', 4);
    EXPECT_EQ(str, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<note>    <to email=\"tove@tove.com\">Tove</to>\n    <from email=\"jani@jani.com\">Jani</from>\n    <heading>Reminder</heading>\n    <body>Don't forget me this weekend!</body>\n</note>\n");

    // tostring
    EXPECT_EQ(to.tostring(), "<to email=\"tove@tove.com\">Tove</to>\n");
    EXPECT_EQ(from.tostring(), "<from email=\"jani@jani.com\">Jani</from>\n");
    EXPECT_EQ(body.tostring(), "<body>Don't forget me this weekend!</body>\n");

    // fromstring
    auto from_copy = xml_node_t::fromstring(from.tostring());
    EXPECT_EQ(from.get_tag(), from_copy.get_tag());

    // move constructor
    xml_document_t d2(std::move(d1));
    ASSERT_EQ(d2.get_children().size(), 1);
    str = d2.dumps(' ', 0);
    EXPECT_EQ(str, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>\n");

    // move assign
    d1 = std::move(d2);
    ASSERT_EQ(d1.get_children().size(), 1);
    str = d1.dumps(' ', 0);
    EXPECT_EQ(str, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>\n");

}
