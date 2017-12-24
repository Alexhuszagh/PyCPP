//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Key-value database cache.
 */

#pragma once

#include <pycpp/cache/kv_backend.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/string_view.h>
#include <pycpp/stl/utility.h>
#include <stddef.h>

PYCPP_BEGIN_NAMESPACE

// TODO: use the C api for the comparator...
// static void CmpDestroy(void* arg);
// static int CmpCompare(void* arg, const char* a, size_t alen, const char* b, size_t blen);
// static const char* CmpName(void* arg)
// leveldb_comparator_t* cmp;
// cmp = leveldb_comparator_create(NULL, CmpDestroy, CmpCompare, CmpName);
// leveldb_comparator_destroy(cmp);

// TODO: I need lexi here....
// I need lexical cast conversions...

// DECLARATION
// -----------

/**
 *  \brief STL-like wrapper around a key-value database iterator.
 */
template <typename T>
struct kv_iterator: iterator<input_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    using base_t = iterator<input_iterator_tag, T>;
    using self_t = kv_iterator;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

// TODO: need everything

private:
    void* iterator_;
};


// TODO: there's an issue
// kv_cache **only** takes string arguments
// Most classes store pointers to elements.
// We need to ensure there's a to_string, and from_string method
// for each class, or some shit...
// Which is quite a lot of work considering it won't work
// with STL containers out-of-the-box.

/**
 *  \brief Map-like wrapper around a key-value database store.
 */
template <
    typename Key,
    typename T,
    typename Compare = less<Key>,
    typename Allocator = allocator<pair<const Key, T>>
    // TODO: do I need an allocator???
>
struct kv_cache
{
public:
    // MEMBER TYPES
    // ------------
    using key_type = Key;
    using mapped_type = T;
    using value_type = pair<const Key, T>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = kv_iterator<value_type>;
    using const_iterator = kv_iterator<const value_type>;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;
    using allocator_type = Allocator;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: need to use the comparator...
    // TODO: need the allocator too
    // TODO: noexcept?
    kv_cache(const path_view_t&, kv_options = kv_none);
    ~kv_cache();

// TODO: need everything
private:
    void* db_;
    kv_options options_;
};

// SPECIALIZATION
// --------------

template <typename Key, typename T, typename Compare, typename Allocator>
struct is_relocatable<kv_cache<K, T, Compare, Allocator>>: true_type
{};

// IMPLEMENTATION
// --------------


template <typename K, typename T, typename C, typename A>
kv_cache<K, T, C, A>::kv_cache(const path_view_t& path, kv_options options):
    options_(options)
{
    kv_open(db_, path, options);
}


template <typename K, typename T, typename C, typename A>
kv_cache<K, T, C, A>::~kv_cache()
{
    kv_close(db_);
}


PYCPP_END_NAMESPACE
