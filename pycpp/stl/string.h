//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL string alias.
 */

#pragma once

#include <pycpp/stl/memory.h>
#if defined(USE_XXHASH) || defined(USE_POLYMORPHIC_ALLOCATOR)
#   include <pycpp/stl/detail/xxhash.h>
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

PYCPP_SPECIALIZE_HASH_STRING(hash, string);
PYCPP_SPECIALIZE_HASH_STRING(hash, wstring);
PYCPP_SPECIALIZE_HASH_STRING(hash, u16string);
PYCPP_SPECIALIZE_HASH_STRING(hash, u32string);

#endif          // USE_POLYMORPHIC_ALLOCATOR

// Specialize std::basic_string types for xxhash
#if USE_XXHASH

PYCPP_SPECIALIZE_HASH_STRING(hash, std::string);
PYCPP_SPECIALIZE_HASH_STRING(hash, std::wstring);
PYCPP_SPECIALIZE_HASH_STRING(hash, std::u16string);
PYCPP_SPECIALIZE_HASH_STRING(hash, std::u32string);

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
