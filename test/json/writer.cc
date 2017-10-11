//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief JSON writer unittests.
 */

#include <pycpp/json.h>
#include <gtest/gtest.h>


// TESTS
// -----


TEST(json, json_stream_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
    std::stringstream sstream;
    json_stream_writer reader(sstream);
    EXPECT_EQ(sstream.str(), "");
}


TEST(json, json_file_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
//    std::string str(" { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ");
//    std::string path("test.json");
//    {
//        ofstream ostream(path);
//        ostream << str << std::endl;
//    }
//    json_file_writer reader;
//    test_json_writer(reader, path);
//    EXPECT_TRUE(remove_file(path));
}


TEST(json, json_string_writer)
{
    // don't worry about compliance testing:
    // the backends are robustly tested
//    std::string str(" { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ");
//    json_string_writer reader;
//    test_json_writer(reader, str);
}
