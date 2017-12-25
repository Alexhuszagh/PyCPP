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

static json_string_t EXPECTED = "{\n    \"k1\": \"v1\",\n    \"k2\": 5.0\n}";

template <typename Writer, typename ... Ts>
static Writer test_json_writer(bool move, Ts&&... ts)
{
    Writer w1;
    if (move) {
        Writer w2(forward<Ts>(ts)...);
        w1.swap(w2);
    } else {
        w1 = Writer(forward<Ts>(ts)...);
    }
    w1.start_object();
    w1.key("k1");
    w1.string("v1");
    w1.key("k2");
    w1.number(5);
    w1.end_object();
    w1.flush();

    return w1;
}


static void check_result(const json_string_t& str)
{
    EXPECT_EQ(replace(str, NEWLINE, POSIX_NEWLINE), EXPECTED);
}

// TESTS
// -----


TEST(json, json_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    {
        json_ostringstream_t sstream;
        test_json_writer<json_stream_writer>(false, sstream);
        check_result(sstream.str());
    }
    {
        json_ostringstream_t sstream;
        test_json_writer<json_stream_writer>(true, sstream);
        check_result(sstream.str());
    }
}


TEST(json, json_file_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    string path("test.json");
    auto checker = [&path]()
    {
        json_stringstream_t sstream;
        ifstream ifs(path);
        sstream << ifs.rdbuf();
        check_result(sstream.str());
        ifs.close();
        EXPECT_TRUE(remove_file(path));
    };

    {
        test_json_writer<json_file_writer>(false, path);
        checker();
    }
    {
        test_json_writer<json_file_writer>(true, path);
        checker();
    }
}


TEST(json, json_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    check_result(test_json_writer<json_string_writer>(false).str());
}
