//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Least-recently inserted cache.
 *
 *  Low-overhead FIFO-like cache. Remembers the last N elements
 *  inserted into the queue. The queue does not update when
 *  the value is updated for a given key-value. The underlying
 *  implmentation is similar to the LRU cache, and suffers
 *  the same optimization issues.
 */

#pragma once

#include <pycpp/cache/lru.h>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Macro wrapper to automate iterator construction.
 */
#define LRI_ITERATOR(it)                                                \
    iterator(it, [](value_type& p) -> mapped_type&                      \
    {                                                                   \
        return p.second;                                                \
    })

/**
 *  \brief Macro wrapper to automate const_iterator construction.
 */
#define LRI_CONST_ITERATOR(it)                                          \
    const_iterator(it, [](const value_type& p) -> const mapped_type&    \
    {                                                                   \
        return p.second;                                                \
    })

// DECLARATION
// -----------

/**
 *  \brief O(1) LRI cache implemented via a hashtable and linked list.
 */
template <
    typename Key,
    typename Value,
    typename Hash = hash<Key>,
    typename Pred = equal_to<Key>,
    typename Alloc = allocator<pair<const Key, Value>>,
    template <typename, typename> class List = list,
    template <typename, typename, typename, typename, typename> class Map = unordered_map
>
struct lri_cache
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = lri_cache<Key, Value, Hash, Pred, Alloc, List, Map>;
    using key_type = Key;
    using mapped_type = Value;
    using value_type = pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using list_type = List<value_type, allocator_type>;
    using map_type = lru_detail::map<self_t, Map>;
    using iterator = lru_detail::iterator<typename list_type::iterator>;
    using const_iterator = lru_detail::const_iterator<typename list_type::iterator>;

    // MEMBER FUNCTIONS
    // ----------------
    lri_cache(int cache_size = 128, const allocator_type& alloc = allocator_type());
    lri_cache(const self_t&, const allocator_type& alloc = allocator_type());
    self_t& operator=(const self_t&);
    lri_cache(self_t&&, const allocator_type& alloc = allocator_type());
    self_t& operator=(self_t&&);

    // CAPACITY
    size_type size() const noexcept;
    size_type cache_size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;

    // ITERATORS
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // ELEMENT ACCESS
    mapped_type& operator[](const key_type&);
    mapped_type& operator[](key_type&&);
    mapped_type& at(const key_type&);
    const mapped_type& at(const key_type&) const;

    // ELEMENT LOOKUP
    iterator find(const key_type&);
    const_iterator find(const key_type&) const;
    size_type count(const key_type&) const;
    pair<iterator, iterator> equal_range(const key_type&);
    pair<const_iterator, const_iterator> equal_range(const key_type&) const;

    // MODIFIERS
    pair<iterator, bool> insert(const key_type&, const mapped_type&);
    pair<iterator, bool> insert(const key_type&, mapped_type&&);
    pair<iterator, bool> insert(key_type&&, mapped_type&&);
    iterator erase(const_iterator);
    size_type erase(const key_type&);
    iterator erase(const_iterator, const_iterator);
    void clear();
    void swap(self_t&);

    // BUCKET
    size_type bucket_count() const noexcept;
    size_type max_bucket_count() const noexcept;
    size_type bucket_size(size_type) const;
    size_type bucket(const key_type&) const;

    // HASH POLICY
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float);
    void rehash(size_type);
    void reserve(size_type);

    // OBSERVERS
    hasher hash_function() const;
    key_equal key_eq() const;
    allocator_type get_allocator() const noexcept;

protected:
    // CACHE
    void clean();
    iterator pop(const_iterator);
    iterator put(const key_type&, const mapped_type&);
    iterator put(const key_type&, mapped_type&&);
    iterator put(key_type&&, mapped_type&&);
    iterator get(iterator);
    const_iterator get(const_iterator) const;

    mutable list_type list_;
    map_type map_;
    size_type cache_size_;
};

// SPECIALIZATION
// --------------

template <
    typename Key,
    typename Value,
    typename Hash,
    typename Pred,
    typename Alloc,
    template <typename, typename> class List,
    template <typename, typename, typename, typename, typename> class Map
>
struct is_relocatable<lri_cache<Key, Value, Hash, Pred, Alloc, List, Map>>: bool_constant<
        is_relocatable<typename lri_cache<Key, Value, Hash, Pred, Alloc, List, Map>::list_type>::value &&
        is_relocatable<typename lri_cache<Key, Value, Hash, Pred, Alloc, List, Map>::map_type>::value
    >
{};

// IMPLEMENTATION
// --------------


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
lri_cache<K, V, H, P, A, L, M>::lri_cache(int cache_size, const allocator_type& alloc):
    list_(alloc),
    map_(alloc),
    cache_size_(cache_size)
{}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
lri_cache<K, V, H, P, A, L, M>::lri_cache(const self_t& rhs, const allocator_type& alloc):
    list_(rhs.list_, alloc),
    map_(alloc),
    cache_size_(rhs.cache_size_)
{
    for (auto it = list_.begin(); it != list_.end(); ++it) {
        map_.emplace(make_pair(cref(it->first), it));
    }
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        clear();

        cache_size_ = rhs.cache_size_;
        list_ = list_type(rhs.list_);
        for (auto it = list_.begin(); it != list_.end(); ++it) {
            map_.emplace(make_pair(cref(it->first), it));
        }
    }

    return *this;
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
lri_cache<K, V, H, P, A, L, M>::lri_cache(self_t&& rhs, const allocator_type& alloc):
    list_(alloc),
    map_(alloc)
{
    swap(rhs);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::operator=(self_t&& rhs) -> self_t&
{
    swap(rhs);
    return *this;
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::cache_size() const noexcept -> size_type
{
    return cache_size_;
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::size() const noexcept -> size_type
{
    return map_.size();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::max_size() const noexcept -> size_type
{
    return map_.max_size();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
bool lri_cache<K, V, H, P, A, L, M>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::begin() noexcept -> iterator
{
    return LRI_ITERATOR(list_.begin());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::begin() const noexcept -> const_iterator
{
    return LRI_CONST_ITERATOR(list_.begin());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::cbegin() const noexcept -> const_iterator
{
    return LRI_CONST_ITERATOR(list_.begin());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::end() noexcept -> iterator
{
    return LRI_ITERATOR(list_.end());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::end() const noexcept -> const_iterator
{
    return LRI_CONST_ITERATOR(list_.end());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::cend() const noexcept -> const_iterator
{
    return LRI_CONST_ITERATOR(list_.end());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::operator[](const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return *put(key, mapped_type());
    }

    return *get(LRI_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::operator[](key_type&& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return *put(forward<key_type>(key), mapped_type());
    }

    return *get(LRI_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::at(const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        throw out_of_range("lri_cache::at():: Key not found.");
    }

    return *get(LRI_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::at(const key_type& key) const -> const mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        throw out_of_range("lri_cache::at():: Key not found.");
    }

    return *get(LRI_CONST_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::find(const key_type& key) -> iterator
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return end();
    }

    return get(LRI_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::find(const key_type& key) const -> const_iterator
{
    auto it = map_.find(key);
    if (it == map_.cend()) {
        return cend();
    }

    return get(LRI_CONST_ITERATOR(it->second));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::count(const key_type& key) const -> size_type
{
    return map_.count(key);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::equal_range(const key_type& key) -> pair<iterator, iterator>
{
    auto pair = map_.equal_range(key);
    if (pair.first == map_.end()) {
        return make_pair(end(), end());
    } else if (pair.second == map_.end()) {
        return make_pair(get(LRI_ITERATOR(pair.first->second)), end());
    } else {
        return make_pair(get(LRI_ITERATOR(pair.first->second)), get(LRI_ITERATOR(pair.second->second)));
    }
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::equal_range(const key_type& key) const -> pair<const_iterator, const_iterator>
{
    auto pair = map_.equal_range(key);
    if (pair.first == map_.cend()) {
        return make_pair(cend(), cend());
    } else if (pair.second == map_.cend()) {
        return make_pair(get(LRI_CONST_ITERATOR(pair.first->second)), cend());
    } else {
        return make_pair(get(LRI_CONST_ITERATOR(pair.first->second)), get(LRI_CONST_ITERATOR(pair.second->second)));
    }
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::insert(const key_type& key, const mapped_type& value) -> pair<iterator, bool>
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return make_pair(put(key, value), true);
    }

    return make_pair(LRI_ITERATOR(it->second), false);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::insert(const key_type& key, mapped_type&& value) -> pair<iterator, bool>
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return make_pair(put(key, forward<mapped_type>(value)), true);
    }

    return make_pair(LRI_ITERATOR(it->second), false);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::insert(key_type&& key, mapped_type&& value) -> pair<iterator, bool>
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return make_pair(put(forward<key_type>(key), forward<mapped_type>(value)), true);
    }

    return make_pair(LRI_ITERATOR(it->second), false);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::erase(const_iterator pos) -> iterator
{
    return pop(pos);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::erase(const key_type& key) -> size_type
{
    auto it = map_.find(key);
    if (it == map_.cend()) {
        return 0;
    }
    erase(LRI_CONST_ITERATOR(it->second));
    return 1;
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::erase(const_iterator first, const_iterator last) -> iterator
{
    for (; first != last; ) {
        first = LRI_CONST_ITERATOR(erase(first).base());
    }
    return LRI_ITERATOR(last.base());
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::clear()
{
    map_.clear();
    list_.clear();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::swap(self_t& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(list_, rhs.list_);
    swap(map_, rhs.map_);
    swap(cache_size_, rhs.cache_size_);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::bucket_count() const noexcept -> size_type
{
    return map_.bucket_count();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::max_bucket_count() const noexcept -> size_type
{
    return map_.max_bucket_count();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::bucket_size(size_type n) const -> size_type
{
    return map_.bucket_size(n);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::bucket(const key_type& key) const -> size_type
{
    return map_.bucket(key);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
float lri_cache<K, V, H, P, A, L, M>::load_factor() const noexcept
{
    return map_.load_factor();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
float lri_cache<K, V, H, P, A, L, M>::max_load_factor() const noexcept
{
    return map_.max_load_factor();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::max_load_factor(float n)
{
    map_.max_load_factor(n);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::rehash(size_type n)
{
    map_.rehash(n);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::reserve(size_type n)
{
    map_.reserve(n);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::hash_function() const -> hasher
{
    return hasher();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::key_eq() const -> key_equal
{
    return key_equal();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::get_allocator() const noexcept -> allocator_type
{
    return map_.get_allocator();
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
void lri_cache<K, V, H, P, A, L, M>::clean()
{
    while(map_.size() > cache_size()) {
        pop(LRI_CONST_ITERATOR(--list_.end()));
    }
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::pop(const_iterator it) -> iterator
{
    map_.erase(it.base()->first);
    return LRI_ITERATOR(list_.erase(it.base()));
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::put(const key_type& key, const mapped_type& value) -> iterator
{
    list_.push_front(make_pair(key, value));
    auto it = list_.begin();
    map_.emplace(make_pair(cref(it->first), it));
    clean();

    return LRI_ITERATOR(it);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::put(const key_type& key, mapped_type&& value) -> iterator
{
    list_.push_front(make_pair(key, forward<mapped_type>(value)));
    auto it = list_.begin();
    map_.emplace(make_pair(cref(it->first), it));
    clean();

    return LRI_ITERATOR(it);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::put(key_type&& key, mapped_type&& value) -> iterator
{
    list_.push_front(make_pair(forward<key_type>(key), forward<mapped_type>(value)));
    auto it = list_.begin();
    map_.emplace(make_pair(cref(it->first), it));
    clean();

    return LRI_ITERATOR(it);
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::get(iterator it) -> iterator
{
    // identity-op, don't re-memoize the item
    return it;
}


template <typename K, typename V, typename H, typename P, typename A, template <typename, typename> class L, template <typename, typename, typename, typename, typename> class M>
auto lri_cache<K, V, H, P, A, L, M>::get(const_iterator it) const -> const_iterator
{
    // identity-op, don't re-memoize the item
    return it;
}

// CLEANUP
// -------

#undef LRI_ITERATOR
#undef LRI_CONST_ITERATOR

PYCPP_END_NAMESPACE
