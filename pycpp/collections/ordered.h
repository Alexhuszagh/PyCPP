//  :copyright: (c) 2017 Tessil.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast Hashmap that remembers the order of insertion.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#if (defined(HAVE_GNUC) && (GNUC_MAJOR_VERSION == 4) && (GNUC_MINOR_VERSION < 9))
#   define TSL_NO_CONTAINER_ERASE_CONST_ITERATOR
#endif


namespace detail_ordered_hash
{
// DETAIL
// ------


template <typename T>
struct make_void
{
    using type = void;
};


template <typename T, typename = void>
struct has_is_transparent: std::false_type
{};


template <typename T>
struct has_is_transparent<T, typename make_void<typename T::is_transparent>::type>: std::true_type
{};


template <typename T, typename = void>
struct is_vector: std::false_type
{};


template <typename T>
struct is_vector<T, typename std::enable_if<
    std::is_same<T, std::vector<typename T::value_type, typename T::allocator_type>>::value
    >::type>: std::true_type
{};


template <typename ValueType, typename KeySelect, typename ValueSelect, typename Hash,
          typename KeyEqual, typename Allocator, typename ValueTypeContainer>
class ordered_hash
{
private:
    static_assert(std::is_same<typename ValueTypeContainer::value_type, ValueType>::value,
                  "ValueTypeContainer::value_type != ValueType.");
    static_assert(std::is_same<typename ValueTypeContainer::allocator_type, Allocator>::value,
                  "ValueTypeContainer::allocator_type != Allocator.");

    using Key = typename KeySelect::key_type;

public:
    template <bool is_const>
    class ordered_iterator;

    using key_type = Key;
    using value_type = ValueType;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = ordered_iterator<false>;
    using const_iterator = ordered_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using values_container_type = ValueTypeContainer;

public:
    template <bool is_const>
    class ordered_iterator
    {
        friend class ordered_hash;
    private:
        using iterator = typename std::conditional<
            is_const,
            typename values_container_type::const_iterator,
            typename values_container_type::iterator
        >::type;

        ordered_iterator(iterator it) noexcept:
            m_iterator(it)
        {}

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const typename ordered_hash::value_type;
        using difference_type = typename iterator::difference_type;
        using reference = value_type&;
        using pointer = value_type*;


        ordered_iterator() noexcept
        {}

        ordered_iterator(const ordered_iterator<false>& other) noexcept: m_iterator(other.m_iterator)
        {}

        const typename ordered_hash::key_type& key() const
        {
            return KeySelect()(*m_iterator);
        }

        template <typename U = ValueSelect, typename std::enable_if<!std::is_same<U, void>::value>::type* = nullptr>
        typename std::conditional<is_const, const typename U::value_type&, typename U::value_type&>::type value() const
        {
            return m_iterator->second;
        }

        reference operator*() const
        {
            return *m_iterator;
        }

        pointer operator->() const
        {
            return m_iterator.operator->();
        }

        ordered_iterator& operator++()
        {
            ++m_iterator; return *this;
        }

        ordered_iterator& operator--()
        {
            --m_iterator; return *this;
        }

        ordered_iterator operator++(int)
        {
            ordered_iterator tmp(*this); ++*this; return tmp;
        }

        ordered_iterator operator--(int)
        {
            ordered_iterator tmp(*this); --*this; return tmp;
        }

        reference operator[](difference_type n) const
        {
            return m_iterator[n];
        }

        ordered_iterator& operator+=(difference_type n)
        {
            m_iterator+= n; return *this;
        }

        ordered_iterator& operator-=(difference_type n)
        {
            m_iterator-= n; return *this;
        }

        ordered_iterator operator+(difference_type n)
        {
            ordered_iterator tmp(*this); return tmp += n;
        }

        ordered_iterator operator-(difference_type n)
        {
            ordered_iterator tmp(*this); return tmp -= n;
        }

        friend bool operator==(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator == rhs.m_iterator;
        }

        friend bool operator!=(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator != rhs.m_iterator;
        }

        friend bool operator<(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator < rhs.m_iterator;
        }

        friend bool operator>(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator > rhs.m_iterator;
        }

        friend bool operator<=(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator <= rhs.m_iterator;
        }

        friend bool operator>=(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator >= rhs.m_iterator;
        }

        friend difference_type operator+(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator + rhs.m_iterator;
        }

        friend difference_type operator-(const ordered_iterator& lhs, const ordered_iterator& rhs)
        {
            return lhs.m_iterator - rhs.m_iterator;
        }

    private:
        iterator m_iterator;
    };

private:
    /**
     *  Each bucket entry stores a 32-bits index which is the index in
     *  m_values corresponding to the bucket and a 32 bits hash
     *  (truncated if the original was 64-bits) corresponding to the
     *  value.
     */
    class bucket_entry
    {
    public:
        using index_type = std::uint32_t;
        using truncated_hash_type = std::uint32_t;

        bucket_entry() noexcept:
            m_index(0),
            m_hash(0)
        {
            set_empty();
        }

        bool has_index() const noexcept
        {
            return !empty();
        }

        bool empty() const noexcept
        {
            return m_index == empty_index;
        }

        void set_empty() noexcept
        {
            m_index = empty_index;
        }

        index_type index() const noexcept
        {
            assert(has_index());
            return m_index;
        }

        truncated_hash_type truncated_hash() const noexcept
        {
            assert(has_index());
            return m_hash;
        }

        void set_index(std::size_t index) noexcept
        {
            assert(index <= max_size());
            m_index = static_cast<index_type>(index & 0xFFFFFFFF);
        }

        void set_hash(std::size_t hash) noexcept
        {
            m_hash = truncate_hash(hash);
        }

        static truncated_hash_type truncate_hash(std::size_t hash)
        {
            return static_cast<truncated_hash_type>(hash & 0xFFFFFFFF);
        }

        static std::size_t max_size()
        {
            return std::numeric_limits<index_type>::max() - nb_reserved_indexes;
        }

    private:
        static const index_type empty_index = std::numeric_limits<index_type>::max();
        static const std::size_t nb_reserved_indexes = 1;

        index_type m_index;
        truncated_hash_type m_hash;
    };


    using buckets_container_allocator = typename std::allocator_traits<allocator_type>::template rebind_alloc<bucket_entry>;
    using buckets_container_type = std::vector<bucket_entry, buckets_container_allocator>;

public:
    ordered_hash(size_type bucket_count,
            const Hash& hash,
            const KeyEqual& equal,
            const Allocator& alloc,
            float max_load_factor):
        m_buckets(alloc),
        m_values(alloc),
        m_hash(hash),
        m_key_equal(equal)
    {
        if (bucket_count == 0) {
            m_mask = 0;
        } else {
            m_buckets.resize(round_up_to_power_of_two(bucket_count));
            m_mask = this->bucket_count() - 1;
        }

        this->max_load_factor(max_load_factor);
    }

    allocator_type get_allocator() const
    {
        return m_values.get_allocator();
    }

    // ITERATORS

    iterator begin() noexcept
    {
        return iterator(m_values.begin());
    }

    const_iterator begin() const noexcept
    {
        return cbegin();
    }

    const_iterator cbegin() const noexcept
    {
        return const_iterator(m_values.cbegin());
    }

    iterator end() noexcept
    {
        return iterator(m_values.end());
    }

    const_iterator end() const noexcept
    {
        return cend();
    }

    const_iterator cend() const noexcept
    {
        return const_iterator(m_values.cend());
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(m_values.end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return rcbegin();
    }

    const_reverse_iterator rcbegin() const noexcept
    {
        return const_reverse_iterator(m_values.cend());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(m_values.begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return rcend();
    }

    const_reverse_iterator rcend() const noexcept
    {
        return const_reverse_iterator(m_values.cbegin());
    }

    // CAPACITY

    bool empty() const noexcept
    {
        return m_values.empty();
    }

    size_type size() const noexcept
    {
        return m_values.size();
    }

    size_type max_size() const noexcept
    {
        return std::min(bucket_entry::max_size(), std::min(m_values.max_size(), m_buckets.max_size()));
    }

    // MODIFIERS

    void clear() noexcept
    {
        m_buckets.clear();
        m_values.clear();
    }

    template <typename P>
    std::pair<iterator, bool> insert(P&& value)
    {
        return insert_impl(KeySelect()(value), m_hash(KeySelect()(value)), std::forward<P>(value));
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last)
    {
        if (std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value)
        {
            const auto nb_elements_insert = std::distance(first, last);
            const std::size_t nb_free_buckets = bucket_count() - size();

            if (nb_elements_insert > 0 && nb_free_buckets < static_cast<std::size_t>(nb_elements_insert)) {
                reserve(size() + (nb_elements_insert - nb_free_buckets));
            }
        }

        for (; first != last; ++first) {
            insert(*first);
        }
    }

    template <typename... Args>
    std::pair<iterator,bool> emplace(Args&&... args)
    {
        return insert(value_type(std::forward<Args>(args)...));
    }

    iterator erase(iterator pos)
    {
        return erase(const_iterator(pos));
    }

    iterator erase(const_iterator pos)
    {
        assert(pos != cend());

        const std::size_t index_erase = iterator_to_index(pos);

        auto it_bucket = find_key(pos.key(), m_hash(pos.key()));
        assert(it_bucket != m_buckets.end());

        erase_value_from_bucket(it_bucket);

        // One element was removed from m_values,
        // due to the left shift the next element is now at the position
        // of the previous element.
        return begin() + index_erase;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last) {
            return get_mutable_iterator(first);
        }

        assert(std::distance(first, last) > 0 && std::distance(cbegin(), first) >= 0);
        const std::size_t start_index = static_cast<std::size_t>(std::distance(cbegin(), first));
        const std::size_t nb_values = static_cast<std::size_t>(std::distance(first, last));
        const std::size_t end_index = start_index + nb_values;

        // Delete all values
#ifdef TSL_NO_CONTAINER_ERASE_CONST_ITERATOR
        auto next_it = m_values.erase(get_mutable_iterator(first).m_iterator, get_mutable_iterator(last).m_iterator);
#else
        auto next_it = m_values.erase(first.m_iterator, last.m_iterator);
#endif

        /*
         *  Mark the buckets corresponding to the values as empty and do
         *  a backward shift.
         *
         *  Also, the erase operation on m_values has shifted all the
         *  values on the right of last.m_iterator.
         *  Adapt the indexes for these values.
         */
        for (std::size_t ibucket = 0; ibucket < m_buckets.size(); ibucket++) {
            if (!m_buckets[ibucket].has_index()) {
                continue;
            }

            if (m_buckets[ibucket].index() >= start_index && m_buckets[ibucket].index() < end_index) {
                m_buckets[ibucket].set_empty();
                backward_shift(ibucket);
            } else if (m_buckets[ibucket].index() >= end_index) {
                m_buckets[ibucket].set_index(m_buckets[ibucket].index() - static_cast<std::size_t>(nb_values));
            }
        }

        return iterator(next_it);
    }

    template <typename K>
    size_type erase(const K& key)
    {
        return erase_impl(key);
    }

    void swap(ordered_hash& other)
    {
        using std::swap;

        swap(m_buckets, other.m_buckets);
        swap(m_values, other.m_values);
        swap(m_mask, other.m_mask);
        swap(m_max_load_factor, other.m_max_load_factor);
        swap(m_load_threshold, other.m_load_threshold);
        swap(m_hash, other.m_hash);
        swap(m_key_equal, other.m_key_equal);
    }

    // LOOKUP

    template <typename K>
    size_type count(const K& key) const
    {
        if (find(key) == end()) {
            return 0;
        }
        else {
            return 1;
        }
    }

    template <typename K>
    iterator find(const K& key)
    {
        if (empty()) {
            return end();
        }

        auto it_bucket = find_key(key, m_hash(key));
        return (it_bucket != m_buckets.end())?begin() + it_bucket->index():end();
    }

    template <typename K>
    const_iterator find(const K& key) const
    {
        if (empty()) {
            return end();
        }

        auto it_bucket = find_key(key, m_hash(key));
        return (it_bucket != m_buckets.end())?begin() + it_bucket->index():end();
    }

    template <typename K>
    std::pair<iterator, iterator> equal_range(const K& key)
    {
        iterator it = find(key);
        return std::make_pair(it, it);
    }

    template <typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
        const_iterator it = find(key);
        return std::make_pair(it, it);
    }

    // BUCKETS

    size_type bucket_count() const
    {
        return m_buckets.size();
    }

    size_type max_bucket_count() const
    {
        return m_buckets.max_size();
    }

    // HASH POLICY

    float load_factor() const
    {
        return static_cast<float>(size())/static_cast<float>(bucket_count());
    }

    float max_load_factor() const
    {
        return m_max_load_factor;
    }

    void max_load_factor(float ml)
    {
        m_max_load_factor = ml;
        m_load_threshold = static_cast<size_type>(static_cast<float>(bucket_count())*m_max_load_factor);
    }

    void rehash(size_type count)
    {
        count = std::max(count, static_cast<size_type>(std::ceil(static_cast<float>(size())/max_load_factor())));
        rehash_impl(count);
    }

    void reserve(size_type count)
    {
        count = static_cast<size_type>(std::ceil(static_cast<float>(count)/max_load_factor()));
        reserve_space_for_values(count);
        rehash(count);
    }

    // OBSERVERS

    hasher hash_function() const
    {
        return m_hash;
    }

    key_equal key_eq() const
    {
        return m_key_equal;
    }

    // OTHER

    iterator get_mutable_iterator(const_iterator pos)
    {
        return begin() + iterator_to_index(pos);
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<!std::is_same<U, void>::value>::type* = nullptr>
    typename U::value_type& at(const K& key)
    {
        return const_cast<typename U::value_type&>(static_cast<const ordered_hash*>(this)->at(key));
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<!std::is_same<U, void>::value>::type* = nullptr>
    const typename U::value_type& at(const K& key) const
    {
        auto it = find(key);
        if (it != end()) {
            return it.value();
        }
        else {
            throw std::out_of_range("Couldn't find the key.");
        }
    }


    template <typename K, typename U = ValueSelect, typename std::enable_if<!std::is_same<U, void>::value>::type* = nullptr>
    typename U::value_type& operator[](K&& key)
    {
        using T = typename U::value_type;

        auto it = find(key);
        if (it != end()) {
            return it.value();
        }
        else {
            return insert(std::make_pair(std::forward<K>(key), T())).first.value();
        }
    }

    const_reference front() const
    {
        return m_values.front();
    }

    const_reference back() const
    {
        return m_values.back();
    }

    template <typename U = values_container_type, typename std::enable_if<is_vector<U>::value>::type* = nullptr>
    const typename values_container_type::value_type* data() const noexcept
    {
        return m_values.data();
    }

    const values_container_type& values_container() const noexcept
    {
        return m_values;
    }

    template <typename U = values_container_type, typename std::enable_if<is_vector<U>::value>::type* = nullptr>
    size_type capacity() const noexcept
    {
        return m_values.capacity();
    }

    void shrink_to_fit()
    {
        m_values.shrink_to_fit();
    }

    void pop_back()
    {
        if (empty()) {
            return;
        }

        erase(std::prev(end()));
    }


    iterator unordered_erase(iterator pos)
    {
        return unordered_erase(const_iterator(pos));
    }

    iterator unordered_erase(const_iterator pos)
    {
        const std::size_t index_erase = iterator_to_index(pos);
        unordered_erase(pos.key());

        // One element was deleted, index_erase now point to the
        // next element
        return begin() + index_erase;
    }

    template <typename K>
    size_type unordered_erase(const K& key)
    {
        if (empty()) {
            return 0;
        }

        auto it_bucket_key = find_key(key, m_hash(key));
        if (it_bucket_key == m_buckets.end()) {
            return 0;
        }

        auto it_bucket_last_elem = find_key(KeySelect()(back()), m_hash(KeySelect()(back())));
        assert(it_bucket_last_elem != m_buckets.end());
        assert(it_bucket_last_elem->has_index());
        assert(it_bucket_last_elem->index() == m_values.size() - 1);


        std::swap(m_values[it_bucket_key->index()], m_values[it_bucket_last_elem->index()]);

        const std::size_t tmp_index = it_bucket_key->index();
        it_bucket_key->set_index(it_bucket_last_elem->index());
        it_bucket_last_elem->set_index(tmp_index);

        erase_value_from_bucket(it_bucket_key);

        return 1;
    }

    friend bool operator==(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values == rhs.m_values;
    }

    friend bool operator!=(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values != rhs.m_values;
    }

    friend bool operator<(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values < rhs.m_values;
    }

    friend bool operator<=(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values <= rhs.m_values;
    }

    friend bool operator>(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values > rhs.m_values;
    }

    friend bool operator>=(const ordered_hash& lhs, const ordered_hash& rhs)
    {
        return lhs.m_values >= rhs.m_values;
    }

private:
    template <typename K>
    typename buckets_container_type::iterator find_key(const K& key, std::size_t hash)
    {
        auto it = static_cast<const ordered_hash*>(this)->find_key(key, hash);
        return m_buckets.begin() + std::distance(m_buckets.cbegin(), it);
    }

    /**
     *  Return bucket which has the key 'key' or m_buckets.end() if none.
     */
    template <typename K>
    typename buckets_container_type::const_iterator find_key(const K& key, std::size_t hash) const
    {
        assert(size() < m_buckets.size());
        const auto truncated_hash = bucket_entry::truncate_hash(hash);

        for (std::size_t ibucket = bucket_for_hash(hash), iprobe = 0; ; ibucket = next_probe(ibucket), ++iprobe) {
            if (m_buckets[ibucket].empty()) {
                return m_buckets.end();
            } else if (m_buckets[ibucket].truncated_hash() == truncated_hash &&
                    m_key_equal(key, KeySelect()(m_values[m_buckets[ibucket].index()]))) {
                return m_buckets.begin() + ibucket;
            } else if (iprobe > dist_from_initial_bucket(ibucket)) {
                return m_buckets.end();
            }
        }
    }

    void rehash_impl(size_type count)
    {
        count = round_up_to_power_of_two(count);
        if (count > max_size()) {
            throw std::length_error("The map exceed its maxmimum size.");
        }

        buckets_container_type old_buckets(count);
        m_buckets.swap(old_buckets);

        this->max_load_factor(m_max_load_factor);
        m_mask = this->bucket_count() - 1;


        for (const bucket_entry& old_bucket: old_buckets) {
            if (!old_bucket.has_index()) {
                continue;
            }

            const auto insert_hash = old_bucket.truncated_hash();
            const auto insert_index = old_bucket.index();

            for (std::size_t ibucket = bucket_for_hash(insert_hash), iprobe = 0; ; ibucket = next_probe(ibucket), ++iprobe) {
                if (m_buckets[ibucket].empty()) {
                    m_buckets[ibucket].set_index(insert_index);
                    m_buckets[ibucket].set_hash(insert_hash);
                    break;
                }

                assert(m_buckets[ibucket].has_index());
                const std::size_t distance = dist_from_initial_bucket(ibucket);

                if (iprobe > distance) {
                    const auto tmp_index = m_buckets[ibucket].index();
                    const auto tmp_hash = m_buckets[ibucket].truncated_hash();

                    m_buckets[ibucket].set_index(insert_index);
                    m_buckets[ibucket].set_hash(insert_hash);

                    insert_with_robin_hood_swap(next_probe(ibucket), distance + 1, tmp_index, tmp_hash);
                    break;
                }
            }
        }
    }

    template <typename T = values_container_type, typename std::enable_if<is_vector<T>::value>::type* = nullptr>
    void reserve_space_for_values(size_type count)
    {
        m_values.reserve(count);
    }

    template <typename T = values_container_type, typename std::enable_if<!is_vector<T>::value>::type* = nullptr>
    void reserve_space_for_values(size_type /*count*/)
    {}

    /**
     *  Swap the empty bucket with the values on its right until we
     *  cross another empty bucket or if the other bucket has a
     *  dist_from_initial_bucket == 0.
     */
    void backward_shift(std::size_t empty_ibucket)
    {
        assert(m_buckets[empty_ibucket].empty());

        std::size_t previous_ibucket = empty_ibucket;
        for (std::size_t current_ibucket = next_probe(previous_ibucket);
            !m_buckets[current_ibucket].empty() && dist_from_initial_bucket(current_ibucket) > 0;
            previous_ibucket = current_ibucket, current_ibucket = next_probe(current_ibucket))
        {
            std::swap(m_buckets[current_ibucket], m_buckets[previous_ibucket]);
        }
    }

    void erase_value_from_bucket(typename buckets_container_type::iterator it_bucket)
    {
        assert(it_bucket != m_buckets.end() && it_bucket->has_index());

        m_values.erase(m_values.begin() + it_bucket->index());

        const std::size_t index_deleted = it_bucket->index();

        // m_values.erase shifted all the values on the right of the
        // erased value, shift the indexes except if it was the last
        // value
        if (index_deleted != m_values.size()) {
            for (auto& bucket: m_buckets) {
                if (bucket.has_index() && bucket.index() > index_deleted) {
                    bucket.set_index(bucket.index() - 1);
                }
            }
        }

        // Mark the bucket as empty and do a backward shift of the
        // values on the right
        it_bucket->set_empty();
        backward_shift(static_cast<std::size_t>(std::distance(m_buckets.begin(), it_bucket)));
    }

    template <typename K>
    size_type erase_impl(const K& key)
    {
        if (empty()) {
            return 0;
        }

        auto it_bucket = find_key(key, m_hash(key));
        if (it_bucket != m_buckets.end()) {
            erase_value_from_bucket(it_bucket);

            return 1;
        } else {
            return 0;
        }
    }

    /**
     *  From ibucket, search for an empty bucket to store the
     *  insert_index an the insert_hash.
     *
     *  If on the way we find a bucket with a value which is further
     *  away from its initial bucket than our current probing, swap
     *  the indexes and the hashes and continue the search for an empty
     *  bucket to store this new index and hash while continuing the
     *  swapping process.
     */
    void insert_with_robin_hood_swap(std::size_t ibucket, std::size_t iprobe,
        typename bucket_entry::index_type insert_index,
        typename bucket_entry::truncated_hash_type insert_hash)
    {
        while (true) {
            if (m_buckets[ibucket].empty()) {
                m_buckets[ibucket].set_index(insert_index);
                m_buckets[ibucket].set_hash(insert_hash);

                return;
            }

            assert(m_buckets[ibucket].has_index());
            const std::size_t distance = dist_from_initial_bucket(ibucket);
            if (iprobe > distance) {
                const auto tmp_index = m_buckets[ibucket].index();
                const auto tmp_hash = m_buckets[ibucket].truncated_hash();

                m_buckets[ibucket].set_index(insert_index);
                m_buckets[ibucket].set_hash(insert_hash);

                insert_index = tmp_index;
                insert_hash = tmp_hash;

                iprobe = distance;
            }

            ibucket = next_probe(ibucket);
            ++iprobe;
        }
    }

    std::size_t dist_from_initial_bucket(std::size_t ibucket) const
    {
        const std::size_t initial_bucket = bucket_for_hash(m_buckets[ibucket].truncated_hash());

        // If the bucket is smaller than the initial bucket for the
        // value, there was a wrapping at the end of the bucket array
        // due to the modulo.
        if (ibucket < initial_bucket) {
            return (bucket_count() + ibucket) - initial_bucket;
        } else {
            return ibucket - initial_bucket;
        }
    }


    template <typename K, typename P>
    std::pair<iterator, bool> insert_impl(const K& key, std::size_t hash, P&& value)
    {
        resize_if_needed(1);

        for (std::size_t ibucket = bucket_for_hash(hash), iprobe = 0; ; ibucket = next_probe(ibucket), ++iprobe) {
            if (m_buckets[ibucket].empty()) {
                m_values.emplace_back(std::forward<P>(value));
                m_buckets[ibucket].set_index(m_values.size() - 1);
                m_buckets[ibucket].set_hash(hash);

                return std::make_pair(std::prev(end()), true);
            } else if (m_buckets[ibucket].truncated_hash() == bucket_entry::truncate_hash(hash) &&
                    m_key_equal(key, KeySelect()(m_values[m_buckets[ibucket].index()]))) {
                return std::make_pair(begin() + m_buckets[ibucket].index(), false);
            } else if (rehash_on_high_nb_probes(iprobe)) {
                return insert_impl(key, hash, std::forward<P>(value));
            } else {
                const std::size_t distance = dist_from_initial_bucket(ibucket);

                if (iprobe > distance) {
                    m_values.emplace_back(std::forward<P>(value));

                    // Propagate the index and the hash of the current
                    // bucket to a more far away bucket. Clear the current
                    // bucket so we can use it to insert the key.
                    insert_with_robin_hood_swap(next_probe(ibucket), distance + 1,
                                                m_buckets[ibucket].index(), m_buckets[ibucket].truncated_hash());

                    m_buckets[ibucket].set_index(m_values.size() - 1);
                    m_buckets[ibucket].set_hash(hash);

                    return std::make_pair(std::prev(end()), true);
                }
            }
        }
    }


    void resize_if_needed(std::size_t delta)
    {
        if (size() + delta >= m_load_threshold) {
            rehash_impl(m_buckets.size() * REHASH_SIZE_MULTIPLICATION_FACTOR);
        }
    }


    std::size_t next_probe(std::size_t index) const
    {
        return (index + 1) & m_mask;
    }

    std::size_t bucket_for_hash(std::size_t hash) const
    {
        return hash & m_mask;
    }

    std::size_t iterator_to_index(const_iterator it) const
    {
        const auto dist = std::distance(cbegin(), it);
        assert(dist >= 0);

        return static_cast<std::size_t>(dist);
    }

    static std::size_t round_up_to_power_of_two(std::size_t value)
    {
        std::size_t power = 1;
        while (power < value) {
            power <<= 1;
        }

        return power;
    }

public:
    static const size_type DEFAULT_INIT_BUCKETS_SIZE = 16;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 0.95f;
    static const size_type REHASH_SIZE_MULTIPLICATION_FACTOR = 2;

    static const size_type REHASH_ON_HIGH_NB_PROBES__NPROBES = 8;
    static constexpr float REHASH_ON_HIGH_NB_PROBES__MIN_LOAD_FACTOR = 0.5f;

    bool rehash_on_high_nb_probes(std::size_t nb_probes)
    {
        if (nb_probes == REHASH_ON_HIGH_NB_PROBES__NPROBES && load_factor() >= REHASH_ON_HIGH_NB_PROBES__MIN_LOAD_FACTOR) {
            rehash_impl(m_buckets.size() * REHASH_SIZE_MULTIPLICATION_FACTOR);
            return true;
        } else {
            return false;
        }
    }

private:
    buckets_container_type m_buckets;
    values_container_type m_values;

    std::size_t m_mask;

    float m_max_load_factor;
    size_type m_load_threshold;

    Hash m_hash;
    KeyEqual m_key_equal;
};

}   /* detail_ordered_hash */

PYCPP_END_NAMESPACE
