//  :copyright: (c) 2013 Google Inc. All Rights Reserved.
//  :copyright: Modified (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast ordered map using B-trees.
 */

#pragma once

#include <pycpp/collections/btree.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// The btree_map class is needed mainly for its constructors.
template <
    typename Key,
    typename Value,
    typename Compare = less<Key>,
    typename Alloc = allocator<pair<const Key, Value>>,
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
    using value_type = typename btree_type::value_type;
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;

    // Default constructor.
    btree_map(const allocator_type& alloc):
        super_type(alloc)
    {}

    btree_map(const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_map(const self_type &x):
        super_type(x)
    {}

    btree_map(const self_type& x, const allocator_type& alloc):
        super_type(x, alloc)
    {}

    // Move constructor.
    btree_map(self_type&& x):
        super_type(forward<self_type>(x))
    {}

    btree_map(self_type&& x, const allocator_type& alloc):
        super_type(forward<self_type>(x), alloc)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_map(InputIterator b, InputIterator e,
              const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}

    template <typename InputIterator>
    btree_map(InputIterator b, InputIterator e,
              const allocator_type& alloc = allocator_type()):
        super_type(b, e, alloc)
    {}

    // Initializer list constructor
    btree_map(initializer_list<value_type> list,
              const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        this->insert(list.begin(), list.end());
    }

    btree_map(initializer_list<value_type> list,
              const allocator_type& alloc = allocator_type()):
        super_type(alloc)
    {
        this->insert(list.begin(), list.end());
    }

    // Copy assignment
    self_type& operator=(const self_type& x)
    {
        this->assign(x);
        return *this;
    }

    // Move assignment
    self_type& operator=(self_type&& x)
    {
        this->swap(x);
        return *this;
    }

    // Initializer list assignment
    self_type& operator=(initializer_list<value_type> list)
    {
        this->insert(list.begin(), list.end());
        return *this;
    }
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
    typename Compare = less<Key>,
    typename Alloc = allocator<pair<const Key, Value> >,
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
    using value_type = typename btree_type::value_type;
    using key_compare = typename btree_type::key_compare;
    using allocator_type = typename btree_type::allocator_type;
    using data_type = typename btree_type::data_type;
    using mapped_type = typename btree_type::mapped_type;

    // Default constructor.
    btree_multimap(const allocator_type& alloc):
        super_type(alloc)
    {}

    btree_multimap(const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_multimap(const self_type& x):
        super_type(x)
    {}

    btree_multimap(const self_type& x, const allocator_type& alloc):
        super_type(x, alloc)
    {}

    // Move constructor
    btree_multimap(self_type&& x):
        super_type(forward<self_type>(x))
    {}

    btree_multimap(self_type&& x, const allocator_type& alloc):
        super_type(forward<self_type>(x), alloc)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_multimap(InputIterator b, InputIterator e,
                   const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}

    template <typename InputIterator>
    btree_multimap(InputIterator b, InputIterator e,
                   const allocator_type& alloc = allocator_type()):
        super_type(b, e, alloc)
    {}

    // Initializer list constructor
    btree_multimap(initializer_list<value_type> list,
                   const key_compare& comp = key_compare(),
                   const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        this->insert(list.begin(), list.end());
    }

    btree_multimap(initializer_list<value_type> list,
                   const allocator_type& alloc = allocator_type()):
        super_type(alloc)
    {
        this->insert(list.begin(), list.end());
    }

    // Copy assignment
    self_type& operator=(const self_type& x)
    {
        this->assign(x);
        return *this;
    }

    // Move assignment
    self_type& operator=(self_type&& x)
    {
        this->swap(x);
        return *this;
    }

    // Initializer list assignment
    self_type& operator=(initializer_list<value_type> list)
    {
        this->insert(list.begin(), list.end());
        return *this;
    }
};


template <typename K, typename V, typename C, typename A, int N>
inline void swap(btree_multimap<K, V, C, A, N>& x,
                 btree_multimap<K, V, C, A, N>& y)
{
    x.swap(y);
}

PYCPP_END_NAMESPACE
