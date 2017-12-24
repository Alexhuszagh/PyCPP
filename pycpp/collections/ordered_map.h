//  :copyright: (c) 2017 Tessil.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast Hashmap that remembers the order of insertion.
 */

#pragma once

#include <pycpp/collections/ordered.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  Implementation of an hash map using open adressing with robin hood
 *  with backshift delete to resolve collision.
 *
 *  The particularity of the hash map is that it remembers the order in
 *  which the elements were added and provide a way to access the
 *  structure which stores these values through the 'values_container()'
 *  method. The used container is defined by ValueTypeContainer, by
 *  default a deque is used (faster on rehash) but a vector
 *  may be used. In this case the map provides a 'data()' method which
 *  give a direct access to the memory used to store the values (which
 *  can be useful to communicate with C API's).
 *
 *  Iterators invalidation:
 *      - clear, operator=, reserve, rehash: always invalidate the
 *        iterators (also invalidate end()).
 *      - insert, emplace, emplace_hint, operator[]: when a vector
 *        is used as ValueTypeContainer and if size() < capacity(), only
 *        end(). Otherwise all the iterators are invalidated if an
 *        insert occurs.
 *      - erase: when a vector is used as ValueTypeContainer
 *        invalidate the iterator of the erased element and all the
 *        ones after the erased element (including end()). Otherwise
 *        all the iterators are invalidated if an erase occurs.
 */
template <
    typename Key,
    typename T,
    typename Hash = hash<Key>,
    typename KeyEqual = equal_to<Key>,
    typename Allocator = allocator<pair<Key, T>>,
    template <typename, typename> class ValueTypeContainer = deque
>
class ordered_map
{
private:
    class KeySelect
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using mutable_value_type = pair<Key, T>;

        const key_type& operator()(const mutable_value_type& key_value) const
        {
            return key_value.first;
        }

        key_type& operator()(mutable_value_type& key_value)
        {
            return key_value.first;
        }
    };

    class ValueSelect
    {
    public:
        using key_type = Key;
        using mapped_type = T;
        using mutable_value_type = pair<Key, T>;

        const mapped_type& operator()(const mutable_value_type& key_value) const
        {
            return key_value.second;
        }

        mapped_type& operator()(mutable_value_type& key_value)
        {
            return key_value.second;
        }
    };

    using ht = detail_ordered_hash::ordered_hash<pair<const Key, T>, pair<Key, T>, KeySelect, ValueSelect, Hash, KeyEqual, Allocator, ValueTypeContainer<pair<Key, T>, Allocator>>;

public:
    using key_type = typename ht::key_type;
    using mapped_type = T;
    using value_type = typename ht::value_type;
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
    using reverse_iterator = typename ht::reverse_iterator;
    using const_reverse_iterator = typename ht::const_reverse_iterator;

    using values_container_type = typename ht::values_container_type;

    // CONSTRUCTORS

    ordered_map():
        ordered_map(ht::DEFAULT_INIT_BUCKETS_SIZE)
    {}

    explicit ordered_map(size_type bucket_count,
            const Hash& hash = Hash(),
            const KeyEqual& equal = KeyEqual(),
            const Allocator& alloc = Allocator()):
        m_ht(bucket_count, hash, equal, alloc, ht::DEFAULT_MAX_LOAD_FACTOR)
    {}

    ordered_map(size_type bucket_count, const Allocator& alloc):
        ordered_map(bucket_count, Hash(), KeyEqual(), alloc)
    {}

    ordered_map(size_type bucket_count, const Hash& hash, const Allocator& alloc):
        ordered_map(bucket_count, hash, KeyEqual(), alloc)
    {}

    explicit ordered_map(const Allocator& alloc):
        ordered_map(ht::DEFAULT_INIT_BUCKETS_SIZE, alloc)
    {}

    template <typename InputIt>
    ordered_map(InputIt first, InputIt last,
            size_type bucket_count = ht::DEFAULT_INIT_BUCKETS_SIZE,
            const Hash& hash = Hash(),
            const KeyEqual& equal = KeyEqual(),
            const Allocator& alloc = Allocator()):
        ordered_map(bucket_count, hash, equal, alloc)
    {
        insert(first, last);
    }

    template <typename InputIt>
    ordered_map(InputIt first, InputIt last,
            size_type bucket_count,
            const Allocator& alloc):
        ordered_map(first, last, bucket_count, Hash(), KeyEqual(), alloc)
    {}

    template <typename InputIt>
    ordered_map(InputIt first, InputIt last,
            size_type bucket_count,
            const Hash& hash,
            const Allocator& alloc):
        ordered_map(first, last, bucket_count, hash, KeyEqual(), alloc)
    {}

    ordered_map(initializer_list<value_type> init,
            size_type bucket_count = ht::DEFAULT_INIT_BUCKETS_SIZE,
            const Hash& hash = Hash(),
            const KeyEqual& equal = KeyEqual(),
            const Allocator& alloc = Allocator()) :
        ordered_map(init.begin(), init.end(), bucket_count, hash, equal, alloc)
    {}

    ordered_map(initializer_list<value_type> init,
            size_type bucket_count,
            const Allocator& alloc):
        ordered_map(init.begin(), init.end(), bucket_count, Hash(), KeyEqual(), alloc)
    {}

    ordered_map(initializer_list<value_type> init,
            size_type bucket_count,
            const Hash& hash,
            const Allocator& alloc):
        ordered_map(init.begin(), init.end(), bucket_count, hash, KeyEqual(), alloc)
    {}

    // TODO: need move constructors
    // TODO: need copy assignemnt
    // TODO: need move assignment

    ordered_map& operator=(initializer_list<value_type> ilist)
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

    reverse_iterator rbegin() noexcept
    {
        return m_ht.rbegin();
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return m_ht.rbegin();
    }

    const_reverse_iterator rcbegin() const noexcept
    {
        return m_ht.rcbegin();
    }

    reverse_iterator rend() noexcept
    {
        return m_ht.rend();
    }

    const_reverse_iterator rend() const noexcept
    {
        return m_ht.rend();
    }

    const_reverse_iterator rcend() const noexcept
    {
        return m_ht.rcend();
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

    template <typename P, enable_if_t<is_constructible<value_type, P&&>::value>* = nullptr>
    pair<iterator,bool> insert(P&& value)
    {
        return m_ht.emplace(forward<P>(value));
    }

    pair<iterator, bool> insert(value_type&& value)
    {
        return m_ht.insert(move(value));
    }

    iterator insert(const_iterator hint, const value_type& value)
    {
        if (hint != cend() && m_ht.key_eq()(KeySelect()(*hint), KeySelect()(value))) {
            return m_ht.get_mutable_iterator(hint);
        }

        return m_ht.insert(value).first;
    }

    template <typename P, enable_if_t<is_constructible<value_type, P&&>::value>* = nullptr>
    iterator insert(const_iterator hint, P&& value)
    {
        value_type val(forward<P>(value));
        if (hint != cend() && m_ht.key_eq()(KeySelect()(*hint), KeySelect()(val))) {
            return m_ht.get_mutable_iterator(hint);
        }

        return m_ht.insert(move(val)).first;
    }

    iterator insert(const_iterator hint, value_type&& value)
    {
        if (hint != cend() && m_ht.key_eq()(KeySelect()(*hint), KeySelect()(value))) {
            return m_ht.get_mutable_iterator(hint);
        }

        return m_ht.insert(move(value)).first;
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
     *  Due to the way elements are stored, emplace will need to move or
     *  copy the key-value once. The method is equivalent to
     *  insert(value_type(forward<Args>(args)...));
     *
     *  Mainly here for compatibility with the unordered_map
     *  interface.
     */
    template <typename... Args>
    pair<iterator,bool> emplace(Args&&... args)
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
    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return m_ht.insert(hint, value_type(forward<Args>(args)...));
    }

    /**
     *  When erasing an element, the insert order will be preserved and
     *  no holes will be present in the container returned by
     *  'values_container()'.
     *
     *  The method is in O(n), if the order is not important
     *  'unordered_erase(...)' method is faster with an O(1)
     *  average complexity.
     */
    iterator erase(iterator pos)
    {
        return m_ht.erase(pos);
    }

    /**
     *  @copydoc erase(iterator pos)
     */
    iterator erase(const_iterator pos)
    {
        return m_ht.erase(pos);
    }

    /**
     *  @copydoc erase(iterator pos)
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        return m_ht.erase(first, last);
    }

    /**
     *  @copydoc erase(iterator pos)
     */
    size_type erase(const key_type& key)
    {
        return m_ht.erase(key);
    }

    /**
     *  @copydoc erase(iterator pos)
     *
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    size_type erase(const K& key)
    {
        return m_ht.erase(key);
    }

    void swap(ordered_map& other)
    {
        other.m_ht.swap(m_ht);
    }

    // LOOKUP

    T& at(const Key& key)
    {
        return m_ht.at(key);
    }

    const T& at(const Key& key) const
    {
        return m_ht.at(key);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    T& at(const K& key)
    {
        return m_ht.at(key);
    }

    /**
     *  @copydoc at(const K& key)
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    const T& at(const K& key) const
    {
        return m_ht.at(key);
    }

    T& operator[](const Key& key)
    {
        return m_ht[key];
    }

    T& operator[](Key&& key)
    {
        return m_ht[move(key)];
    }

    size_type count(const Key& key) const
    {
        return m_ht.count(key);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists. If so, K must be
     *  hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    size_type count(const K& key) const
    {
        return m_ht.count(key);
    }

    iterator find(const Key& key)
    {
        return m_ht.find(key);
    }

    const_iterator find(const Key& key) const
    {
        return m_ht.find(key);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists. If so, K must be
     *  hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    iterator find(const K& key)
    {
        return m_ht.find(key);
    }

    /**
     *  @copydoc find(const K& key)
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    const_iterator find(const K& key) const
    {
        return m_ht.find(key);
    }

    pair<iterator, iterator> equal_range(const Key& key)
    {
        return m_ht.equal_range(key);
    }
    pair<const_iterator, const_iterator> equal_range(const Key& key) const
    {
        return m_ht.equal_range(key);
    }

    /**
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists. If so, K must be
     *  hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    pair<iterator, iterator> equal_range(const K& key)
    {
        return m_ht.equal_range(key);
    }

    /**
     *  @copydoc equal_range(const K& key)
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
        return m_ht.equal_range(key);
    }

    // BUCKETS

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

    const_reference front() const
    {
        return m_ht.front();
    }

    const_reference back() const
    {
        return m_ht.back();
    }

    /**
     *  Only available if ValueTypeContainer is an vector. Same as
     *  calling 'values_container().data()'.
     */
    template <typename U = values_container_type, enable_if_t<detail_ordered_hash::is_vector<U>::value>* = nullptr>
    const typename values_container_type::value_type* data() const noexcept
    {
        return m_ht.data();
    }

    /**
     *  Return the container in which the values are stored. The values
     *  are in the same order as the insertion order and are contiguous
     *  in the structure, no holes (size() == values_container().size()).
     */
    const values_container_type& values_container() const noexcept
    {
        return m_ht.values_container();
    }

    template <typename U = values_container_type, enable_if_t<detail_ordered_hash::is_vector<U>::value>* = nullptr>
    size_type capacity() const noexcept
    {
        return m_ht.capacity();
    }

    void shrink_to_fit()
    {
        m_ht.shrink_to_fit();
    }

    void pop_back()
    {
        m_ht.pop_back();
    }

    /**
     *  Faster erase operation with an O(1) average complexity but it
     *  doesn't preserve the insertion order.
     *
     *  If an erasure occurs, the last element of the map will take the
     *  place of the erased element.
     */
    iterator unordered_erase(iterator pos)
    {
        return m_ht.unordered_erase(pos);
    }

    /**
     * @copydoc unordered_erase(iterator pos)
     */
    iterator unordered_erase(const_iterator pos)
    {
        return m_ht.unordered_erase(pos);
    }

    /**
     *  @copydoc unordered_erase(iterator pos)
     */
    size_type unordered_erase(const key_type& key)
    {
        return m_ht.unordered_erase(key);
    }

    /**
     *  @copydoc unordered_erase(iterator pos)
     *
     *  This overload only participates in the overload resolution if
     *  the typedef KeyEqual::is_transparent exists.
     *  If so, K must be hashable and comparable to Key.
     */
    template <typename K, typename KE = KeyEqual, enable_if_t<detail_ordered_hash::has_is_transparent<KE>::value>* = nullptr>
    size_type unordered_erase(const K& key)
    {
        return m_ht.unordered_erase(key);
    }

    friend bool operator==(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht == rhs.m_ht;
    }

    friend bool operator!=(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht != rhs.m_ht;
    }

    friend bool operator<(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht < rhs.m_ht;
    }

    friend bool operator<=(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht <= rhs.m_ht;
    }

    friend bool operator>(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht > rhs.m_ht;
    }

    friend bool operator>=(const ordered_map& lhs, const ordered_map& rhs)
    {
        return lhs.m_ht >= rhs.m_ht;
    }

    friend void swap(ordered_map& lhs, ordered_map& rhs)
    {
        lhs.swap(rhs);
    }

private:
    ht m_ht;
};

// SPECIALIZATION
// --------------

template <
    typename Key,
    typename T,
    typename Hash,
    typename KeyEqual,
    typename Allocator,
    template <typename, typename> class ValueTypeContainer
>
struct is_relocatable<ordered_map<Key, T, Hash, KeyEqual, Allocator, ValueTypeContainer>>: false_type
{};

PYCPP_END_NAMESPACE
