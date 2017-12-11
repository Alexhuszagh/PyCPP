//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical enumerated value unittests.
 */

#include <pycpp/lexical/enum.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// ENUM
// ----

enum enum_test
{
    enum_first = 1,
    enum_second,
    enum_third,
};


enum class enum_class_test
{
    first = 1,
    second,
    third,
};

// TESTS
// -----


TEST(lexical_enum_formatter, lexical_enum_formatter)
{
    EXPECT_EQ(lexical_enum_formatter(enum_first).string(), string_view("1"));
    EXPECT_EQ(lexical_enum_formatter(enum_class_test::first).string(), string_view("1"));
}


TEST(lexical_enum_extractor, lexical_enum_extractor)
{
    EXPECT_EQ(enum_test(lexical_enum_extractor("1")), enum_first);
    EXPECT_EQ(enum_class_test(lexical_enum_extractor("1")), enum_class_test::first);
}
