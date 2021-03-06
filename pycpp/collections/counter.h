//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Counter implementation for C++.
 *
 *  Key counter analagous to Python's collections.Counter()
 *  class. Counts hashable objects, and provides methods to
 *  return the most frequently occuring keys.
 *
 *  Unlike Python's implementation, subtraction will not
 *  only keep positive counts. It supports positive and
 *  negative counts, and will not artificially remove
 *  any objects unless explicitly requested.
 */

#pragma once

#include <pycpp/collections/counter_fwd.h>
#include <pycpp/sfinae/is_pair.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/numeric.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/unordered_set.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

namespace counter_detail
{
// SFINAE
// ------

template <typename T>
using iterator_value_type = typename iterator_traits<T>::value_type;

template <typename T>
using is_pair_iterator = is_pair<iterator_value_type<T>>;

template <typename T, typename Iter>
using enable_if_pair_t = enable_if_t<is_pair_iterator<Iter>::value, T>;

template <typename T, typename Iter>
using enable_if_not_pair_t = enable_if_t<!is_pair_iterator<Iter>::value, T>;

template <typename allocator_type, typename T>
using rebind_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<T>;

template <typename Map>
using mutable_pair_type = pair<typename Map::key_type, typename Map::mapped_type>;

template <typename Map>
using mutable_pair_list = vector<
    mutable_pair_type<Map>,
    rebind_allocator<typename Map::allocator_type, mutable_pair_type<Map>>
>;

template <typename Map>
using pair_list = vector<
    typename Map::value_type,
    rebind_allocator<typename Map::allocator_type, typename Map::value_type>
>;

template <typename Map>
using key_list = vector<
    typename Map::key_type,
    rebind_allocator<typename Map::allocator_type, typename Map::key_type>
>;

template <typename Map>
using memo_value_type = reference_wrapper<const typename Map::key_type>;

template <typename Map>
using memo_type = unordered_set<
    memo_value_type<Map>,
    typename Map::hasher,
    typename Map::key_equal,
    rebind_allocator<typename Map::allocator_type, memo_value_type<Map>>
>;

// FUNCTIONS
// ---------

template <typename Map, typename Iter>
enable_if_pair_t<void, Iter>
update(Map& map, Iter first, Iter last)
{
    // update mapping from a key-value store
    for (; first != last; ++first) {
        map[first->first] += first->second;
    }
}


template <typename Map, typename Iter>
enable_if_not_pair_t<void, Iter>
update(Map& map, Iter first, Iter last)
{
    // update mapping from list of keys
    for (; first != last; ++first) {
        map[*first]++;
    }
}


template <typename Map>
mutable_pair_list<Map> most_common(const Map& map, size_t n)
{
    using list_type = mutable_pair_list<Map>;
    using value_type = typename mutable_pair_list<Map>::value_type;

    // create values
    list_type values;
    values.reserve(map.size());
    for (const auto& pair: map) {
        values.emplace_back(make_pair(pair.first, pair.second));
    }

    // sort in descending order
    sort(values.begin(), values.end(), [](const value_type& lhs, const value_type& rhs) {
        return lhs.second > rhs.second;
    });

    // trim
    if (n > 0 && n < values.size()) {
        values.resize(n);
    }

    return values;
}


template <typename Map>
key_list<Map> elements(const Map& map)
{
    using list_type = key_list<Map>;
    using value_type = typename Map::value_type;

    // create values
    size_t count = accumulate(map.begin(), map.end(), size_t(0), [](size_t l, const value_type& rhs) {
        count_t r = rhs.second;
        return r > 0 ? l + static_cast<size_t>(r) : l;
    });
    list_type values;
    values.reserve(count);

    // add items
    for (const auto& pair: map) {
        count_t i = pair.second;
        while (i-- > 0) {
            values.emplace_back(pair.first);
        }
    }

    return values;
}

}   /* counter_detail */

// DECLARATION
// -----------

template <
    typename Key,
    typename Hash,
    typename Pred,
    typename Alloc,
    template <typename, typename, typename, typename, typename> class Map
>
struct counter
{
public:
    // MEMBER TYPES
    // ------------
    using self = counter<Key, Hash, Pred, Alloc>;
    using map_type = Map<Key, count_t, Hash, Pred, Alloc>;
    using key_type = Key;
    using mapped_type = count_t;
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
    using iterator = typename map_type::iterator;
    using const_iterator = typename map_type::const_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    counter(const allocator_type& alloc = allocator_type());
    counter(size_type, const allocator_type& alloc = allocator_type());
    counter(const self&, const allocator_type& alloc = allocator_type());
    self& operator=(const self&);
    counter(self&&, const allocator_type& alloc = allocator_type());
    self& operator=(self&&);
    counter(const map_type&, const allocator_type& alloc = allocator_type());
    self& operator=(const map_type&);
    counter(map_type&&, const allocator_type& alloc = allocator_type());
    self& operator=(map_type&&);
    template <typename Iter> counter(Iter, Iter, const allocator_type& alloc = allocator_type());
    template <typename Iter> counter(Iter, Iter, size_type, const allocator_type& alloc = allocator_type());
    counter(initializer_list<value_type>, const allocator_type& alloc = allocator_type());
    counter(initializer_list<value_type>, size_type, const allocator_type& alloc = allocator_type());
    counter(initializer_list<key_type>, const allocator_type& alloc = allocator_type());
    counter(initializer_list<key_type>, size_type, const allocator_type& alloc = allocator_type());

    // CAPACITY
    size_type size() const noexcept;
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
    count_t get(const key_type&, count_t = 0) const;

    // MODIFIERS
    void add(const key_type&);
    void add(key_type&&);
    template <typename Iter> void update(Iter, Iter);
    size_type erase(const key_type&);
    void clear();
    void swap(self&);

    // OPERATORS
    self& operator+=(const self&);
    self operator+(const self&) const;
    self& operator+=(count_t);
    self operator+(count_t) const;
    self& operator-=(const self&);
    self operator-(const self&) const;
    self& operator-=(count_t);
    self operator-(count_t) const;
    self& operator|=(const self&);
    self operator|(const self&) const;
    self& operator&=(const self&);
    self operator&(const self&) const;

    // CONVENIENCE
    counter_detail::mutable_pair_list<self> most_common(size_t n = -1) const;
    counter_detail::key_list<self> elements() const;

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

    // CONVERSION
    explicit operator map_type() const;

protected:
    friend struct threshold_counter<Key, Hash, Pred, Alloc, Map>;

    map_type map_;
};

// SPECIALIZATION
// --------------

template <
    typename Key,
    typename Hash,
    typename Pred,
    typename Alloc,
    template <typename, typename, typename, typename, typename> class Map
>
struct is_relocatable<counter<Key, Hash, Pred, Alloc, Map>>:
    is_relocatable<typename counter<Key, Hash, Pred, Alloc, Map>::map_type>
{};

// IMPLEMENTATION
// --------------


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(const allocator_type& alloc):
    map_(alloc)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(size_type n, const allocator_type& alloc):
    map_(n, alloc)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(const self& rhs, const allocator_type& alloc):
    map_(rhs.map_, alloc)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::operator=(const self& rhs) -> self&
{
    map_ = rhs.map_;
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(self&& rhs, const allocator_type& alloc):
    map_(alloc)
{
    swap(rhs);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(const map_type& rhs, const allocator_type& alloc):
    map_(rhs, alloc)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::operator=(const map_type& rhs) -> self&
{
    map_ = rhs;
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(map_type&& rhs, const allocator_type& alloc):
    map_(move(rhs), alloc)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::operator=(map_type&& rhs) -> self&
{
    map_ = move(rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
template <typename Iter>
inline counter<K, H, P, A, M>::counter(Iter first, Iter last, const allocator_type& alloc):
    map_(alloc)
{
    update(first, last);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
template <typename Iter>
inline counter<K, H, P, A, M>::counter(Iter first, Iter last, size_type n, const allocator_type& alloc):
    map_(n, alloc)
{
    update(first, last);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(initializer_list<value_type> list, const allocator_type& alloc):
    map_(alloc)
{
    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(initializer_list<value_type> list, size_type n, const allocator_type& alloc):
    map_(n, alloc)
{
    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(initializer_list<key_type> list, const allocator_type& alloc):
    map_(alloc)
{
    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::counter(initializer_list<key_type> list, size_type n, const allocator_type& alloc):
    map_(n, alloc)
{
    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::size() const noexcept -> size_type
{
    return map_.size();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::max_size() const noexcept -> size_type
{
    return map_.max_size();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline bool counter<K, H, P, A, M>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::begin() noexcept -> iterator
{
    return map_.begin();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::begin() const noexcept -> const_iterator
{
    return map_.begin();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::cbegin() const noexcept -> const_iterator
{
    return map_.cbegin();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::end() noexcept -> iterator
{
    return map_.end();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::end() const noexcept -> const_iterator
{
    return map_.end();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::cend() const noexcept -> const_iterator
{
    return map_.cend();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator[](const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        it = map_.emplace_hint(it, key, 0);
    }

    return it->second;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator[](key_type&& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        it = map_.emplace_hint(it, forward<key_type>(key), 0);
    }

    return it->second;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::at(const key_type& key) -> mapped_type&
{
    return map_.at(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::at(const key_type& key) const -> const mapped_type&
{
    return map_.at(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
count_t counter<K, H, P, A, M>::get(const key_type& key, count_t n) const
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return n;
    }
    return it->second;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::add(const key_type& key)
{
    map_[key]++;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::add(key_type&& key)
{
    map_[forward<key_type>(key)]++;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
template <typename Iter>
inline void counter<K, H, P, A, M>::update(Iter first, Iter last)
{
    counter_detail::update(map_, first, last);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::erase(const key_type& key) -> size_type
{
    return map_.erase(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::clear()
{
    map_.clear();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::swap(self& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(map_, rhs.map_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator+=(const self& rhs) -> self&
{
    for (const value_type& pair: rhs) {
        map_[pair.first] += pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator+(const self& rhs) const -> self
{
    self copy(*this);
    copy += rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator+=(count_t rhs) -> self&
{
    for (value_type& pair: map_) {
        pair.second += rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator+(count_t rhs) const -> self
{
    self copy(*this);
    copy += rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator-=(const self& rhs) -> self&
{
    for (const value_type& pair: rhs) {
        map_[pair.first] -= pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator-(const self& rhs) const -> self
{
    self copy(*this);
    copy -= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator-=(count_t rhs) -> self&
{
    for (value_type& pair: map_) {
        pair.second -= rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator-(count_t rhs) const -> self
{
    self copy(*this);
    copy -= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator|=(const self& rhs) -> self&
{
    // get key list
    counter_detail::memo_type<self> s;
    for (const value_type& pair: map_) {
        s.emplace(pair.first);
    }
    for (const value_type& pair: rhs.map_) {
        s.emplace(pair.first);
    }

    // get maximum value over all keys
    for (const key_type& key: s) {
        count_t& value = map_[key];
        value = max(value, rhs.get(key, 0));
    }
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator|(const self& rhs) const -> self
{
    self copy(*this);
    copy |= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::operator&=(const self& rhs) -> self&
{
    operator=(*this & rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto counter<K, H, P, A, M>::operator&(const self& rhs) const -> self
{
    self copy;

    // get key list
    counter_detail::memo_type<self> s;
    for (const value_type& pair: map_) {
        if (rhs.map_.find(pair.first) != rhs.map_.end()) {
            s.emplace(pair.first);
        }
    }

    // get minimum value over all keys
    for (const key_type& key: s) {
        copy.map_[key] = min(map_.at(key), rhs.map_.at(key));
    }

    return copy;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::most_common(size_t n) const -> counter_detail::mutable_pair_list<self>
{
    return counter_detail::most_common(map_, n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::elements() const -> counter_detail::key_list<self>
{
    return counter_detail::elements(map_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::bucket_count() const noexcept -> size_type
{
    return map_.bucket_count();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::max_bucket_count() const noexcept -> size_type
{
    return map_.max_bucket_count();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::bucket_size(size_type n) const -> size_type
{
    return map_.bucket_size(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::bucket(const key_type& key) const -> size_type
{
    return map_.bucket(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline float counter<K, H, P, A, M>::load_factor() const noexcept
{
    return map_.load_factor();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline float counter<K, H, P, A, M>::max_load_factor() const noexcept
{
    return map_.max_load_factor();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::max_load_factor(float n)
{
    map_.max_load_factor(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::rehash(size_type n)
{
    map_.rehash(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void counter<K, H, P, A, M>::reserve(size_type n)
{
    map_.reserve(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::hash_function() const -> hasher
{
    return hasher();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::key_eq() const -> key_equal
{
    return key_equal();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto counter<K, H, P, A, M>::get_allocator() const noexcept -> allocator_type
{
    return map_.get_allocator();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline counter<K, H, P, A, M>::operator map_type() const
{
    return map_;
}

PYCPP_END_NAMESPACE
