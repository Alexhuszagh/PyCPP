//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL string alias.
 */

#pragma once

#include <pycpp/stl/allocator.h>
#if defined(USE_XXHASH)
#   include <pycpp/hashlib/xxhash.h>
#endif
#include <string>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <
    typename Char,
    typename Traits = std::char_traits<Char>,
    typename Alloc = allocator<Char>
>
using basic_string = std::basic_string<Char, Traits, Alloc>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

// Specialize polymorphic hash aliases with default hash function.
#if USE_POLYMORPHIC_ALLOCATOR

template <>
struct hash<string>
{
    using argument_type = string;
    using result_type = size_t;

    inline size_t operator()(const string& x) const
    {
        using value_type = typename string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<wstring>
{
    using argument_type = wstring;
    using result_type = size_t;

    inline size_t operator()(const wstring& x) const
    {
        using value_type = typename wstring::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<u16string>
{
    using argument_type = u16string;
    using result_type = size_t;

    inline size_t operator()(const u16string& x) const
    {
        using value_type = typename u16string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<u32string>
{
    using argument_type = u32string;
    using result_type = size_t;

    inline size_t operator()(const u32string& x) const
    {
        using value_type = typename u32string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

#endif          // USE_POLYMORPHIC_ALLOCATOR

// Specialize std::basic_string types for xxhash
#if USE_XXHASH

template <>
struct hash<std::string>
{
    using argument_type = std::string;
    using result_type = size_t;

    inline size_t operator()(const std::string& x) const
    {
        using value_type = typename std::string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<std::wstring>
{
    using argument_type = std::wstring;
    using result_type = size_t;

    inline size_t operator()(const std::wstring& x) const
    {
        using value_type = typename std::wstring::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<std::u16string>
{
    using argument_type = std::u16string;
    using result_type = size_t;

    inline size_t operator()(const std::u16string& x) const
    {
        using value_type = typename std::u16string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<std::u32string>
{
    using argument_type = std::u32string;
    using result_type = size_t;

    inline size_t operator()(const std::u32string& x) const
    {
        using value_type = typename std::u32string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
