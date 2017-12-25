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

static string EXPECTED = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<name>Alex</name>\n";

template <typename Writer, typename ... Ts>
static Writer test_xml_writer(bool move, Ts&&... ts)
{
    Writer w1;
    if (move) {
        Writer w2(std::forward<Ts>(ts)...);
        w1.swap(w2);
    } else {
        w1 = Writer(std::forward<Ts>(ts)...);
    }
    w1.start_element("name");
    w1.write_text("Alex");
    w1.end_element();
    w1.flush();

    return w1;
}


static void check_result(const string& str)
{
    EXPECT_EQ(replace(str, NEWLINE, POSIX_NEWLINE), EXPECTED);
}


// TESTS
// -----


TEST(xml, xml_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    {
        ostringstream sstream;
        test_xml_writer<xml_stream_writer>(false, sstream);
        check_result(sstream.str());
    }
    {
        ostringstream sstream;
        test_xml_writer<xml_stream_writer>(true, sstream);
        check_result(sstream.str());
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
        check_result(sstream.str());
        EXPECT_TRUE(remove_file(path));
    };

    {
        test_xml_writer<xml_file_writer>(false, path);
        checker();
    }
    {
        test_xml_writer<xml_file_writer>(true, path);
        checker();
    }
}


TEST(xml, xml_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    check_result(test_xml_writer<xml_string_writer>(false).str());
    check_result(test_xml_writer<xml_string_writer>(true).str());
}
