//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Transformable stream unittests.
 */

#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/string.h>
#include <pycpp/stream/filter.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// FUNCTIONS
// ---------

void doublechars(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = min(srclen, dstlen / 2) * char_size;
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


TEST(filter_istream, nocallback)
{
    istringstream sstream("This is a message");
    filter_istream stream(sstream);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "This is a message");
}


TEST(filter_istream, doublechars)
{
    istringstream sstream("This is a message");
    filter_istream stream(sstream, doublechars);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
}


TEST(filter_ostream, nocallback)
{
    ostringstream sstream;
    {
        filter_ostream stream(sstream);
        stream << "This is a message";
    }
    EXPECT_EQ(sstream.str(), "This is a message");
}


TEST(filter_ostream, doublechars)
{
    ostringstream sstream;
    {
        filter_ostream stream(sstream, doublechars);
        stream << "This is a message";
    }
    EXPECT_EQ(sstream.str(), "TThhiiss  iiss  aa  mmeessssaaggee");
}
