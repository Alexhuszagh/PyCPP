//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief CSV unittests.
 */

#include <pycpp/csv.h>
#include <pycpp/filesystem.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/string/string.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// CONSTANTS
// ---------

static const std::string CSV_SIMPLE_ALL = {34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_SKIP2_ALL = {10, 10, 34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_TAB_ALL = {34, 84, 104, 105, 115, 34, 9, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 9, 34, 77, -61, -86, 109, 101, 115, 34, 9, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 9, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 9, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 9, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_SIMPLE_MINIMAL = {84, 104, 105, 115, 44, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 44, 77, -61, -86, 109, 101, 115, 44, -20, -71, -100, -22, -75, -84, 10, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 44, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, -16, -97, -101, -126, 10};
static const csv_row CSV_HEADER = {
    {84, 104, 105, 115},
    {-28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85},
    {77, -61, -86, 109, 101, 115},
    {-20, -71, -100, -22, -75, -84}
};
static const csv_row CSV_ROW = {
    {-32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78},
    {32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118},
    {-29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44},
    {-16, -97, -101, -126}
};
static const csv_map CSV_MAP = {
    {{84, 104, 105, 115}, {-32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78}},
    {{-28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85}, {32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118}},
    {{77, -61, -86, 109, 101, 115}, {-29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44}},
    {{-20, -71, -100, -22, -75, -84}, {-16, -97, -101, -126}},
};

// TESTS
// -----

// PUNCTUATION

TEST(csvpunct, delimiter)
{
    csvpunct punct;
    EXPECT_EQ(punct.delimiter(), ',');
}


TEST(csvpunct, quote)
{
    csvpunct punct;
    EXPECT_EQ(punct.quote(), '"');
}


TEST(csvpunct, escape)
{
    csvpunct punct;
    EXPECT_EQ(punct.escape(), '\\');
}

// SIMPLE READER


TEST(csv_stream_reader, simple_all)
{
    istringstream sstream(CSV_SIMPLE_ALL);
    csv_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_HEADER);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_ROW);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, simple_minimal)
{
    istringstream sstream(CSV_SIMPLE_MINIMAL);
    csv_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_HEADER);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_ROW);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, skip)
{
    istringstream sstream(CSV_SKIP2_ALL);
    csv_stream_reader reader(sstream, 2);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_HEADER);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_ROW);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, iterator)
{
    istringstream sstream(CSV_SIMPLE_ALL);
    csv_stream_reader reader(sstream);
    auto first = reader.begin();
    auto last = reader.end();
    vector<csv_row> rows(first, last);
    EXPECT_EQ(rows.size(), 2);
    EXPECT_EQ(rows[0], CSV_HEADER);
    EXPECT_EQ(rows[1], CSV_ROW);
}


TEST(csv_stream_reader, punctuation)
{
    istringstream sstream(CSV_TAB_ALL);
    csv_stream_reader reader(sstream, 0, new tabpunct);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_HEADER);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_ROW);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, move)
{
    istringstream sstream(CSV_SIMPLE_MINIMAL);
    csv_stream_reader r1(sstream), r2;
    r1.swap(r2);
    EXPECT_FALSE(bool(r1));
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_HEADER);
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_ROW);
    EXPECT_FALSE(bool(r2));
}


#if defined(BUILD_FILESYSTEM)

TEST(csv_file_reader, simple_all)
{
    string path("sample_csv_path");
    ofstream ostream(path);
    ostream << CSV_TAB_ALL;
    ostream.close();

    {
        csv_file_reader reader(path, 0, new tabpunct);
        EXPECT_TRUE(bool(reader));
        EXPECT_EQ(reader(), CSV_HEADER);
        EXPECT_TRUE(bool(reader));
        EXPECT_EQ(reader(), CSV_ROW);
        EXPECT_FALSE(bool(reader));
    }
    EXPECT_TRUE(remove_file(path));
}


TEST(csv_file_reader, move)
{
    string path("sample_csv_path");
    ofstream ostream(path);
    ostream << CSV_TAB_ALL;
    ostream.close();

    {
        csv_file_reader r1(path, 0, new tabpunct), r2;
        r1.swap(r2);
        EXPECT_FALSE(bool(r1));
        EXPECT_TRUE(bool(r2));
        EXPECT_EQ(r2(), CSV_HEADER);
        EXPECT_TRUE(bool(r2));
        EXPECT_EQ(r2(), CSV_ROW);
        EXPECT_FALSE(bool(r2));
    }
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_string_reader, simple_all)
{
    csv_string_reader reader(CSV_SIMPLE_ALL);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_HEADER);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_ROW);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_string_reader, move)
{
    csv_string_reader r1(CSV_SIMPLE_ALL), r2;
    r1.swap(r2);
    EXPECT_FALSE(bool(r1));
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_HEADER);
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_ROW);
    EXPECT_FALSE(bool(r2));
}

// SIMPLE WRITER


TEST(csv_stream_writer, simple_all)
{
    ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_ALL);
        writer(CSV_HEADER);
        writer(CSV_ROW);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_ALL);
}


TEST(csv_stream_writer, simple_minimal)
{
    ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_MINIMAL);
        writer(CSV_HEADER);
        writer(CSV_ROW);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_MINIMAL);
}


TEST(csv_stream_writer, punctuation)
{
    ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_ALL, new tabpunct);
        writer(CSV_HEADER);
        writer(CSV_ROW);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


TEST(csv_stream_writer, move)
{
    ostringstream sstream;
    {
        csv_stream_writer w1(sstream, CSV_QUOTE_MINIMAL), w2;
        w1.swap(w2);
        w2(CSV_HEADER);
        w2(CSV_ROW);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_MINIMAL);
}


#if defined(BUILD_FILESYSTEM)

TEST(csv_file_writer, simple_all)
{
    string path("sample_csv_path");
    {
        csv_file_writer writer(path, CSV_QUOTE_ALL, new tabpunct);
        writer(CSV_HEADER);
        writer(CSV_ROW);
    }

    stringstream sstream;
    ifstream istream(path);
    sstream << istream.rdbuf();
    istream.close();

    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
    EXPECT_TRUE(remove_file(path));
}


TEST(csv_file_writer, move)
{
    string path("sample_csv_path");
    {
        csv_file_writer w1(path, CSV_QUOTE_ALL, new tabpunct), w2;
        w1.swap(w2);
        w2(CSV_HEADER);
        w2(CSV_ROW);
    }

    stringstream sstream;
    ifstream istream(path);
    sstream << istream.rdbuf();
    istream.close();

    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_string_writer, simple_all)
{
    csv_string_writer writer(CSV_QUOTE_ALL, new tabpunct);
    writer(CSV_HEADER);
    writer(CSV_ROW);
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


TEST(csv_string_writer, move)
{
    csv_string_writer w1(CSV_QUOTE_ALL, new tabpunct), w2;
    w1.swap(w2);
    w2(CSV_HEADER);
    w2(CSV_ROW);
    EXPECT_EQ(replace(w2.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}

// DICT READER

TEST(csv_dict_stream_reader, simple_all)
{
    istringstream sstream(CSV_SIMPLE_ALL);
    csv_dict_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_MAP);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_dict_stream_reader, simple_minimal)
{
    istringstream sstream(CSV_SIMPLE_MINIMAL);
    csv_dict_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_MAP);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_dict_stream_reader, skip)
{
    istringstream sstream(CSV_SKIP2_ALL);
    csv_dict_stream_reader reader(sstream, 2);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_MAP);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_dict_stream_reader, iterator)
{
    istringstream sstream(CSV_SIMPLE_ALL);
    csv_dict_stream_reader reader(sstream);
    auto first = reader.begin();
    auto last = reader.end();
    vector<csv_map> rows(first, last);
    EXPECT_EQ(rows.size(), 1);
    EXPECT_EQ(rows[0], CSV_MAP);
}


TEST(csv_dict_stream_reader, punctuation)
{
    istringstream sstream(CSV_TAB_ALL);
    csv_dict_stream_reader reader(sstream, 0, new tabpunct);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_MAP);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_dict_stream_reader, move)
{
    istringstream sstream(CSV_SIMPLE_MINIMAL);
    csv_dict_stream_reader r1(sstream), r2;
    r1.swap(r2);
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_MAP);
    EXPECT_FALSE(bool(r2));
}


#if defined(BUILD_FILESYSTEM)

TEST(csv_dict_file_reader, simple_all)
{
    string path("sample_csv_path");
    ofstream ostream(path);
    ostream << CSV_TAB_ALL;
    ostream.close();

    {
        csv_dict_file_reader reader(path, 0, new tabpunct);
        EXPECT_TRUE(bool(reader));
        EXPECT_EQ(reader(), CSV_MAP);
        EXPECT_FALSE(bool(reader));
    }
    EXPECT_TRUE(remove_file(path));
}


TEST(csv_dict_file_reader, move)
{
    string path("sample_csv_path");
    ofstream ostream(path);
    ostream << CSV_TAB_ALL;
    ostream.close();

    {
        csv_dict_file_reader r1(path, 0, new tabpunct), r2;
        r1.swap(r2);
        EXPECT_TRUE(bool(r2));
        EXPECT_EQ(r2(), CSV_MAP);
        EXPECT_FALSE(bool(r2));
    }
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_dict_string_reader, simple_all)
{
    csv_dict_string_reader reader(CSV_SIMPLE_ALL);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), CSV_MAP);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_dict_string_reader, move)
{
    csv_dict_string_reader r1(CSV_SIMPLE_ALL), r2;
    r1.swap(r2);
    EXPECT_TRUE(bool(r2));
    EXPECT_EQ(r2(), CSV_MAP);
    EXPECT_FALSE(bool(r2));
}


// DICT WRITER

TEST(csv_dict_stream_writer, simple_all)
{
    ostringstream sstream;
    {
        csv_dict_stream_writer writer(sstream, CSV_HEADER, CSV_QUOTE_ALL);
        writer(CSV_MAP);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_ALL);
}


TEST(csv_dict_stream_writer, simple_minimal)
{
    ostringstream sstream;
    {
        csv_dict_stream_writer writer(sstream, CSV_HEADER, CSV_QUOTE_MINIMAL);
        writer(CSV_MAP);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_MINIMAL);
}


TEST(csv_dict_stream_writer, punctuation)
{
    ostringstream sstream;
    {
        csv_dict_stream_writer writer(sstream, CSV_HEADER, CSV_QUOTE_ALL, new tabpunct);
        writer(CSV_MAP);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


TEST(csv_dict_stream_writer, move)
{
    ostringstream sstream;
    {
        csv_dict_stream_writer w1(sstream, CSV_HEADER, CSV_QUOTE_MINIMAL), w2;
        w1.swap(w2);
        w2(CSV_MAP);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_MINIMAL);
}

#if defined(BUILD_FILESYSTEM)

TEST(csv_dict_file_writer, simple_all)
{
    string path("sample_csv_path");
    {
        csv_dict_file_writer writer(path, CSV_HEADER, CSV_QUOTE_ALL, new tabpunct);
        writer(CSV_MAP);
    }

    stringstream sstream;
    ifstream istream(path);
    sstream << istream.rdbuf();
    istream.close();

    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
    EXPECT_TRUE(remove_file(path));
}


TEST(csv_dict_file_writer, move)
{
    string path("sample_csv_path");
    {
        csv_dict_file_writer w1(path, CSV_HEADER, CSV_QUOTE_ALL, new tabpunct), w2;
        w1.swap(w2);
        w2(CSV_MAP);
    }

    stringstream sstream;
    ifstream istream(path);
    sstream << istream.rdbuf();
    istream.close();

    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_dict_string_writer, simple_all)
{
    csv_dict_string_writer writer(CSV_HEADER, CSV_QUOTE_ALL, new tabpunct);
    writer(CSV_MAP);
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


TEST(csv_dict_string_writer, move)
{
    csv_dict_string_writer w1(CSV_HEADER, CSV_QUOTE_ALL, new tabpunct), w2;
    w1.swap(w2);
    w2(CSV_MAP);
    EXPECT_EQ(replace(w2.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}
