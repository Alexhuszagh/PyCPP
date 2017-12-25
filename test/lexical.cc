//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical conversion unittests.
 */

#include <pycpp/lexical.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// ENUM
// ----


enum class enum_test: uint32_t
{
    member = 0,
};


// TESTS
// -----


TEST(lexical, format)
{
    EXPECT_EQ(lexical(nullptr), NULL_STRING);
    EXPECT_EQ(lexical(enum_test::member), "0");
    EXPECT_EQ(lexical(true), TRUE_STRING);
    EXPECT_EQ(lexical(false), FALSE_STRING);
    EXPECT_EQ(lexical('A'), "A");
    EXPECT_EQ(lexical('\0'), string("\0", 1));
    EXPECT_EQ(lexical(1), "1");
    EXPECT_EQ(lexical(1.0), "1");
}


TEST(lexical, extract)
{
    EXPECT_EQ(lexical<std::nullptr_t>(NULL_STRING), nullptr);
    ASSERT_THROW(lexical<std::nullptr_t>(FALSE_STRING), runtime_error);
    EXPECT_EQ(lexical<bool>(TRUE_STRING), true);
    EXPECT_EQ(lexical<bool>(FALSE_STRING), false);
    ASSERT_THROW(lexical<bool>(NULL_STRING), runtime_error);
    EXPECT_EQ(lexical<char>("f"), 'f');
    EXPECT_EQ(lexical<unsigned char>("f"), 'f');
    ASSERT_THROW(lexical<char>(""), runtime_error);
    ASSERT_THROW(lexical<unsigned char>(""), runtime_error);
    EXPECT_EQ(lexical<short>("-1"), -1);
    EXPECT_EQ(lexical<int>("-1"), -1);
    EXPECT_EQ(lexical<long>("-1"), -1);
    EXPECT_EQ(lexical<long long>("-1"), -1);
    EXPECT_EQ(lexical<unsigned short>("5"), 5);
    EXPECT_EQ(lexical<unsigned int>("5"), 5);
    EXPECT_EQ(lexical<unsigned long>("5"), 5);
    EXPECT_EQ(lexical<unsigned long long>("5"), 5);
    EXPECT_EQ(lexical<enum_test>("0"), enum_test::member);
}
