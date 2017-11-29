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

#include <pycpp/reference/core.h>
#include <pycpp/sfinae/is_pair.h>
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <unordered_map>
#include <vector>

PYCPP_BEGIN_NAMESPACE

namespace counter_detail
{
// SFINAE
// ------

template <typename T>
using iterator_value_type = typename std::iterator_traits<T>::value_type;

template <typename T>
using is_pair_iterator = is_pair<iterator_value_type<T>>;

template <typename T, typename Iter>
using enable_if_pair = std::enable_if<is_pair_iterator<T>::value, T>;

template <typename T, typename Iter>
using enable_if_not_pair = std::enable_if<!is_pair_iterator<T>::value, T>;

// FUNCTIONS
// ---------

template <typename Map, typename Iter>
typename enable_if_pair<Iter, void>::type
update(Map& map, Iter first, Iter last)
{
    // update mapping from a key-value store
    for (; first != last; ++first) {
        map[first->first] += first->second;
    }
}

template <typename Map, typename Iter>
typename enable_if_not_pair<Iter, void>::type
update(Map& map, Iter first, Iter last)
{
    // update mapping from list of keys
    for (; first != last; ++first) {
        map[*first]++;
    }
}

}   /* counter_detail */

// ALIAS
// -----

using count_t = std::ptrdiff_t;

// DECLARATION
// -----------

template <
    typename Key,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, count_t>>
>
struct counter
{
public:
    // MEMBER TYPES
    // ------------
    using self = counter<Key, Hash, Pred, Alloc>;
    using key_type = Key;
    using mapped_type = count_t;
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
    using iterator = typename std::unordered_map<Key, count_t, Hash, Pred, Alloc>::iterator;
    using const_iterator = typename std::unordered_map<Key, count_t, Hash, Pred, Alloc>::const_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    counter();
    counter(size_type);
    counter(const self&);
    self& operator=(const self&);
    counter(self&&);
    self& operator=(self&&);
    template <typename Iter> counter(Iter, Iter);
    template <typename Iter> counter(Iter, Iter, size_type);
    counter(std::initializer_list<key_type>);
    counter(std::initializer_list<key_type>, size_type);

    // CAPACITY
    size_type size() const;
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

    // MODIFIERS
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
    self& operator*=(const self&);
    self operator*(const self&) const;
    self& operator*=(count_t);
    self operator*(count_t) const;
    self& operator/=(const self&);
    self operator/(const self&) const;
    self& operator/=(count_t);
    self operator/(count_t) const;
    self& operator|=(const self&);
    self operator|(const self&) const;
    self& operator&=(const self&);
    self operator&(const self&) const;

    // CONVENIENCE
    std::vector<value_type> most_common(size_t n = -1) const;
    std::vector<key_type> elements() const;

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
    allocator_type get_allocator() const;

protected:
    using map_t = std::unordered_map<Key, count_t, Hash, Pred, Alloc>;
    map_t map_;
};

// IMPLEMENTATION
// --------------


template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter()
{}


template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter(size_type n):
    map_(n)
{}


template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter(const self& rhs):
    map_(rhs.map_)
{}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator=(const self& rhs) -> self&
{
    map_ = rhs.map_;
    return *this;
}


template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter(self&& rhs)
{
    swap(rhs);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A>
template <typename Iter>
counter<K, H, P, A>::counter(Iter first, Iter last)
{
    update(first, last);
}


template <typename K, typename H, typename P, typename A>
template <typename Iter>
counter<K, H, P, A>::counter(Iter first, Iter last, size_type n):
    map_(n)
{
    update(first, last);
}


template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter(std::initializer_list<key_type> list)
{
    update(list.begin(), list.end());
}

template <typename K, typename H, typename P, typename A>
counter<K, H, P, A>::counter(std::initializer_list<key_type> list, size_type n):
    map_(n)
{
    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::size() const -> size_type
{
    return map_.size();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::max_size() const -> size_type
{
    return map_.max_size();
}


template <typename K, typename H, typename P, typename A>
bool counter<K, H, P, A>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::begin() -> iterator
{
    return map_.begin();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::begin() const -> const_iterator
{
    return map_.begin();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::cbegin() const -> const_iterator
{
    return map_.cbegin();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::end() -> iterator
{
    return map_.end();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::end() const -> const_iterator
{
    return map_.end();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::cend() const -> const_iterator
{
    return map_.cend();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator[](const key_type& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        it = map_.emplace_hint(it, key, 0);
    }

    return it->second;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator[](key_type&& key) -> mapped_type&
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        it = map_.emplace_hint(it, std::forward<key_type>(key), 0);
    }

    return it->second;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::at(const key_type& key) -> mapped_type&
{
    return map_.at(key);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::at(const key_type& key) const -> const mapped_type&
{
    return map_.at(key);
}

template <typename K, typename H, typename P, typename A>
template <typename Iter>
void counter<K, H, P, A>::update(Iter first, Iter last)
{
    counter_detail::update(map_, first, last);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::erase(const key_type& key) -> size_type
{
    return map_.erase(key);
}


template <typename K, typename H, typename P, typename A>
void counter<K, H, P, A>::clear()
{
    map_.clear();
}


template <typename K, typename H, typename P, typename A>
void counter<K, H, P, A>::swap(self& rhs)
{
    std::swap(map_, rhs.map_);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator+=(const self& rhs) -> self&
{
    for (const auto& pair: rhs) {
        map_[pair.first] += pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator+(const self& rhs) const -> self
{
    self copy(*this);
    copy += rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator+=(count_t rhs) -> self&
{
    for (const auto& pair: map_) {
        pair.second += rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator+(count_t rhs) const -> self
{
    self copy(*this);
    copy += rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator-=(const self& rhs) -> self&
{
    for (const auto& pair: rhs) {
        map_[pair.first] -= pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator-(const self& rhs) const -> self
{
    self copy(*this);
    copy -= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator-=(count_t rhs) -> self&
{
    for (const auto& pair: map_) {
        pair.second -= rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator-(count_t rhs) const -> self
{
    self copy(*this);
    copy -= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator*=(const self& rhs) -> self&
{
    for (const auto& pair: rhs) {
        map_[pair.first] *= pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator*(const self& rhs)const -> self
{
    self copy(*this);
    copy *= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator*=(count_t rhs) -> self&
{
    for (const auto& pair: map_) {
        pair.second *= rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator*(count_t rhs) const -> self
{
    self copy(*this);
    copy *= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator/=(const self& rhs) -> self&
{
    for (const auto& pair: rhs) {
        map_[pair.first] /= pair.second;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator/(const self& rhs) const -> self
{
    self copy(*this);
    copy /= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator/=(count_t rhs) -> self&
{
    for (const auto& pair: map_) {
        pair.second /= rhs;
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator/(count_t rhs) const -> self
{
    self copy(*this);
    copy /= rhs;
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator|=(const self& rhs) -> self&
{
    operator=(*this | rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator|(const self& rhs) const -> self
{
    self copy;
    for (const value_type& pair: rhs.map_) {
        auto it = map_.find(pair.first);
        if (it != map_.end()) {
            copy.map_[pair.first] = std::min(it->second, pair.second);
        }
    }
    return copy;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator&=(const self& rhs) -> self&
{
    for (const value_type& pair: rhs.map_) {
        count_t& value = map_[pair.first];
        value = std::max(value, pair.second);
    }
    return *this;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::operator&(const self& rhs) const -> self
{
    self copy(*this);
    copy &= rhs;
    return copy;
}


// TODO: insert here


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::most_common(size_t n) const -> std::vector<value_type>
{
    // create values
    std::vector<value_type> values;
    values.reserve(size());
    for (const auto& pair: map_) {
        values.emplace_back(pair);
    }

    // sort in descending order
    std::sort(values.begin(), values.end(), [](const value_type& lhs, const value_type& rhs) {
        return lhs.second > rhs.second;
    });

    // trim
    values.resize(n);

    return values;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::elements() const -> std::vector<key_type>
{
    // create values
    size_t count = std::accumulate(begin(), end(), 0, [](size_t l, count_t r) {
        return r > 0 ? l + r : l;
    });
    std::vector<value_type> values;
    values.reserve(count);

    // add items
    for (const auto& pair: map_) {
        count_t i = pair.second;
        while (i-- > 0) {
            values.emplace_back(pair.first);
        }
    }

    return values;
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::bucket_count() const noexcept -> size_type
{
    return map_.bucket_count();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::max_bucket_count() const noexcept -> size_type
{
    return map_.max_bucket_count();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::bucket_size(size_type n) const -> size_type
{
    return map_.bucket_size(n);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::bucket(const key_type& key) const -> size_type
{
    return map_.bucket(key);
}


template <typename K, typename H, typename P, typename A>
float counter<K, H, P, A>::load_factor() const noexcept
{
    return map_.load_factor();
}


template <typename K, typename H, typename P, typename A>
float counter<K, H, P, A>::max_load_factor() const noexcept
{
    return map_.max_load_factor();
}


template <typename K, typename H, typename P, typename A>
void counter<K, H, P, A>::max_load_factor(float n)
{
    map_.max_load_factor(n);
}


template <typename K, typename H, typename P, typename A>
void counter<K, H, P, A>::rehash(size_type n)
{
    map_.rehash(n);
}


template <typename K, typename H, typename P, typename A>
void counter<K, H, P, A>::reserve(size_type n)
{
    map_.reserve(n);
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::hash_function() const -> hasher
{
    return hasher();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::key_eq() const -> key_equal
{
    return key_equal();
}


template <typename K, typename H, typename P, typename A>
auto counter<K, H, P, A>::get_allocator() const -> allocator_type
{
    return allocator_type();
}

PYCPP_END_NAMESPACE
