//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Least-recently used cache.
 */

#pragma once

#include <pycpp/reference/core.h>
#include <functional>
#include <list>
#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// DECLARATION
// -----------

template <typename pair>
using transformed_pair = std::pair<typename pair::first_type, deref<typename pair::second_type>>;

template <typename pair>
using lru_cache_transform = std::function<transformed_pair<pair>(const pair&)>;

template <typename pair>
using lru_iterator = transform_iterator<pair, lru_cache_transform<pair>>;

}   /* detail */

// DECLARATION
// -----------

/**
 *  \brief O(1) LRU cache implemented via a hashtable and linked list.
 */
template <
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, Value>>
>
struct lru_cache
{
public:
    // MEMBER TYPES
    // ------------
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
//    using iterator = detail::lru_iterator<value_type>;
//    using const_iterator = detail::lru_iterator<const value_type>;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    // MEMBER FUNCTIONS
    // ----------------
    lru_cache(int cache_size);

    // CAPACITY
    size_type size() const;
    size_type cache_size() const;
    size_type max_size() const;
    bool empty() const noexcept;

    // ITERATORS
// TODO: implement...
//    iterator begin();
//    const_iterator begin() const;
//    const_iterator cbegin() const;
//    iterator end();
//    const_iterator end() const;
//    const_iterator cend() const;

    // ELEMENT ACCESS
//    mapped_type& operator[](const key_type&);
//    mapped_type& operator[](key_type&&);
    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;

protected:
    using list_type = std::list<value_type, allocator_type>;
    using list_iterator_type = typename list_type::iterator;
    using map_type = std::unordered_map<key_type, list_iterator_type, hasher, key_equal>;

    map_type map_;
    size_type cache_size_;
};

// IMPLEMENTATION
// --------------

template <typename K, typename V, typename H, typename P, typename A>
lru_cache<K, V, H, P, A>::lru_cache(int cache_size):
    cache_size_(cache_size)
{}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::cache_size() const -> size_type
{
    return cache_size_;
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::size() const -> size_type
{
    return map_.size();
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::max_size() const -> size_type
{
    return map_.max_size();
}


template <typename K, typename V, typename H, typename P, typename A>
bool lru_cache<K, V, H, P, A>::empty() const noexcept
{
    return map_.empty();
}


//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::begin() -> iterator
//{
//    using pair_type = typename map_type::value_type;
//    return iterator(map_.begin(), [](pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}
//
//
//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::begin() const -> const_iterator
//{
//    using pair_type = typename map_type::value_type;
//    return const_iterator(map_.begin(), [](const pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}
//
//
//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::cbegin() const -> const_iterator
//{
//    using pair_type = typename map_type::value_type;
//    return const_iterator(map_.begin(), [](const pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}
//
//
//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::end() -> iterator
//{
//    using pair_type = typename map_type::value_type;
//    return iterator(map_.end(), [](pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}
//
//
//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::end() const -> const_iterator
//{
//    using pair_type = typename map_type::value_type;
//    return const_iterator(map_.end(), [](const pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}
//
//
//template <typename K, typename V, typename H, typename P, typename A>
//auto lru_cache<K, V, H, P, A>::cend() const -> const_iterator
//{
//    using pair_type = typename map_type::value_type;
//    return const_iterator(map_.end(), [](const pair_type& p)
//    {
//        return std::make_pair(p.first, *p.second);
//    });
//}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::at(const key_type& key) -> mapped_type&
{
    return *map_.at(key);
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::at(const key_type& key) const -> const mapped_type&
{
    return *map_.at(key);
}


PYCPP_END_NAMESPACE
