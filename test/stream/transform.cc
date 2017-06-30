//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Transformable stream unittests.
 */

#include "stream/transform.h"

#include <gtest/gtest.h>
#include <sstream>
#include <string>


// FUNCTIONS
// ---------

streamsize_pair doublechars(const void* src, size_t srclen,
    void* dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = std::min(srclen, dstlen) * char_size;
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    for (size_t i = 0; i < bytes; ++i) {
        dst_[2*i] = src_[i];
        dst_[2*i + 1] = src_[i];
    }

    return std::make_pair(bytes, 2*bytes);
}

// TESTS
// -----


TEST(transform_istream, nocallback)
{
    std::istringstream sstream("This is a message");
    transform_istream stream(sstream);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "This is a message");
}


TEST(transform_istream, seekg)
{
    std::istringstream sstream("This is a message");
    transform_istream stream(sstream);
    std::string line;

    stream.seekg(5);
    std::getline(stream, line);
    EXPECT_EQ(line, "is a message");

    sstream.seekg(0);
    transform_istream stream2(sstream, doublechars);
    stream2.seekg(5);
    std::getline(stream2, line);
    EXPECT_EQ(line, "iiss  aa  mmeessssaaggee");
}


TEST(transform_istream, doublechars)
{
    std::istringstream sstream("This is a message");
    transform_istream stream(sstream, doublechars);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
}


// TODO: ostream
// TODO: ifstream
// TODO: ofstream
