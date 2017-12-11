//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/secure/string.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

template <typename T, template <typename> class Hash>
static void test_hash()
{
    using result_type = decltype(Hash<T>()(std::declval<T>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}

// TESTS
// -----


TEST(secure_string, secure_string)
{
    // default initializer
    secure_string str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_GT(str.capacity(), 0);
    EXPECT_GT(str.max_size(), 0);

    // intializer list
    str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    EXPECT_EQ(str.size(), 14);
    EXPECT_GT(str.capacity(), 14);

    // move + secure_string(const_pointer)
    str = secure_string("This is some data");
    EXPECT_EQ(str.size(), 17);
    EXPECT_GT(str.capacity(), 17);

    // move + secure_string(const_pointer)
    str = secure_string("This is some data", 8);
    EXPECT_EQ(str.size(), 8);
    EXPECT_GT(str.capacity(), 8);

    // move + fill
    str = secure_string(15, '0');
    EXPECT_EQ(str.size(), 15);
    EXPECT_GT(str.capacity(), 15);

    // iterator construct
    secure_string str2(str.begin(), str.end());
    EXPECT_EQ(str2.size(), 15);
    EXPECT_GT(str2.capacity(), 15);
    EXPECT_EQ(str, str2);
}


TEST(secure_string, swap)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string other;
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    std::swap(str, other);
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(other.size(), 14);

    str.swap(other);
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);
}


TEST(secure_string, relational)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string other;

    // eq
    EXPECT_TRUE(str == str);
    EXPECT_FALSE(str == other);
    EXPECT_TRUE(str == str.view());
    EXPECT_FALSE(str == str.data());
    EXPECT_FALSE(str.data() == str);

    // ne
    EXPECT_FALSE(str != str);
    EXPECT_TRUE(str != other);
    EXPECT_FALSE(str != str.view());
    EXPECT_TRUE(str != str.data());
    EXPECT_TRUE(str.data() != str);

    // lt
    EXPECT_FALSE(str < str);
    EXPECT_FALSE(str < other);
    EXPECT_FALSE(str < str.view());
    EXPECT_FALSE(str < str.data());
    EXPECT_TRUE(str.data() < str);

    // le
    EXPECT_TRUE(str <= str);
    EXPECT_FALSE(str <= other);
    EXPECT_TRUE(str <= str.view());
    EXPECT_FALSE(str <= str.data());
    EXPECT_TRUE(str.data() <= str);

    // gt
    EXPECT_FALSE(str > str);
    EXPECT_TRUE(str > other);
    EXPECT_FALSE(str > str.view());
    EXPECT_TRUE(str > str.data());
    EXPECT_FALSE(str.data() > str);

    // ge
    EXPECT_TRUE(str >= str);
    EXPECT_TRUE(str >= other);
    EXPECT_TRUE(str >= str.view());
    EXPECT_TRUE(str >= str.data());
    EXPECT_FALSE(str.data() >= str);
}


TEST(secure_string, concatenation)
{
    secure_string str1("exa");
    secure_string str2("mple");
    secure_string str3(str1), str4(str2), str5(str1), str6(str2);

    // secure_string + secure_string
    EXPECT_EQ(str3 + str4, "example");
    EXPECT_EQ(std::move(str3) + str4, "example");
    EXPECT_EQ(str5 + std::move(str4), "example");
    EXPECT_EQ(std::move(str5) + std::move(str6), "example");

    // secure_string + const char*
    str3 = str1, str4 = str2, str5 = str1, str6 = str2;
    EXPECT_EQ(str3 + "mple", "example");
    EXPECT_EQ(std::move(str3) + "mple", "example");
    EXPECT_EQ("exa" + str4, "example");
    EXPECT_EQ("exa" + std::move(str4), "example");

    // secure_string + char
    str3 = str1, str4 = str2, str5 = str1, str6 = str2;
    EXPECT_EQ(str3 + 'm', "exam");
    EXPECT_EQ(std::move(str3) + 'm', "exam");
    EXPECT_EQ('a' + str4, "ample");
    EXPECT_EQ('a' + std::move(str4), "ample");

    // secure_string + view
    str3 = str1, str4 = str2, str5 = str1, str6 = str2;
    EXPECT_EQ(str3 + string_view("mple"), "example");
    EXPECT_EQ(std::move(str3) + string_view("mple"), "example");
    EXPECT_EQ(string_view("exa") + str4, "example");
    EXPECT_EQ(string_view("exa") + std::move(str4), "example");
}


TEST(secure_string, memory)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};

    str.noaccess();
    str.readonly();
    EXPECT_EQ(str.front(), '\0');
    str.readwrite();
    EXPECT_EQ(str.front(), '\0');
    str.mlock();
    str.munlock();
    EXPECT_EQ(str.front(), '\0');
}


TEST(secure_string, iterator)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string reversed(str.rbegin(), str.rend());
    EXPECT_EQ(secure_string(str.begin(), str.end()), str);
    EXPECT_EQ(secure_string(str.rbegin(), str.rend()), reversed);
}


TEST(secure_string, capacity)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string other;

    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    EXPECT_EQ(str.length(), 14);
    EXPECT_EQ(other.length(), 0);

    EXPECT_FALSE(str.empty());
    EXPECT_TRUE(other.empty());

    str.clear();
    EXPECT_TRUE(str.empty());
    EXPECT_GT(str.capacity(), 0);

    str.reserve(50);
    EXPECT_TRUE(str.empty());
    EXPECT_GE(str.capacity(), 50);

    str.shrink_to_fit();
    EXPECT_TRUE(str.empty());
    EXPECT_LT(str.capacity(), 50);

    str.resize(50);
    EXPECT_EQ(str.size(), 50);
    EXPECT_GT(str.capacity(), 50);
}


TEST(secure_string, element)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};

    EXPECT_EQ(str[0], '\0');
    EXPECT_EQ(str.at(0), '\0');
    EXPECT_EQ(str[1], 'T');
    EXPECT_EQ(str.at(1), 'T');
    EXPECT_EQ(str.front(), '\0');
    EXPECT_EQ(str.back(), '\n');
}


TEST(secure_string, modifier)
{
    secure_string data = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string str;

    str.assign({0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10});
    EXPECT_EQ(str.size(), 14);

    str.assign(data, 4, 10);
    EXPECT_EQ(str.size(), 10);

    str.assign(data.data());
    EXPECT_EQ(str.size(), 0);

    str.assign(data.data());
    EXPECT_EQ(str.size(), 0);

    str.assign(data.data(), data.size());
    EXPECT_EQ(str.size(), 14);

    str.push_back('\0');
    EXPECT_EQ(str.size(), 15);
    EXPECT_EQ(str.back(), '\0');

    str += '\0';
    EXPECT_EQ(str.size(), 16);
    EXPECT_EQ(str.back(), '\0');

    str += secure_string("hello");
    EXPECT_EQ(str.size(), 21);

    str += "xy";
    EXPECT_EQ(str.size(), 23);

    str += {'a', 'b', 'c'};
    EXPECT_EQ(str.size(), 26);

    str.append(secure_string("de"));
    EXPECT_EQ(str.size(), 28);

    str.append(data.view());
    EXPECT_EQ(str.size(), 42);

    str.append(data, 5, 5);
    EXPECT_EQ(str.size(), 47);

    str.append("xyz\0");
    EXPECT_EQ(str.size(), 50);

    str.append("xyz\0", 4);
    EXPECT_EQ(str.size(), 54);

    // insert
    str = secure_string("example");
    EXPECT_EQ(str, "example");
    str.insert(0, "b");
    EXPECT_EQ(str, "bexample");

    str.insert(0, "bee", 1, 1);
    EXPECT_EQ(str, "ebexample");

    str.insert(2, 3, 'c');
    EXPECT_EQ(str, "ebcccexample");

    str.insert(str.begin() + 2, 3, 'd');
    EXPECT_EQ(str, "ebdddcccexample");

    str.insert(str.begin() + 2, 'f');
    EXPECT_EQ(str, "ebfdddcccexample");

    secure_string tmp("123");
    str.insert(str.begin() + 2, tmp.begin(), tmp.end());
    EXPECT_EQ(str, "eb123fdddcccexample");

    str.insert(str.begin() + 2, {'4', '5', '6'});
    EXPECT_EQ(str, "eb456123fdddcccexample");

    // replace
    str.replace(2, 3, "4");
    EXPECT_EQ(str, "eb4123fdddcccexample");

    str.replace(2, 4, "4123", 1, 1);
    EXPECT_EQ(str, "eb1fdddcccexample");

    str.replace(str.begin()+2, str.begin()+4, "f");
    EXPECT_EQ(str, "ebfdddcccexample");

    str.replace(1, 8, 3, 'd');
    EXPECT_EQ(str, "edddexample");

    str.replace(str.begin()+1, str.begin()+4, 3, 'c');
    EXPECT_EQ(str, "ecccexample");

}



TEST(secure_string, operations)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string null;
    secure_string non_null = {84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};

    // find
    EXPECT_EQ(str.find(secure_string("is")), 3);
    EXPECT_EQ(str.find("is"), 3);
    EXPECT_EQ(str.find(string_view("is")), 3);
    EXPECT_EQ(str.find("is", 0, 2), 3);
    EXPECT_EQ(str.find('i'), 3);

    // find_first_of
    EXPECT_EQ(str.find_first_of(secure_string("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi"), 2);
    EXPECT_EQ(str.find_first_of(string_view("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi", 0, 3), 2);
    EXPECT_EQ(str.find_first_of('h'), 2);

    EXPECT_EQ(str.find_first_of(secure_string("x")), secure_string::npos);
    EXPECT_EQ(str.find_first_of("x"), secure_string::npos);
    EXPECT_EQ(str.find_first_of(string_view("x")), secure_string::npos);
    EXPECT_EQ(str.find_first_of("x", 0, 1), secure_string::npos);
    EXPECT_EQ(str.find_first_of('x'), secure_string::npos);

    // find_first_not_of
    EXPECT_EQ(non_null.find_first_not_of(secure_string("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish"), 4);
    EXPECT_EQ(non_null.find_first_not_of(string_view("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish", 0, 4), 4);
    EXPECT_EQ(non_null.find_first_not_of('T'), 1);

    EXPECT_EQ(non_null.find_first_not_of(secure_string("Thisdta \n")), secure_string::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n"), secure_string::npos);
    EXPECT_EQ(non_null.find_first_not_of(string_view("Thisdta \n")), secure_string::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n", 0, 9), secure_string::npos);

    // rfind
    EXPECT_EQ(str.rfind(secure_string("is")), 6);
    EXPECT_EQ(str.rfind("is"), 6);
    EXPECT_EQ(str.rfind(string_view("is")), 6);
    EXPECT_EQ(str.rfind("is", 0, 2), secure_string::npos);
    EXPECT_EQ(str.rfind('i'), 6);

    EXPECT_EQ(str.rfind(secure_string("isx")), secure_string::npos);
    EXPECT_EQ(str.rfind("isx"), secure_string::npos);
    EXPECT_EQ(str.rfind(string_view("isx")), secure_string::npos);
    EXPECT_EQ(str.rfind("isx", 0, 3), secure_string::npos);
    EXPECT_EQ(str.rfind('x'), secure_string::npos);

    EXPECT_EQ(null.rfind('i'), secure_string::npos);

    // find_last_of
    EXPECT_EQ(str.find_last_of(secure_string("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi"), 7);
    EXPECT_EQ(str.find_last_of(string_view("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi", 0, 3), secure_string::npos);
    EXPECT_EQ(str.find_last_of('h'), 2);

    EXPECT_EQ(str.find_last_of(secure_string("x")), secure_string::npos);
    EXPECT_EQ(str.find_last_of("x"), secure_string::npos);
    EXPECT_EQ(str.find_last_of(string_view("x")), secure_string::npos);
    EXPECT_EQ(str.find_last_of("x", 0, 1), secure_string::npos);
    EXPECT_EQ(str.find_last_of('x'), secure_string::npos);

    // find_last_not_of
    EXPECT_EQ(non_null.find_last_not_of(secure_string("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n"), 7);
    EXPECT_EQ(non_null.find_last_not_of(string_view("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n", 0, 4), 0);
    EXPECT_EQ(non_null.find_last_not_of('\n'), 11);

    EXPECT_EQ(non_null.find_last_not_of(secure_string("Thisdta \n")), secure_string::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n"), secure_string::npos);
    EXPECT_EQ(non_null.find_last_not_of(string_view("Thisdta \n")), secure_string::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n", 0, 9), secure_string::npos);
}


TEST(secure_string, conversions)
{
    secure_string str = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
    secure_string other;
    EXPECT_TRUE(bool(str));
    EXPECT_FALSE(bool(other));

    EXPECT_EQ(string_view(str), string_view(str));
    EXPECT_EQ(string_view(other), string_view(""));
}


TEST(secure_string, hash)
{
    // std::hash
    test_hash<secure_string, std::hash>();
    test_hash<secure_wstring, std::hash>();
    test_hash<secure_u16string, std::hash>();
    test_hash<secure_u32string, std::hash>();

    // hash
    test_hash<secure_string, hash>();
    test_hash<secure_wstring, hash>();
    test_hash<secure_u16string, hash>();
    test_hash<secure_u32string, hash>();
}
