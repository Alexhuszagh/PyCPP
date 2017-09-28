//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief CSV unittests.
 */

#include <pycpp/csv.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// CONSTANTS
// ---------

static const std::string CSV_SIMPLE = {34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_SKIP2 = {10, 10, 34, 84, 104, 105, 115, 34, 44, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 44, 34, 77, -61, -86, 109, 101, 115, 34, 44, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 44, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 44, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 44, 34, 44, 34, -16, -97, -101, -126, 34, 10};
static const std::string CSV_TAB = {34, 84, 104, 105, 115, 34, 9, 34, -28, -67, -112, -24, -105, -92, 32, -27, -71, -71, -27, -92, -85, 34, 9, 34, 77, -61, -86, 109, 101, 115, 34, 9, 34, -20, -71, -100, -22, -75, -84, 34, 10, 34, -32, -72, -96, -32, -72, -124, -32, -72, -89, -32, -72, -78, 34, 9, 34, 32, -39, -125, -39, -118, -40, -88, -39, -120, -40, -79, -40, -81, 32, -39, -124, -39, -124, -39, -125, -40, -86, -40, -89, -40, -88, -40, -87, 32, -40, -88, -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, 34, 9, 34, -29, -126, -90, -29, -126, -91, -29, -126, -83, -29, -125, -91, 9, 34, 9, 34, -16, -97, -101, -126, 34, 10};
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


TEST(csv_stream_reader, simple)
{
    std::istringstream sstream(CSV_SIMPLE);
    csv_stream_reader reader(sstream);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, skip)
{
    std::istringstream sstream(CSV_SKIP2);
    csv_stream_reader reader(sstream, 2);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}


TEST(csv_stream_reader, iterator)
{
    std::istringstream sstream(CSV_SIMPLE);
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
    std::istringstream sstream(CSV_TAB);
    csv_stream_reader reader(sstream);
    reader.punctuation(tabpunct());
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW1);
    EXPECT_TRUE(bool(reader));
    EXPECT_EQ(reader(), ROW2);
    EXPECT_FALSE(bool(reader));
}
