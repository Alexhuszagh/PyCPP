//  :copyright: (c) 2017 Tessil.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast hashmap-based set using robin hood indexing.
 */

#pragma once

#include <pycpp/collections/robin.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  Implementation of a hash set using open-adressing and the robin
 *  hood hashing algorithm with backward shift deletion.
 *
 *  For operations modifying the hash set (insert, erase, rehash, ...),
 *  the strong exception guarantee is only guaranteed when the expression
 *  `is_nothrow_swappable<Key>::value &&
 *  is_nothrow_move_constructible<Key>::value` is true, otherwise
 *  if an exception is thrown during the swap or the move, the hash set
 *  may end up in a undefined state. Per the standard a `Key` with a
 *  noexcept copy constructor and no move constructor also satisfies the
 *  `is_nothrow_move_constructible<Key>::value` criterion (and
 *  will thus guarantee the strong exception for the set).
 *
 *  When `StoreHash` is true, 32 bits of the hash are stored alongside
 *  the values. It can improve the performance during lookups if the
 *  `KeyEqual` function takes time (or engenders a cache-miss for example)
 *  as we then compare the stored hashes before comparing the keys.
 *  When `robin_detail::power_of_two_growth_policy` is used as
 *  `GrowthPolicy`, it may also speed-up the rehash process as we can
 *  avoid to recalculate the hash. When it is detected that storing the
 *  hash will not incur any memory penality due to alignement (i.e.
 *  `sizeof(robin_detail::bucket_entry<ValueType, true>) ==
 *  sizeof(robin_detail::bucket_entry<ValueType, false>)`) and
 *  `robin_detail::power_of_two_growth_policy` is used, the hash will be
 *  stored even if `StoreHash` is false so that we can speed-up the rehash
 *  (but it will not be used on lookups unless `StoreHash` is true).
 *
 *  `GrowthPolicy` defines how the set grows and consequently how a
 *  hash value is mapped to a bucket. By default the set uses
 *  `robin_detail::power_of_two_growth_policy`. This policy keeps the
 *  number of buckets to a power of two and uses a mask to set the hash
 *  to a bucket instead of the slow modulo. Other growth policies are
 *  available and you may define your own growth policy, check
 *  `robin_detail::power_of_two_growth_policy` for the interface.
 *
 *  If the destructor of `Key` throws an exception, the behaviour of the
 *  class is undefined.
 *
 *  Iterators invalidation:
 *      - clear, operator=, reserve, rehash: always invalidate the
 *        iterators.
 *      - insert, emplace, emplace_hint, operator[]: if there is an
 *        effective insert, invalidate the iterators.
 *      - erase: always invalidate the iterators.
 */
template <
    typename Key,
    typename Hash = hash<Key>,
    typename KeyEqual = equal_to<Key>,
    typename Allocator = allocator<Key>,
    bool StoreHash = false,
    typename GrowthPolicy = robin_detail::power_of_two_growth_policy<2>
>
class robin_set
{
private:
    template <typename U>
    using has_is_transparent = robin_detail::has_is_transparent<U>;

    class KeySelect
    {
    public:
        using key_type = Key;

        const key_type& operator()(const Key& key) const noexcept
        {
            return key;
        }

        key_type& operator()(Key& key) noexcept
        {
            return key;
        }
    };

    using ht = robin_detail::robin_hash<Key, Key, KeySelect, void, Hash, KeyEqual, Allocator, StoreHash, GrowthPolicy>;

public:
    using key_type = typename ht::key_type;
    using value_type = typename ht::value_type;
    using mutable_value_type = typename ht::mutable_value_type;
    using size_type = typename ht::size_type;
    using difference_type = typename ht::difference_type;
    using hasher = typename ht::hasher;
    using key_equal = typename ht::key_equal;
    using allocator_type = typename ht::allocator_type;
    using reference = typename ht::reference;
    using const_reference = typename ht::const_reference;
    using pointer = typename ht::pointer;
    using const_pointer = typename ht::const_pointer;
    using iterator = typename ht::iterator;
    using const_iterator = typename ht::const_iterator;


    // CONSTRUCTORS

    robin_set():
        robin_set(ht::DEFAULT_INIT_BUCKETS_SIZE)
    {}

    explicit robin_set(size_type bucket_count,
                       const Hash& hash = Hash(),
                       const KeyEqual& equal = KeyEqual(),
                       const Allocator& alloc = Allocator()):
        m_ht(bucket_count, hash, equal, alloc, ht::DEFAULT_MAX_LOAD_FACTOR)
    {}

    robin_set(size_type bucket_count,
              const Allocator& alloc):
        robin_set(bucket_count, Hash(), KeyEqual(), alloc)
    {}

    robin_set(size_type bucket_count,
              const Hash& hash,
              const Allocator& alloc):
        robin_set(bucket_count, hash, KeyEqual(), alloc)
    {}

    explicit robin_set(const Allocator& alloc):
        robin_set(ht::DEFAULT_INIT_BUCKETS_SIZE, alloc)
    {}

    template <typename InputIt>
    robin_set(InputIt first, InputIt last,
              size_type bucket_count = ht::DEFAULT_INIT_BUCKETS_SIZE,
              const Hash& hash = Hash(),
              const KeyEqual& equal = KeyEqual(),
              const Allocator& alloc = Allocator()):
        robin_set(bucket_count, hash, equal, alloc)
    {
        insert(first, last);
    }

    template <typename InputIt>
    robin_set(InputIt first, InputIt last,
              size_type bucket_count,
              const Allocator& alloc):
        robin_set(first, last, bucket_count, Hash(), KeyEqual(), alloc)
    {}

    template <typename InputIt>
    robin_set(InputIt first, InputIt last,
              size_type bucket_count,
              const Hash& hash,
              const Allocator& alloc):
        robin_set(first, last, bucket_count, hash, KeyEqual(), alloc)
    {}

    robin_set(initializer_list<value_type> init,
              size_type bucket_count = ht::DEFAULT_INIT_BUCKETS_SIZE,
              const Hash& hash = Hash(),
              const KeyEqual& equal = KeyEqual(),
              const Allocator& alloc = Allocator()):
        robin_set(init.begin(), init.end(), bucket_count, hash, equal, alloc)
    {}

    robin_set(initializer_list<value_type> init,
              size_type bucket_count,
              const Allocator& alloc):
        robin_set(init.begin(), init.end(), bucket_count, Hash(), KeyEqual(), alloc)
    {}

    robin_set(initializer_list<value_type> init,
              size_type bucket_count,
              const Hash& hash,
              const Allocator& alloc):
        robin_set(init.begin(), init.end(), bucket_count, hash, KeyEqual(), alloc)
    {}

    robin_set& operator=(initializer_list<value_type> ilist)
    {
        m_ht.clear();

        m_ht.reserve(ilist.size());
        m_ht.insert(ilist.begin(), ilist.end());

        return *this;
    }

    allocator_type get_allocator() const
    {
        return m_ht.get_allocator();
    }

    // ITERATORS
    iterator begin() noexcept
    {
        return m_ht.begin();
    }

    const_iterator begin() const noexcept
    {
        return m_ht.begin();
    }

    const_iterator cbegin() const noexcept
    {
        return m_ht.cbegin();
    }

    iterator end() noexcept
    {
        return m_ht.end();
    }

    const_iterator end() const noexcept
    {
        return m_ht.end();
    }

    const_iterator cend() const noexcept
    {
        return m_ht.cend();
    }

    // CAPACITY

    bool empty() const noexcept
    {
        return m_ht.empty();
    }

    size_type size() const noexcept
    {
        return m_ht.size();
    }

    size_type max_size() const noexcept
    {
        return m_ht.max_size();
    }

    // MODIFIERS

    void clear() noexcept
    {
        m_ht.clear();
    }

    pair<iterator, bool> insert(const value_type& value)
    {
        return m_ht.insert(value);
    }

    pair<iterator, bool> insert(value_type&& value)
    {
        return m_ht.insert(move(value));
    }

    iterator insert(const_iterator hint, const value_type& value)
    {
        return m_ht.insert(hint, value);
    }

    iterator insert(const_iterator hint, value_type&& value)
    {
        return m_ht.insert(hint, move(value));
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last)
    {
        m_ht.insert(first, last);
    }

    void insert(initializer_list<value_type> ilist)
    {
        m_ht.insert(ilist.begin(), ilist.end());
    }

    /**
     *  Due to the way elements are stored, emplace will need to move
     *  or copy the key-value once. The method is equivalent to
     *  insert(value_type(forward<Args>(args)...));
     *
     *  Mainly here for compatibility with the unordered_map
     *  interface.
     */
    template <typename... Args>
    pair<iterator, bool> emplace(Args&&... args)
    {
        return m_ht.emplace(forward<Args>(args)...);
    }

    /**
     *  Due to the way elements are stored, emplace_hint will need to
     *  move or copy the key-value once. The method is equivalent to
     *  insert(hint, value_type(forward<Args>(args)...));
     *
     *  Mainly here for compatibility with the unordered_map
     *  interface.
     */
    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return m_ht.emplace_hint(hint, forward<Args>(args)...);
    }

    iterator erase(iterator pos)
    {
        return m_ht.erase(pos);
    }

    iterator erase(const_iterator pos)
    {
        return m_ht.erase(pos);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        return m_ht.erase(first, last);
    }

    size_type erase(const key_type& key)
    {
        return m_ht.erase(key);
    }

    /**
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup to the value if you already have
     *  the hash.
     */
    size_type erase(const key_type& key, size_t precalculated_hash)
    {
        return m_ht.erase(key, precalculated_hash);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    size_type erase(const K& key)
    {
        return m_ht.erase(key);
    }

    /**
     *  @copydoc erase(const K& key)
     *
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup to the value if you already have
     *  the hash.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    size_type erase(const K& key, size_t precalculated_hash)
    {
        return m_ht.erase(key, precalculated_hash);
    }

    void swap(robin_set& other)
    {
        other.m_ht.swap(m_ht);
    }

    // LOOKUP

    size_type count(const Key& key) const
    {
        return m_ht.count(key);
    }

    /**
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    size_type count(const Key& key, size_t precalculated_hash) const
    {
        return m_ht.count(key, precalculated_hash);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    size_type count(const K& key) const { return m_ht.count(key); }

    /**
     *  @copydoc count(const K& key) const
     *
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    size_type count(const K& key, size_t precalculated_hash) const
    {
        return m_ht.count(key, precalculated_hash);
    }

    iterator find(const Key& key)
    {
        return m_ht.find(key);
    }

    /**
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    iterator find(const Key& key, size_t precalculated_hash)
    {
        return m_ht.find(key, precalculated_hash);
    }

    const_iterator find(const Key& key) const
    {
        return m_ht.find(key);
    }

    /**
     *  @copydoc find(const Key& key, size_t precalculated_hash)
     */
    const_iterator find(const Key& key, size_t precalculated_hash) const
    {
        return m_ht.find(key, precalculated_hash);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    iterator find(const K& key)
    {
        return m_ht.find(key);
    }

    /**
     *  @copydoc find(const K& key)
     *
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    iterator find(const K& key, size_t precalculated_hash)
    {
        return m_ht.find(key, precalculated_hash);
    }

    /**
     *  @copydoc find(const K& key)
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    const_iterator find(const K& key) const
    {
        return m_ht.find(key);
    }

    /**
     *  @copydoc find(const K& key)
     *
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    const_iterator find(const K& key, size_t precalculated_hash) const
    {
        return m_ht.find(key, precalculated_hash);
    }

    pair<iterator, iterator> equal_range(const Key& key)
    {
        return m_ht.equal_range(key);
    }

    /**
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    pair<iterator, iterator> equal_range(const Key& key, size_t precalculated_hash)
    {
        return m_ht.equal_range(key, precalculated_hash);
    }

    pair<const_iterator, const_iterator> equal_range(const Key& key) const
    {
        return m_ht.equal_range(key);
    }

    /**
     *  @copydoc equal_range(const Key& key, size_t precalculated_hash)
     */
    pair<const_iterator, const_iterator> equal_range(const Key& key, size_t precalculated_hash) const
    {
        return m_ht.equal_range(key, precalculated_hash);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    pair<iterator, iterator> equal_range(const K& key)
    {
        return m_ht.equal_range(key);
    }

    /**
     *  @copydoc equal_range(const K& key)
     *
     *  Use the hash value 'precalculated_hash' instead of hashing the
     *  key. The hash value should be the same as hash_function()(key).
     *  Useful to speed-up the lookup if you already have the hash.
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    pair<iterator, iterator> equal_range(const K& key, size_t precalculated_hash)
    {
        return m_ht.equal_range(key, precalculated_hash);
    }

    /**
     *  @copydoc equal_range(const K& key)
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
        return m_ht.equal_range(key);
    }

    /**
     *  @copydoc equal_range(const K& key, size_t precalculated_hash)
     */
    template <class K, class KE = KeyEqual, enable_if_t<has_is_transparent<KE>::value>* = nullptr>
    pair<const_iterator, const_iterator> equal_range(const K& key, size_t precalculated_hash) const
    {
        return m_ht.equal_range(key, precalculated_hash);
    }

    // BUCKET INTERFACE

    size_type bucket_count() const
    {
        return m_ht.bucket_count();
    }

    size_type max_bucket_count() const
    {
        return m_ht.max_bucket_count();
    }

    // HASH POLICY

    float load_factor() const
    {
        return m_ht.load_factor();
    }

    float max_load_factor() const
    {
        return m_ht.max_load_factor();
    }

    void max_load_factor(float ml)
    {
        m_ht.max_load_factor(ml);
    }

    void rehash(size_type count)
    {
        m_ht.rehash(count);
    }

    void reserve(size_type count)
    {
        m_ht.reserve(count);
    }

    // OBSERVERS
    hasher hash_function() const
    {
        return m_ht.hash_function();
    }

    key_equal key_eq() const
    {
        return m_ht.key_eq();
    }

    // OTHER

    /**
     *  Convert a const_iterator to an iterator.
     */
    iterator mutable_iterator(const_iterator pos)
    {
        return m_ht.mutable_iterator(pos);
    }

    friend bool operator==(const robin_set& lhs, const robin_set& rhs)
    {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        for (const auto& element_lhs: lhs) {
            const auto it_element_rhs = rhs.find(element_lhs);
            if (it_element_rhs == rhs.cend()) {
                return false;
            }
        }

        return true;
    }

    friend bool operator!=(const robin_set& lhs, const robin_set& rhs)
    {
        return !operator==(lhs, rhs);
    }

    friend void swap(robin_set& lhs, robin_set& rhs)
    {
        lhs.swap(rhs);
    }

private:
    ht m_ht;
};


/**
 *  Same as `robin_set<Key, Hash, KeyEqual, Allocator, StoreHash,
 *  robin_detail::prime_growth_policy>`.
 */
template <
    typename Key,
    typename Hash = hash<Key>,
    typename KeyEqual = equal_to<Key>,
    typename Allocator = allocator<Key>,
    bool StoreHash = false
>
using robin_pg_set = robin_set<Key, Hash, KeyEqual, Allocator, StoreHash, robin_detail::prime_growth_policy>;

PYCPP_END_NAMESPACE
