//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief XML writer unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/string/string.h>
#include <pycpp/xml.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------


template <typename Writer>
static void test_xml_writer(Writer& writer)
{
    writer.start_element("name");
    writer.write_text("Alex");
    writer.end_element();
    writer.flush();
}


// TESTS
// -----


TEST(xml, xml_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    ostringstream sstream;
    xml_stream_writer writer(sstream);
    test_xml_writer(writer);
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n");
}


TEST(xml, xml_file_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    string path("test.xml");
    {
        xml_file_writer writer(path);
        test_xml_writer(writer);
    }
    stringstream sstream;
    {
        ifstream istream(path);
        sstream << istream.rdbuf();
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n");
    EXPECT_TRUE(remove_file(path));
}


TEST(xml, xml_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    xml_string_writer writer;
    test_xml_writer(writer);
    // force POSIX-like newlines
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n");
}
