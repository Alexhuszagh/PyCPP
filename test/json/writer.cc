//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON writer unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/json.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/string/string.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------


template <typename Writer>
static void test_json_writer(Writer& writer)
{
    writer.start_object();
    writer.key("k1");
    writer.string("v1");
    writer.key("k2");
    writer.number(5);
    writer.end_object();
    writer.flush();
}

// TESTS
// -----


TEST(json, json_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    ostringstream sstream;
    json_stream_writer writer(sstream);
    test_json_writer(writer);
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), "{\n    \"k1\": \"v1\",\n    \"k2\": 5.0\n}");
}


TEST(json, json_file_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    string path("test.xml");
    std::string str;
    {
        json_file_writer writer(path);
        test_json_writer(writer);
    }
    ostringstream sstream;
    {
        ifstream istream(path);
        sstream << istream.rdbuf();
        // force POSIX-like newlines
    }
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), "{\n    \"k1\": \"v1\",\n    \"k2\": 5.0\n}");
    EXPECT_TRUE(remove_file(path));
}


TEST(json, json_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    json_string_writer writer;
    test_json_writer(writer);
    // force POSIX-like newlines
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), "{\n    \"k1\": \"v1\",\n    \"k2\": 5.0\n}");
}
