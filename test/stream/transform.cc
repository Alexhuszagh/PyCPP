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

void doublechars(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = std::min(srclen, dstlen / 2) * char_size;
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    // copy bytes
    while (bytes--) {
        *dst_++ = *src_;
        *dst_++ = *src_++;
    }

    // reassign to buffer
    src = (const void*) src_;
    dst = (void*) dst_;
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
