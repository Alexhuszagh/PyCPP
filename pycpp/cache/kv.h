//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Key-value database cache.
 */

#pragma once

#include <pycpp/cache/kv_backend.h>
#include <pycpp/string/view.h>
#include <cstddef>

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
struct kv_iterator: std::iterator<std::input_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, T> base_t;
    typedef kv_iterator self_t;
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
    typename Compare = std::less<Key>
>
struct kv_cache
{
public:
    // MEMBER TYPES
    // ------------
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = kv_iterator<value_type>;
    using const_iterator = kv_iterator<const value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // MEMBER FUNCTIONS
    // ----------------
    // TODO: need to use the comparator...
    kv_cache(const path_t&, kv_options = kv_none);
    ~kv_cache();

// TODO: need everything
private:
    void* db_;
    kv_options options_;
};


// IMPLEMENTATION
// --------------


template <typename K, typename T, typename C>
kv_cache<K, T, C>::kv_cache(const path_t& path, kv_options options):
    options_(options)
{
    kv_open(db_, path, options);
}


template <typename K, typename T, typename C>
kv_cache<K, T, C>::~kv_cache()
{
    kv_close(db_);
}


PYCPP_END_NAMESPACE
