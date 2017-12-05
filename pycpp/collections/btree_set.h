//  :copyright: (c) 2013 Google Inc. All Rights Reserved.
//  :copyright: Modified (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast ordered set using B-trees.
 */

#pragma once

#include <pycpp/collections/btree.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// The btree_set class is needed mainly for its constructors.
template <
    typename Key,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<Key>,
    int TargetNodeSize = 256
>
class btree_set: public btree_detail::btree_unique_container<
        btree_detail::btree<btree_detail::btree_set_params<Key, Compare, Alloc, TargetNodeSize>>
    >
{
    using self_type = btree_set<Key, Compare, Alloc, TargetNodeSize>;
    using params_type = btree_detail::btree_set_params<Key, Compare, Alloc, TargetNodeSize>;
    using btree_type = btree_detail::btree<params_type>;
    using super_type = btree_detail::btree_unique_container<btree_type>;

public:
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;

    // Default constructor.
    btree_set(const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_set(const self_type& x):
        super_type(x)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_set(InputIterator b, InputIterator e,
              const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}
};


template <typename K, typename C, typename A, int N>
inline void swap(btree_set<K, C, A, N>& x, btree_set<K, C, A, N>& y)
{
    x.swap(y);
}

// The btree_multiset class is needed mainly for its constructors.
template <
    typename Key,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<Key>,
    int TargetNodeSize = 256
>
class btree_multiset: public btree_detail::btree_multi_container<
        btree_detail::btree<btree_detail::btree_set_params<Key, Compare, Alloc, TargetNodeSize>>
    >
{

    using self_type = btree_multiset<Key, Compare, Alloc, TargetNodeSize>;
    using params_type = btree_detail::btree_set_params<Key, Compare, Alloc, TargetNodeSize>;
    using btree_type = btree_detail::btree<params_type>;
    using super_type = btree_detail::btree_multi_container<btree_type>;

public:
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;

    // Default constructor.
    btree_multiset(const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_multiset(const self_type& x):
        super_type(x)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_multiset(InputIterator b, InputIterator e,
                   const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}
};

template <typename K, typename C, typename A, int N>
inline void swap(btree_multiset<K, C, A, N>& x, btree_multiset<K, C, A, N>& y)
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
