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

// Must specialize if we have a custom allocator or we want to use
// the faster hash.
#if USE_XXHASH || USE_POLYMORPHIC_ALLOCATOR

template <>
struct hash<string>
{
    inline size_t operator()(const string& x) const
    {
        using value_type = typename wstring::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<wstring>
{
    inline size_t operator()(const wstring& x) const
    {
        using value_type = typename wstring::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<u16string>
{
    inline size_t operator()(const u16string& x) const
    {
        using value_type = typename u16string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

template <>
struct hash<u32string>
{
    inline size_t operator()(const u32string& x) const
    {
        using value_type = typename u32string::value_type;
        return xxhash_string(x.data(), x.size() * sizeof(value_type));
    }
};

#endif          // USE_XXHASH || USE_POLYMORPHIC_ALLOCATOR

PYCPP_END_NAMESPACE
