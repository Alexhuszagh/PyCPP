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

template <typename it>
using iterator_value_type = typename std::iterator_traits<it>::value_type;

template <typename it>
using lru_iterator_transform = typename iterator_value_type<it>::second_type&;

template <typename it>
using lru_cache_transform = std::function<lru_iterator_transform<it>(iterator_value_type<it>&)>;

template <typename it>
using lru_iterator = transform_iterator<it, lru_cache_transform<it>>;

template <typename it>
using lru_iterator_const_transform = const typename iterator_value_type<it>::second_type&;

template <typename it>
using lru_cache_const_transform = std::function<lru_iterator_const_transform<it>(const iterator_value_type<it>&)>;

template <typename it>
using lru_const_iterator = transform_iterator<it, lru_cache_const_transform<it>>;

template <typename lru>
using lru_list = std::list<typename lru::value_type, typename lru::allocator_type>;

template <typename lru>
using lru_map = std::unordered_map<typename lru::key_type, typename lru_list<lru>::iterator, typename lru::hasher, typename lru::key_equal>;

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
    using self = lru_cache<Key, Value, Hash, Pred, Alloc>;
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = detail::lru_iterator<typename detail::lru_list<self>::iterator>;
    using const_iterator = detail::lru_const_iterator<typename detail::lru_list<self>::iterator>;

    // MEMBER FUNCTIONS
    // ----------------
    lru_cache(int cache_size);

    // CAPACITY
    size_type size() const;
    size_type cache_size() const;
    size_type max_size() const;
    bool empty() const noexcept;

    // ITERATORS
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    // ELEMENT ACCESS
    mapped_type& operator[](const key_type&);
    mapped_type& operator[](key_type&&);
    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;

protected:
    using list_t = detail::lru_list<self>;
    using list_iterator_t = typename list_t::iterator;
    using map_t = detail::lru_map<self>;

    void clean();
    list_iterator_t put(const key_type&, const mapped_type&);
    list_iterator_t put(const key_type&, mapped_type&&);
    mapped_type& get(list_iterator_t);
    const mapped_type& get(list_iterator_t) const;

    mutable list_t list_;
    map_t map_;
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


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::begin() -> iterator
{
    return iterator(list_.begin(), [](value_type& p) -> mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::begin() const -> const_iterator
{
    return const_iterator(list_.begin(), [](const value_type& p) -> const mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::cbegin() const -> const_iterator
{
    return const_iterator(list_.begin(), [](const value_type& p) -> const mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::end() -> iterator
{
    return iterator(list_.end(), [](value_type& p) -> mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::end() const -> const_iterator
{
    return const_iterator(list_.end(), [](const value_type& p) -> const mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::cend() const -> const_iterator
{
    return const_iterator(list_.end(), [](const value_type& p) -> const mapped_type&
    {
        return p.second;
    });
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::operator[](const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return put(key, value_type())->second;
    }

    return get(it->second);
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::operator[](key_type&& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return put(key, value_type())->second;
    }

    return get(it->second);
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::at(const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        throw std::out_of_range("lru_cache::at():: Key not found.");
    }

    return get(it->second);
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::at(const key_type& key) const -> const mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        throw std::out_of_range("lru_cache::at():: Key not found.");
    }

    return get(it->second);
}


template <typename K, typename V, typename H, typename P, typename A>
void lru_cache<K, V, H, P, A>::clean()
{
    while(map_.size() > cache_size()) {
        map_.erase(list_.back());
        list_.pop_back();
    }
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::put(const key_type& key, const mapped_type& value) -> list_iterator_t
{
    list_.push_front(std::make_pair(key, value));
    map_.insert(std::make_pair(key, list_.begin()));
    clean();

    return list_.begin();
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::put(const key_type& key, mapped_type&& value) -> list_iterator_t
{
    list_.push_front(std::make_pair(key, std::forward<mapped_type>(value)));
    map_.insert(std::make_pair(key, list_.begin()));
    clean();

    return list_.begin();
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::get(list_iterator_t it) -> mapped_type&
{
    list_.splice(list_.begin(), list_, it);
    return it->second;
}


template <typename K, typename V, typename H, typename P, typename A>
auto lru_cache<K, V, H, P, A>::get(list_iterator_t it) const -> const mapped_type&
{
    list_.splice(list_.begin(), list_, it);
    return it->second;
}


PYCPP_END_NAMESPACE
