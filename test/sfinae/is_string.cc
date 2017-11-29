//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief String-detection unittests.
 */

#include <pycpp/sfinae/is_string.h>
#include <gtest/gtest.h>
#include <string>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_string, is_string)
{
    using pair = std::pair<int, int>;

    // char*
    static_assert(is_charp<char*>::value, "");
    static_assert(is_charp<char* const>::value, "");
    static_assert(is_charp<char* volatile>::value, "");
    static_assert(is_charp<char* const volatile>::value, "");
    static_assert(!is_charp<const char*>::value, "");
    static_assert(!is_charp<wchar_t*>::value, "");
    static_assert(!is_charp<char16_t*>::value, "");
    static_assert(!is_charp<char32_t*>::value, "");
    static_assert(!is_charp<pair>::value, "");

    // const char*
    static_assert(is_const_charp<const char*>::value, "");
    static_assert(is_const_charp<const char* const>::value, "");
    static_assert(is_const_charp<const char* volatile>::value, "");
    static_assert(is_const_charp<const char* const volatile>::value, "");
    static_assert(!is_const_charp<char*>::value, "");
    static_assert(!is_const_charp<const wchar_t*>::value, "");
    static_assert(!is_const_charp<const char16_t*>::value, "");
    static_assert(!is_const_charp<const char32_t*>::value, "");
    static_assert(!is_const_charp<pair>::value, "");

    // wchar_t*
    static_assert(is_wcharp<wchar_t*>::value, "");
    static_assert(is_wcharp<wchar_t* const>::value, "");
    static_assert(is_wcharp<wchar_t* volatile>::value, "");
    static_assert(is_wcharp<wchar_t* const volatile>::value, "");
    static_assert(!is_wcharp<const wchar_t*>::value, "");
    static_assert(!is_wcharp<char*>::value, "");
    static_assert(!is_wcharp<char16_t*>::value, "");
    static_assert(!is_wcharp<char32_t*>::value, "");
    static_assert(!is_wcharp<pair>::value, "");

    // const wchar_t*
    static_assert(is_const_wcharp<const wchar_t*>::value, "");
    static_assert(is_const_wcharp<const wchar_t* const>::value, "");
    static_assert(is_const_wcharp<const wchar_t* volatile>::value, "");
    static_assert(is_const_wcharp<const wchar_t* const volatile>::value, "");
    static_assert(!is_const_wcharp<wchar_t*>::value, "");
    static_assert(!is_const_wcharp<const char*>::value, "");
    static_assert(!is_const_wcharp<const char16_t*>::value, "");
    static_assert(!is_const_wcharp<const char32_t*>::value, "");
    static_assert(!is_const_wcharp<pair>::value, "");

    // char16_t*
    static_assert(is_char16p<char16_t*>::value, "");
    static_assert(is_char16p<char16_t* const>::value, "");
    static_assert(is_char16p<char16_t* volatile>::value, "");
    static_assert(is_char16p<char16_t* const volatile>::value, "");
    static_assert(!is_char16p<const char16_t*>::value, "");
    static_assert(!is_char16p<char*>::value, "");
    static_assert(!is_char16p<wchar_t*>::value, "");
    static_assert(!is_char16p<char32_t*>::value, "");
    static_assert(!is_char16p<pair>::value, "");

    // const char16_t*
    static_assert(is_const_char16p<const char16_t*>::value, "");
    static_assert(is_const_char16p<const char16_t* const>::value, "");
    static_assert(is_const_char16p<const char16_t* volatile>::value, "");
    static_assert(is_const_char16p<const char16_t* const volatile>::value, "");
    static_assert(!is_const_char16p<char16_t*>::value, "");
    static_assert(!is_const_char16p<const char*>::value, "");
    static_assert(!is_const_char16p<const wchar_t*>::value, "");
    static_assert(!is_const_char16p<const char32_t*>::value, "");
    static_assert(!is_const_char16p<pair>::value, "");

    // char32_t*
    static_assert(is_char32p<char32_t*>::value, "");
    static_assert(is_char32p<char32_t* const>::value, "");
    static_assert(is_char32p<char32_t* volatile>::value, "");
    static_assert(is_char32p<char32_t* const volatile>::value, "");
    static_assert(!is_char32p<const char32_t*>::value, "");
    static_assert(!is_char32p<char*>::value, "");
    static_assert(!is_char32p<wchar_t*>::value, "");
    static_assert(!is_char32p<char16_t*>::value, "");
    static_assert(!is_char32p<pair>::value, "");

    // const char32_t*
    static_assert(is_const_char32p<const char32_t*>::value, "");
    static_assert(is_const_char32p<const char32_t* const>::value, "");
    static_assert(is_const_char32p<const char32_t* volatile>::value, "");
    static_assert(is_const_char32p<const char32_t* const volatile>::value, "");
    static_assert(!is_const_char32p<char32_t*>::value, "");
    static_assert(!is_const_char32p<const char*>::value, "");
    static_assert(!is_const_char32p<const wchar_t*>::value, "");
    static_assert(!is_const_char32p<const char16_t*>::value, "");
    static_assert(!is_const_char32p<pair>::value, "");

    // basic_string
    static_assert(is_stl_string<std::string>::value, "");
    static_assert(is_stl_string<std::wstring>::value, "");
    static_assert(is_stl_string<std::u16string>::value, "");
    static_assert(is_stl_string<std::u32string>::value, "");
    static_assert(!is_stl_string<char*>::value, "");
    static_assert(!is_stl_string<wchar_t*>::value, "");
    static_assert(!is_stl_string<char16_t*>::value, "");
    static_assert(!is_stl_string<char32_t*>::value, "");
    static_assert(!is_stl_string<pair>::value, "");

    // general
    static_assert(is_string<char*>::value, "");
    static_assert(is_string<wchar_t*>::value, "");
    static_assert(is_string<char16_t*>::value, "");
    static_assert(is_string<char32_t*>::value, "");
    static_assert(is_string<const char*>::value, "");
    static_assert(is_string<const wchar_t*>::value, "");
    static_assert(is_string<const char16_t*>::value, "");
    static_assert(is_string<const char32_t*>::value, "");
    static_assert(is_string<std::string>::value, "");
    static_assert(is_string<std::wstring>::value, "");
    static_assert(is_string<std::u16string>::value, "");
    static_assert(is_string<std::u32string>::value, "");
    static_assert(!is_string<pair>::value, "");
}
