//  :copyright: (c) 2013 Google Inc. All Rights Reserved.
//  :copyright: Modified (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast ordered map using B-trees.
 */

#pragma once

#include <pycpp/collections/btree.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// The btree_map class is needed mainly for its constructors.
template <
    typename Key,
    typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>,
    int TargetNodeSize = 256
>
class btree_map: public btree_detail::btree_map_container<
        btree_detail::btree<btree_detail::btree_map_params<Key, Value, Compare, Alloc, TargetNodeSize>>
    >
{

    using self_type = btree_map<Key, Value, Compare, Alloc, TargetNodeSize>;
    using params_type = btree_detail::btree_map_params<Key, Value, Compare, Alloc, TargetNodeSize>;
    using btree_type = btree_detail::btree<params_type>;
    using super_type = btree_detail::btree_map_container<btree_type>;

public:
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;

    // Default constructor.
    btree_map(const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_map(const self_type &x):
        super_type(x)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_map(InputIterator b, InputIterator e,
              const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}
};


template <typename K, typename V, typename C, typename A, int N>
inline void swap(btree_map<K, V, C, A, N>& x, btree_map<K, V, C, A, N>& y)
{
    x.swap(y);
}

// The btree_multimap class is needed mainly for its constructors.
template <
    typename Key,
    typename Value,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value> >,
    int TargetNodeSize = 256
>
class btree_multimap: public btree_detail::btree_multi_container<
        btree_detail::btree<btree_detail::btree_map_params<Key, Value, Compare, Alloc, TargetNodeSize>>
    >
{

    using self_type = btree_multimap<Key, Value, Compare, Alloc, TargetNodeSize>;
    using params_type = btree_detail::btree_map_params<Key, Value, Compare, Alloc, TargetNodeSize>;
    using btree_type = btree_detail::btree<params_type>;
    using super_type = btree_detail::btree_multi_container<btree_type>;

public:
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;
    using data_type = typename btree_type::data_type;
    using mapped_type = typename btree_type::mapped_type;

    // Default constructor.
    btree_multimap(const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_multimap(const self_type& x):
        super_type(x)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_multimap(InputIterator b, InputIterator e,
                   const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}
};


template <typename K, typename V, typename C, typename A, int N>
inline void swap(btree_multimap<K, V, C, A, N>& x,
                 btree_multimap<K, V, C, A, N>& y)
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
