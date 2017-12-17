//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON SAX unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/xml.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stream/fstream.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE


// -------


template <typename SaxReader, typename OpenArg>
static void test_xml_reader(SaxReader& reader, OpenArg& arg)
{
    xml_document_t document;
    xml_dom_handler handler(document);
    reader.set_handler(handler);
    reader.open(arg);

    // check document root
    ASSERT_EQ(document.get_children().size(), 1);

    // check note
    auto &note = document.get_children().front();
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
}

// TESTS
// -----


TEST(xml, xml_stream_reader)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    std::string str("<?xml version=\"1.0\" encoding=\"UTF-8\"?><note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>");
    istringstream sstream(str);
    xml_stream_reader reader;
    test_xml_reader(reader, sstream);
}


TEST(xml, xml_file_reader)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    std::string str("<?xml version=\"1.0\" encoding=\"UTF-8\"?><note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>");
    std::string path("test.xml");
    {
        ofstream ostream(path);
        ostream << str << endl;
    }
    {
        xml_file_reader reader;
        test_xml_reader(reader, path);
    }
    EXPECT_TRUE(remove_file(path));
}


TEST(xml, xml_string_reader)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    std::string str("<?xml version=\"1.0\" encoding=\"UTF-8\"?><note><to email=\"tove@tove.com\">Tove</to><from email=\"jani@jani.com\">Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body></note>");
    xml_string_reader reader;
    test_xml_reader(reader, str);
}
