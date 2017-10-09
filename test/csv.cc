//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief CSV unittests.
 */

#include <pycpp/csv.h>
#include <pycpp/filesystem.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/string/string.h>
#include <pycpp/string/whitespace.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// CONSTANTS
// ---------

static const std::string CSV_SIMPLE_ALL = {34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_SKIP2_ALL = {10, 10, 34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_TAB_ALL = {34, 84, 104, 105, 115, 34, 9, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 9, 34, 77, -61, -86, 109, 101, 115, 34, 9, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 9, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 9, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 9, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_SIMPLE_MINIMAL = {84, 104, 105, 115, 44, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 44, 77, -61, -86, 109, 101, 115, 44, -20, -71, -100, -22, -75, -84, 10, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 44, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, -16, -97, -101, -126, 10};
static const csv_row ROW1 = {
    {84, 104, 105, 115},
    {-28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85},
    {77, -61, -86, 109, 101, 115},
    {-20, -71, -100, -22, -75, -84}
};
static const csv_row ROW2 = {
    {-32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78},
    {32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118},
    {-29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44},
    {-16, -97, -101, -126}
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
    std::istringstream sstream(CSV_SIMPLE_ALL);
    csv_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, simple_minimal)
{
    std::istringstream sstream(CSV_SIMPLE_MINIMAL);
    csv_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, skip)
{
    std::istringstream sstream(CSV_SKIP2_ALL);
    csv_stream_reader reader(sstream, 2);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, iterator)
{
    std::istringstream sstream(CSV_SIMPLE_ALL);
    csv_stream_reader reader(sstream);
    auto first = reader.begin();
    auto last = reader.end();
    std::vector<csv_row> rows(first, last);
    EXPECT_EQ(rows.size(), 2);
    EXPECT_EQ(rows[0], ROW1);
    EXPECT_EQ(rows[1], ROW2);
}


TEST(csv_stream_reader, punctuation)
{
    std::istringstream sstream(CSV_TAB_ALL);
    csv_stream_reader reader(sstream);
    reader.punctuation(new tabpunct);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


#if defined(BUILD_FILESYSTEM)

TEST(csv_file_reader, simple_all)
{
    std::string path("sample_csv_path");
    ofstream ostream(path);
    ostream << CSV_TAB_ALL;
    ostream.close();

    {
        csv_file_reader reader(path);
        reader.punctuation(new tabpunct);
        EXPECT_TRUE(bool(reader));
        EXPECT_EQ(reader(), ROW1);
        EXPECT_TRUE(bool(reader));
        EXPECT_EQ(reader(), ROW2);
        EXPECT_FALSE(bool(reader));
    }
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_string_reader, simple_all)
{
    csv_string_reader reader(CSV_SIMPLE_ALL);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


// SIMPLE WRITER


TEST(csv_stream_writer, simple_all)
{
    std::ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_ALL);
        writer(ROW1);
        writer(ROW2);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_ALL);
}


TEST(csv_stream_writer, simple_minimal)
{
    std::ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_MINIMAL);
        writer(ROW1);
        writer(ROW2);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_SIMPLE_MINIMAL);
}


TEST(csv_stream_writer, punctuation)
{
    std::ostringstream sstream;
    {
        csv_stream_writer writer(sstream, CSV_QUOTE_ALL);
        writer.punctuation(new tabpunct);
        writer(ROW1);
        writer(ROW2);
    }
    // force POSIX-like newlines
    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


#if defined(BUILD_FILESYSTEM)

TEST(csv_file_writer, simple_all)
{
    std::string path("sample_csv_path");
    {
        csv_file_writer writer(path, CSV_QUOTE_ALL);
        writer.punctuation(new tabpunct);
        writer(ROW1);
        writer(ROW2);
    }

    std::stringstream sstream;
    ifstream istream(path);
    sstream << istream.rdbuf();
    istream.close();

    EXPECT_EQ(replace(sstream.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
    EXPECT_TRUE(remove_file(path));
}

#endif          // BUILD_FILESYSTEM


TEST(csv_string_writer, simple_all)
{
    csv_string_writer writer(CSV_QUOTE_ALL);
    writer.punctuation(new tabpunct);
    writer(ROW1);
    writer(ROW2);
    EXPECT_EQ(replace(writer.str(), NEWLINE, POSIX_NEWLINE), CSV_TAB_ALL);
}


// DICT READER

// TODO: here

// DICT WRITER

// TODO: here
