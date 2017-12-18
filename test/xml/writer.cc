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

static std::string EXPECTED = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n";

template <typename Writer, typename OpenArg>
static void test_xml_writer(OpenArg& arg, bool move = false)
{
    Writer w1;
    if (move) {
        Writer w2(arg);
        w1.swap(w2);
    } else {
        w1 = Writer(arg);
    }
    w1.start_element("name");
    w1.write_text("Alex");
    w1.end_element();
    w1.flush();
}


// TESTS
// -----


TEST(xml, xml_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    {
        ostringstream sstream;
        test_xml_writer<xml_stream_writer>(sstream);
        EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), EXPECTED);
    }
    {
        ostringstream sstream;
        test_xml_writer<xml_stream_writer>(sstream, true);
        EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), EXPECTED);
    }
}


TEST(xml, xml_file_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    string path("test.xml");
    auto checker = [&path]()
    {
        stringstream sstream;
        ifstream ifs(path);
        sstream << ifs.rdbuf();
        EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), EXPECTED);
        EXPECT_TRUE(remove_file(path));
    };

    {
        test_xml_writer<xml_file_writer>(path);
        checker();
    }
    {
        test_xml_writer<xml_file_writer>(path, true);
        checker();
    }
    exit(0);
}


#if 0
TEST(xml, xml_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    xml_string_writer writer;
    test_xml_writer(writer);
    // force POSIX-like newlines
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n");
}
#endif
