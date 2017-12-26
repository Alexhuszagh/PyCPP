//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Counter implementation for C++.
 *
 *  Automatically compacting counter that removes items
 *  below a certain count threshold. Counts hashable objects
 *  and provides methods to return the most-frequently occuring
 *  keys. TheAPI is analogous to boltons.cacheutils's
 *  [ThresholdCounter][reference].
 *
 *  Similar to the boltons implementation, this implements the
 *  Lossy Counting algorithm described in “Approximate
 *  Frequency Counts over Data Streams” by Manku & Motwani.
 *
 *  To prevent any issues with auto-compacting, counts
 *  cannot be directly incremented or decremented
 *  manually, however, explicit conversion operators
 *  to and from `counter` are provided.
 *
 *  reference:
 *      https://boltons.readthedocs.io/en/latest/cacheutils.html#boltons.cacheutils.ThresholdCounter
 */

#pragma once

#include <pycpp/collections/counter.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

namespace counter_detail
{
// FUNCTIONS
// ---------

/**
 *  \brief Initialize from a moved external map.
 */
template <typename Map>
void update_from_map(const Map& map, size_t& count)
{
    using value_type = typename Map::value_type;
    count = accumulate(map.begin(), map.end(), size_t(0), [](size_t l, const value_type& rhs) {
        count_t r = rhs.second;
        return r > 0 ? l + static_cast<size_t>(r) : l;
    });
}


template <typename Map, typename Iter>
enable_if_pair_t<size_t, Iter>
update_nonnegative(Map& map, Iter first, Iter last)
{
    // update mapping from a key-value store
    size_t count = 0;
    for (; first != last; ++first) {
        if (first->second > 0){
            map[first->first] += first->second;
            count += first->second;
        }
    }

    return count;
}


template <typename Map, typename Iter>
enable_if_not_pair_t<size_t, Iter>
update_nonnegative(Map& map, Iter first, Iter last)
{
    // update mapping from list of keys
    size_t count = 0;
    for (; first != last; ++first) {
        ++count;
        map[*first]++;
    }

    return count;
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
struct threshold_counter
{
public:
    // MEMBER TYPES
    // ------------
    using self = threshold_counter<Key, Hash, Pred, Alloc>;
    using counter_type = counter<Key, Hash, Pred, Alloc, Map>;
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
    threshold_counter(float threshold = 0.01, const allocator_type& alloc = allocator_type());
    threshold_counter(const self&, const allocator_type& alloc = allocator_type());
    self& operator=(const self&);
    threshold_counter(self&&, const allocator_type& alloc = allocator_type());
    self& operator=(self&&);
    threshold_counter(const counter_type&, float = 0.01, const allocator_type& alloc = allocator_type());
    self& operator=(const counter_type&);
    threshold_counter(counter_type&&, float = 0.01, const allocator_type& alloc = allocator_type());
    self& operator=(counter_type&&);
    threshold_counter(const map_type&, float = 0.01, const allocator_type& alloc = allocator_type());
    self& operator=(const map_type&);
    threshold_counter(map_type&&, float = 0.01, const allocator_type& alloc = allocator_type());
    self& operator=(map_type&&);
    template <typename Iter> threshold_counter(Iter, Iter, float = 0.01, const allocator_type& alloc = allocator_type());
    threshold_counter(initializer_list<value_type>, float = 0.01, const allocator_type& alloc = allocator_type());
    threshold_counter(initializer_list<key_type>, float = 0.01, const allocator_type& alloc = allocator_type());

    // CAPACITY
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;

    // ITERATORS
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // ELEMENT ACCESS
    const mapped_type& at(const key_type&) const;
    mapped_type get(const key_type&, mapped_type = 0) const;

    // MODIFIERS
    void add(const key_type&);
    void add(key_type&&);
    template <typename Iter> void update(Iter, Iter);
    void clear();
    void swap(self&);

    // CONVENIENCE
    counter_detail::mutable_pair_list<self> most_common(size_t n = -1) const;
    counter_detail::key_list<self> elements() const;
    count_t get_common_count() const;
    count_t get_uncommon_count() const;
    double get_commonality() const;
    void autocompact();
    void check_autocompact();

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
    explicit operator counter_type() const;
    explicit operator map_type() const;

protected:
    friend struct counter<Key, Hash, Pred, Alloc, Map>;

    map_type map_;
    size_t interval_;
    size_t count_ = 0;
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
struct is_relocatable<threshold_counter<Key, Hash, Pred, Alloc, Map>>:
    is_relocatable<typename threshold_counter<Key, Hash, Pred, Alloc, Map>::map_type>
{};

// IMPLEMENTATION
// --------------


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(const self& rhs, const allocator_type& alloc):
    map_(rhs.map_, alloc),
    interval_(rhs.interval_),
    count_(rhs.count_)
{}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto threshold_counter<K, H, P, A, M>::operator=(const self& rhs) -> self&
{
    map_ = rhs.map_;
    interval_ = rhs.interval_;
    count_ = rhs.count_;
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline threshold_counter<K, H, P, A, M>::threshold_counter(self&& rhs, const allocator_type& alloc):
    map_(alloc)
{
    swap(rhs);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(const counter_type& rhs, float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    update(rhs.begin(), rhs.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::operator=(const counter_type& rhs) -> self&
{
    update(rhs.begin(), rhs.end());
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(counter_type&& rhs, float threshold, const allocator_type& alloc):
    map_(move(rhs.map_), alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    counter_detail::update_from_map(map_, count_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto threshold_counter<K, H, P, A, M>::operator=(counter_type&& rhs) -> self&
{
    map_ = move(rhs.map_);
    counter_detail::update_from_map(map_, count_);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(const map_type& rhs, float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    update(rhs.begin(), rhs.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::operator=(const map_type& rhs) -> self&
{
    update(rhs.begin(), rhs.end());
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(map_type&& rhs, float threshold, const allocator_type& alloc):
    map_(move(rhs), alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    counter_detail::update_from_map(map_, count_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto threshold_counter<K, H, P, A, M>::operator=(map_type&& rhs) -> self&
{
    map_ = move(rhs);
    counter_detail::update_from_map(map_, count_);
    return *this;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
template <typename Iter>
threshold_counter<K, H, P, A, M>::threshold_counter(Iter first, Iter last, float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    update(first, last);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(initializer_list<value_type> list, float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
threshold_counter<K, H, P, A, M>::threshold_counter(initializer_list<key_type> list, float threshold, const allocator_type& alloc):
    map_(alloc),
    interval_(static_cast<size_t>(1 / threshold))
{
    // check the threshold is meaningful, can be counted
    assert(threshold <= 1 && threshold >= 1./static_cast<float>(SIZE_MAX));

    update(list.begin(), list.end());
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::size() const noexcept -> size_type
{
    return map_.size();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::max_size() const noexcept -> size_type
{
    return map_.max_size();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline bool threshold_counter<K, H, P, A, M>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::begin() const noexcept -> const_iterator
{
    return map_.begin();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::cbegin() const noexcept -> const_iterator
{
    return map_.cbegin();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::end() const noexcept -> const_iterator
{
    return map_.end();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::cend() const noexcept -> const_iterator
{
    return map_.cend();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::at(const key_type& key) const -> const mapped_type&
{
    return map_.at(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
auto threshold_counter<K, H, P, A, M>::get(const key_type& key, mapped_type n) const -> mapped_type
{
    auto it = map_.find(key);
    if (it == map_.end()) {
        return n;
    }
    return it->second;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
void threshold_counter<K, H, P, A, M>::add(const key_type& key)
{
    map_[key]++;
    count_++;
    check_autocompact();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
void threshold_counter<K, H, P, A, M>::add(key_type&& key)
{
    map_[forward<key_type>(key)]++;
    count_++;
    check_autocompact();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
template <typename Iter>
void threshold_counter<K, H, P, A, M>::update(Iter first, Iter last)
{
    size_t before = count_;
    size_t added = counter_detail::update_nonnegative(map_, first, last);
    count_ += added;
    // short-circuit if we can
    if (added >= interval_ || ((count_ % interval_) < (before % interval_))) {
        autocompact();
    }
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void threshold_counter<K, H, P, A, M>::clear()
{
    map_.clear();
    count_ = 0;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
void threshold_counter<K, H, P, A, M>::swap(self& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(map_, rhs.map_);
    swap(interval_, rhs.interval_);
    swap(count_, rhs.count_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::most_common(size_t n) const -> counter_detail::mutable_pair_list<self>
{
    return counter_detail::most_common(map_, n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::elements() const -> counter_detail::key_list<self>
{
    return counter_detail::elements(map_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
count_t threshold_counter<K, H, P, A, M>::get_common_count() const
{
    return accumulate(begin(), end(), count_t(0), [](count_t l, const value_type& rhs) {
        count_t r = rhs.second;
        return l + r;
    });
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline count_t threshold_counter<K, H, P, A, M>::get_uncommon_count() const
{
    return count_ - get_common_count();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline double threshold_counter<K, H, P, A, M>::get_commonality() const
{
    return static_cast<double>(get_common_count()) / count_;
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
void threshold_counter<K, H, P, A, M>::autocompact()
{
    // create compact representation
    map_type map;
    count_t threshold = count_ /interval_;
    // don't use move semantics: we don't want to trigger any re-balancing
    // or other undefined behavior.
    for (const value_type& pair: map_) {
        if (pair.second >= threshold) {
            map[pair.first] = pair.second;
        }
    }

    // overwrite existing counts
    map_ = move(map);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void threshold_counter<K, H, P, A, M>::check_autocompact()
{
    if (count_ % interval_ == 0) {
        autocompact();
    }
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::bucket_count() const noexcept -> size_type
{
    return map_.bucket_count();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::max_bucket_count() const noexcept -> size_type
{
    return map_.max_bucket_count();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::bucket_size(size_type n) const -> size_type
{
    return map_.bucket_size(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::bucket(const key_type& key) const -> size_type
{
    return map_.bucket(key);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline float threshold_counter<K, H, P, A, M>::load_factor() const noexcept
{
    return map_.load_factor();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline float threshold_counter<K, H, P, A, M>::max_load_factor() const noexcept
{
    return map_.max_load_factor();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void threshold_counter<K, H, P, A, M>::max_load_factor(float n)
{
    map_.max_load_factor(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void threshold_counter<K, H, P, A, M>::rehash(size_type n)
{
    map_.rehash(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline void threshold_counter<K, H, P, A, M>::reserve(size_type n)
{
    map_.reserve(n);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::hash_function() const -> hasher
{
    return hasher();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::key_eq() const -> key_equal
{
    return key_equal();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline auto threshold_counter<K, H, P, A, M>::get_allocator() const noexcept -> allocator_type
{
    return map_.get_allocator();
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline threshold_counter<K, H, P, A, M>::operator counter_type() const
{
    return counter_type(map_);
}


template <typename K, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> class M>
inline threshold_counter<K, H, P, A, M>::operator map_type() const
{
    return map_;
}

PYCPP_END_NAMESPACE
