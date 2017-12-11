//  :copyright: (c) 2017 Tessil.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fast hashmap using robin hood indexing.
 *
 *  Provides the implementation of the underlying `robin_map`
 *  and `robin_set` classes, fast, STL-like hashmaps.
 *
 *  See benchmarks for a comparison to other well-known hashmap
 *  implementations:
 *      https://tessil.github.io/2016/08/29/benchmark-hopscotch-map.html
 */

#pragma once

#include <pycpp/stl/exception.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/ratio.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/tuple.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>

PYCPP_BEGIN_NAMESPACE

namespace robin_detail
{
// DETAIL
// ------

// GROWTH

/**
 * Grow the hash table by a factor of GrowthFactor keeping the bucket count to a power of two. It allows
 * the table to use a mask operation instead of a modulo operation to map a hash to a bucket.
 *
 * GrowthFactor must be a power of two >= 2.
 */
template <size_t GrowthFactor>
class power_of_two_growth_policy
{
public:
    /**
     *  Called on the hash table creation and on rehash. The number of
     *  buckets for the table is passed in parameter. This number is a
     *  minimum, the policy may update this value with a higher value
     *  if needed (but not lower).
     */
    power_of_two_growth_policy(size_t& min_bucket_count_in_out)
    {
        if (min_bucket_count_in_out > max_bucket_count()) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        static_assert(MIN_BUCKETS_SIZE > 0, "MIN_BUCKETS_SIZE must be > 0.");
        size_t min_bucket_count = MIN_BUCKETS_SIZE;

        min_bucket_count_in_out = std::max(min_bucket_count, min_bucket_count_in_out);
        min_bucket_count_in_out = round_up_to_power_of_two(min_bucket_count_in_out);
        m_mask = min_bucket_count_in_out - 1;
    }

    /**
     *  Return the bucket [0, bucket_count()) to which the hash belongs.
     */
    size_t bucket_for_hash(size_t hash) const noexcept
    {
        return hash & m_mask;
    }

    /**
     *  Return the bucket count to use when the bucket array grows on rehash.
     */
    size_t next_bucket_count() const
    {
        if ((m_mask + 1) > max_bucket_count() / GrowthFactor) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        return (m_mask + 1) * GrowthFactor;
    }

    /**
     *  Return the maximum number of buckets supported by the policy.
     */
    size_t max_bucket_count() const
    {
        // Largest power of two.
        return (std::numeric_limits<size_t>::max() / 2) + 1;
    }

private:
    static size_t round_up_to_power_of_two(size_t value)
    {
        if (is_power_of_two(value)) {
            return value;
        }

        if (value == 0) {
            return 1;
        }

        --value;
        for(size_t i = 1; i < sizeof(size_t) * CHAR_BIT; i *= 2) {
            value |= value >> i;
        }

        return value + 1;
    }

    static constexpr bool is_power_of_two(size_t value)
    {
        return value != 0 && (value & (value - 1)) == 0;
    }

protected:
    static const size_t MIN_BUCKETS_SIZE = 2;
    static_assert(is_power_of_two(GrowthFactor) && GrowthFactor >= 2, "GrowthFactor must be a power of two >= 2.");

    size_t m_mask;
};


/**
 *  Grow the hash table by GrowthFactor::num / GrowthFactor::den and
 *  use a modulo to map a hash to a bucket. Slower but it can be useful
 *  if you want a slower growth.
 */
template <typename GrowthFactor = std::ratio<3, 2>>
class mod_growth_policy
{
public:
    mod_growth_policy(size_t& min_bucket_count_in_out)
    {
        if (min_bucket_count_in_out > max_bucket_count()) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        static_assert(MIN_BUCKETS_SIZE > 0, "MIN_BUCKETS_SIZE must be > 0.");
        size_t min_bucket_count = MIN_BUCKETS_SIZE;

        min_bucket_count_in_out = std::max(min_bucket_count, min_bucket_count_in_out);
        m_bucket_count = min_bucket_count_in_out;
    }

    size_t bucket_for_hash(size_t hash) const noexcept
    {
        return hash % m_bucket_count;
    }

    size_t next_bucket_count() const
    {
        if (m_bucket_count == max_bucket_count()) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        double next_bucket_count = std::ceil(double(m_bucket_count) * REHASH_SIZE_MULTIPLICATION_FACTOR);
        if (!std::isnormal(next_bucket_count)) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        if (next_bucket_count > double(max_bucket_count())) {
            return max_bucket_count();
        } else {
            return size_t(next_bucket_count);
        }
    }

    size_t max_bucket_count() const {
        return MAX_BUCKET_COUNT;
    }

private:
    static const size_t MIN_BUCKETS_SIZE = 2;
    static constexpr double REHASH_SIZE_MULTIPLICATION_FACTOR = 1.0 * GrowthFactor::num / GrowthFactor::den;
    static const size_t MAX_BUCKET_COUNT = size_t(double(std::numeric_limits<size_t>::max() / REHASH_SIZE_MULTIPLICATION_FACTOR));

    static_assert(REHASH_SIZE_MULTIPLICATION_FACTOR >= 1.1, "Growth factor should be >= 1.1.");

    size_t m_bucket_count;
};


static constexpr const std::array<size_t, 39> PRIMES = {{
    5ul, 17ul, 29ul, 37ul, 53ul, 67ul, 79ul, 97ul, 131ul, 193ul, 257ul, 389ul, 521ul, 769ul, 1031ul, 1543ul, 2053ul,
    3079ul, 6151ul, 12289ul, 24593ul, 49157ul, 98317ul, 196613ul, 393241ul, 786433ul, 1572869ul, 3145739ul,
    6291469ul, 12582917ul, 25165843ul, 50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
}};

template <unsigned int IPrime>
static constexpr size_t mod(size_t hash)
{
    return hash % PRIMES[IPrime];
}

//  MOD_PRIME[iprime](hash) returns hash % PRIMES[iprime]. This table
//  allows for faster modulo as the compiler can optimize the modulo
//  code better with a constant known at the compilation.
static constexpr const std::array<size_t(*)(size_t), 39> MOD_PRIME = {{
    &mod<0>, &mod<1>, &mod<2>, &mod<3>, &mod<4>, &mod<5>, &mod<6>, &mod<7>, &mod<8>, &mod<9>, &mod<10>,
    &mod<11>, &mod<12>, &mod<13>, &mod<14>, &mod<15>, &mod<16>, &mod<17>, &mod<18>, &mod<19>, &mod<20>,
    &mod<21>, &mod<22>, &mod<23>, &mod<24>, &mod<25>, &mod<26>, &mod<27>, &mod<28>, &mod<29>, &mod<30>,
    &mod<31>, &mod<32>, &mod<33>, &mod<34>, &mod<35>, &mod<36>, &mod<37> , &mod<38>
}};

/**
 *  Grow the hash table by using prime numbers as bucket count. Slower
 *  than power_of_two_growth_policy in general but will probably
 *  distribute the values around better in the buckets with a poor hash
 *  function.
 *
 *  To allow the compiler to optimize the modulo operation, a lookup
 *  table is used with constant primes numbers.
 *
 *  With a switch the code would look like:
 *  \code
 *      switch(iprime) { // iprime is the current prime of the hash table
 *          case 0: hash % 5ul;
 *              break;
 *          case 1: hash % 17ul;
 *              break;
 *          case 2: hash % 29ul;
 *              break;
 *          ...
 *      }
 *  \endcode
 *
 *  Due to the constant variable in the modulo the compiler is able to
 *  optimize the operation by a series of multiplications, substractions
 *  and shifts.
 *
 *  The 'hash % 5' could become something like 'hash - (hash *
 *  0xCCCCCCCD) >> 34) * 5' in a 64 bits environement.
 */
class prime_growth_policy
{
public:
    prime_growth_policy(size_t& min_bucket_count_in_out)
    {
        auto it_prime = std::lower_bound(PRIMES.begin(), PRIMES.end(), min_bucket_count_in_out);
        if (it_prime == PRIMES.end()) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        m_iprime = static_cast<unsigned int>(std::distance(PRIMES.begin(), it_prime));
        min_bucket_count_in_out = *it_prime;
    }

    size_t bucket_for_hash(size_t hash) const noexcept
    {
        return MOD_PRIME[m_iprime](hash);
    }

    size_t next_bucket_count() const
    {
        if (m_iprime + 1 >= PRIMES.size()) {
            throw std::length_error("The hash table exceeds its maxmimum size.");
        }

        return PRIMES[m_iprime + 1];
    }

    size_t max_bucket_count() const
    {
        return PRIMES.back();
    }

private:
    unsigned int m_iprime;

    static_assert(std::numeric_limits<decltype(m_iprime)>::max() >= PRIMES.size(),
                  "The type of m_iprime is not big enough.");
};

// HASH

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

template <typename U>
struct is_power_of_two_policy: std::false_type
{};

template <size_t GrowthFactor>
struct is_power_of_two_policy<power_of_two_growth_policy<GrowthFactor>>: std::true_type
{};

using truncated_hash_type = std::uint_least32_t;


/**
 *  Helper class that store a truncated hash if StoreHash is
 *  true and nothing otherwise.
 */
template <bool StoreHash>
class bucket_entry_hash
{
public:
    bool bucket_hash_equal(size_t /*hash*/) const noexcept
    {
        return true;
    }

    truncated_hash_type truncated_hash() const noexcept
    {
        return 0;
    }

protected:
    void set_hash(truncated_hash_type /*hash*/) noexcept
    {}
};


template <>
class bucket_entry_hash<true>
{
public:
    bool bucket_hash_equal(size_t hash) const noexcept
    {
        return m_hash == truncated_hash_type(hash);
    }

    truncated_hash_type truncated_hash() const noexcept
    {
        return m_hash;
    }

protected:
    void set_hash(truncated_hash_type hash) noexcept
    {
        m_hash = truncated_hash_type(hash);
    }

private:
    truncated_hash_type m_hash;
};


/**
 *  Each bucket entry has:
 *  - A value of type `ValueType`.
 *  - An integer to store how far the value of the bucket, if any, is
 *    from its ideal bucket
 *    (ex: if the current bucket 5 has the value 'foo' and `hash('foo')
 *    % nb_buckets` == 3, `dist_from_ideal_bucket()` will return 2 as
 *    the current value of the bucket is two buckets away from its
 *    ideal bucket)
 *    If there is no value in the bucket (i.e. `empty()` is true)
 *    `dist_from_ideal_bucket()` will be < 0.
 *  - A marker which tells us if the bucket is the last bucket of the
 *    bucket array (useful for the iterator of the hash table).
 *  - If `StoreHash` is true, 32 bits of the hash of the value, if any,
 *    are also stored in the bucket.  If the size of the hash is more
 *    than 32 bits, it is truncated. We don't store the full hash
 *    as storing the hash is a potential opportunity to use the unused
 *    space due to the alignement of the bucket_entry structure. We
 *    can thus potentially store the hash without any extra space
 *    (which would not be possible with 64 bits of the hash).
 */
template <typename ValueType, bool StoreHash>
class bucket_entry: public bucket_entry_hash<StoreHash>
{
    using bucket_hash = bucket_entry_hash<StoreHash>;

public:
    using value_type = ValueType;
    using distance_type = std::int_least16_t;

    bucket_entry() noexcept:
        bucket_hash(),
        m_dist_from_ideal_bucket(EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET),
        m_last_bucket(false)
    {
        assert(empty());
    }

    bucket_entry(const bucket_entry& other) noexcept(std::is_nothrow_copy_constructible<value_type>::value):
        bucket_hash(other),
        m_dist_from_ideal_bucket(EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET),
        m_last_bucket(other.m_last_bucket)
    {
        if (!other.empty()) {
            ::new (static_cast<void*>(std::addressof(m_value))) value_type(other.value());
            m_dist_from_ideal_bucket = other.m_dist_from_ideal_bucket;
        }
    }

    /**
     *  Never really used, but still necessary as we must call resize
     *  on an empty `std::vector<bucket_entry>`. and we need to support
     *  move-only types. See robin_hash constructor for details.
     */
    bucket_entry(bucket_entry&& other) noexcept(std::is_nothrow_move_constructible<value_type>::value):
        bucket_hash(std::move(other)),
        m_dist_from_ideal_bucket(EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET),
        m_last_bucket(other.m_last_bucket)
    {
        if (!other.empty()) {
            ::new (static_cast<void*>(std::addressof(m_value))) value_type(std::move(other.value()));
            m_dist_from_ideal_bucket = other.m_dist_from_ideal_bucket;
        }
    }

    bucket_entry& operator=(const bucket_entry& other) noexcept(std::is_nothrow_copy_constructible<value_type>::value)
    {
        if (this != &other) {
            clear();

            bucket_hash::operator=(other);
            if (!other.empty()) {
                ::new (static_cast<void*>(std::addressof(m_value))) value_type(other.value());
            }

            m_dist_from_ideal_bucket = other.m_dist_from_ideal_bucket;
            m_last_bucket = other.m_last_bucket;
        }

        return *this;
    }

    bucket_entry& operator=(bucket_entry&& ) = delete;

    ~bucket_entry() noexcept
    {
        clear();
    }

    void clear() noexcept
    {
        if (!empty()) {
            destroy_value();
            m_dist_from_ideal_bucket = EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET;
        }
    }

    bool empty() const noexcept
    {
        return m_dist_from_ideal_bucket == EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET;
    }

    value_type& value() noexcept
    {
        assert(!empty());
        return *reinterpret_cast<value_type*>(std::addressof(m_value));
    }

    const value_type& value() const noexcept
    {
        assert(!empty());
        return *reinterpret_cast<const value_type*>(std::addressof(m_value));
    }

    distance_type dist_from_ideal_bucket() const noexcept
    {
        return m_dist_from_ideal_bucket;
    }

    bool last_bucket() const noexcept
    {
        return m_last_bucket;
    }

    void set_as_last_bucket() noexcept
    {
        m_last_bucket = true;
    }

    template <typename... Args>
    void set_value_of_empty_bucket(distance_type dist_from_ideal_bucket,
                                   truncated_hash_type hash, Args&&... value_type_args)
    {
        assert(dist_from_ideal_bucket >= 0);
        assert(empty());

        ::new (static_cast<void*>(std::addressof(m_value))) value_type(std::forward<Args>(value_type_args)...);
        this->set_hash(hash);
        m_dist_from_ideal_bucket = dist_from_ideal_bucket;

        assert(!empty());
    }

    void swap_with_value_in_bucket(distance_type& dist_from_ideal_bucket,
                                   truncated_hash_type& hash, value_type& value)
    {
        assert(!empty());

        std::swap(value, this->value());
        std::swap(dist_from_ideal_bucket, m_dist_from_ideal_bucket);

        // Avoid warning of unused variable if StoreHash is false
        (void) hash;
        if (StoreHash) {
            const truncated_hash_type tmp_hash = this->truncated_hash();
            this->set_hash(hash);
            hash = tmp_hash;
        }
    }

    static truncated_hash_type truncate_hash(size_t hash) noexcept
    {
        return truncated_hash_type(hash);
    }

private:
    void destroy_value() noexcept
    {
        assert(!empty());
        value().~value_type();
    }

private:
    using storage = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    static const distance_type EMPTY_MARKER_DIST_FROM_IDEAL_BUCKET = -1;

    distance_type m_dist_from_ideal_bucket;
    bool m_last_bucket;
    storage m_value;
};


/**
 * Internal common class used by `robin_map` and `robin_set`.
 *
 * ValueType is what will be stored by `robin_hash` (usually `std::pair<Key, T>` for map and `Key` for set).
 *
 * `KeySelect` should be a `FunctionObject` which takes a `ValueType` in parameter and returns a
 *  reference to the key.
 *
 * `ValueSelect` should be a `FunctionObject` which takes a `ValueType` in parameter and returns a
 *  reference to the value. `ValueSelect` should be void if there is no value (in a set for example).
 *
 * The strong exception guarantee only holds if the expression
 * `std::is_nothrow_swappable<ValueType>::value && std::is_nothrow_move_constructible<ValueType>::value` is true.
 *
 * Behaviour is undefined if the destructor of `ValueType` throws.
 */
template <
    typename ValueType,
    typename KeySelect,
    typename ValueSelect,
    typename Hash,
    typename KeyEqual,
    typename Allocator,
    bool StoreHash,
    typename GrowthPolicy
>
class robin_hash: private Hash, private KeyEqual, private GrowthPolicy
{
private:
    template <typename U>
    using has_mapped_type = typename std::integral_constant<bool, !std::is_same<U, void>::value>;

public:
    template <bool IsConst>
    class robin_iterator;

    using key_type = typename KeySelect::key_type;
    using value_type = ValueType;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = robin_iterator<false>;
    using const_iterator = robin_iterator<true>;

private:
    /**
     *  Either store the hash because we are asked by the `StoreHash`
     *  template parameter or store the hash because it doesn't cost us
     *  anything in size and can be used to speed up rehash.
     */
    static constexpr bool STORE_HASH = StoreHash || ((sizeof(bucket_entry<value_type, true>) == sizeof(bucket_entry<value_type, false>)) &&
                                                     (sizeof(size_t) == sizeof(truncated_hash_type) || is_power_of_two_policy<GrowthPolicy>::value) &&
                                                     // Don't store the hash for primitive types with default hash.
                                                     (!std::is_arithmetic<key_type>::value || !std::is_same<Hash, std::hash<key_type>>::value)
                                                     );
    /**
     *  Only use the stored hash on lookup if we are explictly asked.
     *  We are not sure how slow the KeyEqual operation is. An extra
     *  comparison may slow things down with a fast KeyEqual.
     */
    static constexpr bool USE_STORED_HASH_ON_LOOKUP = StoreHash;

    /**
     *  We can only use the hash on rehash if the size of the hash type
     *  is the same as the stored one or if we use a power of two modulo.
     *  In the case of the power of two modulo, we just mask the least
     *  significant bytes, we just have to check that the
     *  truncated_hash_type didn't truncated more bytes.
     */
    static bool use_stored_hash_on_rehash(size_type bucket_count)
    {
        (void) bucket_count;
        if (STORE_HASH && sizeof(size_t) == sizeof(truncated_hash_type)) {
            return true;
        } else if (STORE_HASH && is_power_of_two_policy<GrowthPolicy>::value) {
            assert(bucket_count > 0);
            return (bucket_count - 1) <= std::numeric_limits<truncated_hash_type>::max();
        } else {
            return false;
        }
    }

    using bucket_entry_type = bucket_entry<value_type, STORE_HASH>;
    using distance_type = typename bucket_entry_type::distance_type;
    using buckets_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<bucket_entry_type>;
    using buckets_container_type = std::vector<bucket_entry_type, buckets_allocator>;

public:
    /**
     *  The 'operator*()' and 'operator->()' methods return a const
     *  reference and const pointer respectively to the stored value
     *  type.
     *
     *  In case of a map, to get a mutable reference to the value
     *  associated to a key (the '.second' in the stored pair), you
     *  have to call 'value()'.
     *
     *  The main reason for this is that if we returned a
     *  `std::pair<Key, T>&` instead of a `const std::pair<Key, T>&`,
     *  the user may modify the key which will put the map in a
     *  undefined state.
     */
    template <bool IsConst>
    class robin_iterator
    {
        friend class robin_hash;

    private:
        using iterator_bucket = typename std::conditional<
            IsConst,
            typename buckets_container_type::const_iterator,
            typename buckets_container_type::iterator
        >::type;

        robin_iterator(iterator_bucket it) noexcept:
            m_iterator(it)
        {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const typename robin_hash::value_type;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;

        robin_iterator() noexcept
        {}

        robin_iterator(const robin_iterator<false>& other) noexcept:
            m_iterator(other.m_iterator)
        {}

        const typename robin_hash::key_type& key() const
        {
            return KeySelect()(m_iterator->value());
        }

        template <typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value && IsConst>::type* = nullptr>
        const typename U::value_type& value() const
        {
            return U()(m_iterator->value());
        }

        template <typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value && !IsConst>::type* = nullptr>
        typename U::value_type& value()
        {
            return U()(m_iterator->value());
        }

        reference operator*() const
        {
            return m_iterator->value();
        }

        pointer operator->() const
        {
            return std::addressof(m_iterator->value());
        }

        robin_iterator& operator++()
        {
            while (true) {
                if (m_iterator->last_bucket()) {
                    ++m_iterator;
                    return *this;
                }

                ++m_iterator;
                if (!m_iterator->empty()) {
                    return *this;
                }
            }
        }

        robin_iterator operator++(int)
        {
            robin_iterator tmp(*this);
            ++*this;

            return tmp;
        }

        friend bool operator==(const robin_iterator& lhs, const robin_iterator& rhs)
        {
            return lhs.m_iterator == rhs.m_iterator;
        }

        friend bool operator!=(const robin_iterator& lhs, const robin_iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        iterator_bucket m_iterator;
    };

public:
    robin_hash(size_type bucket_count, const Hash& hash, const KeyEqual& equal, const Allocator& alloc, float max_load_factor):
        Hash(hash),
        KeyEqual(equal),
        // We need a non-zero bucket_count
        GrowthPolicy(bucket_count == 0?++bucket_count:bucket_count),
        m_buckets(alloc),
        m_bucket_count(bucket_count),
        m_nb_elements(0),
        m_grow_on_next_insert(false)
    {
        if (bucket_count > max_bucket_count()) {
            throw std::length_error("The map exceeds its maxmimum size.");
        }

        /*
         *  We can't use the `vector(size_type count, const Allocator& alloc)`
         *  constructor as it's only available in C++14 and we need to
         *  support C++11. We thus must resize after using the
         *  `vector(const Allocator& alloc)` constructor.
         *
         *  We can't use `vector(size_type count, const T& value,
         *  const Allocator& alloc)` as it requires the value T to be
         *  copyable.
         */
        m_buckets.resize(m_bucket_count);

        assert(!m_buckets.empty());
        m_buckets.back().set_as_last_bucket();

        this->max_load_factor(max_load_factor);
    }

    robin_hash(const robin_hash& other) = default;

    robin_hash(robin_hash&& other) noexcept(std::is_nothrow_move_constructible<Hash>::value &&
                                            std::is_nothrow_move_constructible<KeyEqual>::value &&
                                            std::is_nothrow_move_constructible<GrowthPolicy>::value &&
                                            std::is_nothrow_move_constructible<buckets_container_type>::value):
        Hash(std::move(static_cast<Hash&>(other))),
        KeyEqual(std::move(static_cast<KeyEqual&>(other))),
        GrowthPolicy(std::move(static_cast<GrowthPolicy&>(other))),
        m_buckets(std::move(other.m_buckets)),
        m_bucket_count(other.m_bucket_count),
        m_nb_elements(other.m_nb_elements),
        m_load_threshold(other.m_load_threshold),
        m_max_load_factor(other.m_max_load_factor),
        m_grow_on_next_insert(other.m_grow_on_next_insert)
    {
        other.clear();
    }

    robin_hash& operator=(const robin_hash& other) = default;

    robin_hash& operator=(robin_hash&& other)
    {
        other.swap(*this);
        other.clear();

        return *this;
    }

    allocator_type get_allocator() const
    {
        return m_buckets.get_allocator();
    }

    // ITERATORS

    iterator begin() noexcept
    {
        auto begin = m_buckets.begin();
        while (begin != m_buckets.end() && begin->empty()) {
            ++begin;
        }

        return iterator(begin);
    }

    const_iterator begin() const noexcept
    {
        return cbegin();
    }

    const_iterator cbegin() const noexcept
    {
        auto begin = m_buckets.cbegin();
        while (begin != m_buckets.cend() && begin->empty()) {
            ++begin;
        }

        return const_iterator(begin);
    }

    iterator end() noexcept
    {
        return iterator(m_buckets.end());
    }

    const_iterator end() const noexcept
    {
        return cend();
    }

    const_iterator cend() const noexcept
    {
        return const_iterator(m_buckets.cend());
    }

    // CAPACITY

    bool empty() const noexcept
    {
        return m_nb_elements == 0;
    }

    size_type size() const noexcept
    {
        return m_nb_elements;
    }

    size_type max_size() const noexcept {
        return m_buckets.max_size();
    }

    // MODIFIERS

    void clear() noexcept
    {
        for (auto& bucket: m_buckets) {
            bucket.clear();
        }

        m_nb_elements = 0;
        m_grow_on_next_insert = false;
    }

    template <typename P>
    std::pair<iterator, bool> insert(P&& value)
    {
        return insert_impl(KeySelect()(value), std::forward<P>(value));
    }

    template <typename P>
    iterator insert(const_iterator hint, P&& value)
    {
        if (hint != cend() && compare_keys(KeySelect()(*hint), KeySelect()(value))) {
            return mutable_iterator(hint);
        }

        return insert(std::forward<P>(value)).first;
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last)
    {
        if (std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value) {
            const auto nb_elements_insert = std::distance(first, last);
            const size_type nb_free_buckets = m_load_threshold - size();
            assert(m_load_threshold >= size());

            if (nb_elements_insert > 0 && nb_free_buckets < size_type(nb_elements_insert)) {
                reserve(size() + size_type(nb_elements_insert));
            }
        }

        for (; first != last; ++first) {
            insert(*first);
        }
    }

    template <typename K, typename M>
    std::pair<iterator, bool> insert_or_assign(K&& key, M&& obj)
    {
        auto it = try_emplace(std::forward<K>(key), std::forward<M>(obj));
        if (!it.second) {
            it.first.value() = std::forward<M>(obj);
        }

        return it;
    }

    template <typename K, typename M>
    iterator insert_or_assign(const_iterator hint, K&& key, M&& obj)
    {
        if (hint != cend() && compare_keys(KeySelect()(*hint), key)) {
            auto it = mutable_iterator(hint);
            it.value() = std::forward<M>(obj);

            return it;
        }

        return insert_or_assign(std::forward<K>(key), std::forward<M>(obj)).first;
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        return insert(value_type(std::forward<Args>(args)...));
    }

    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return insert(hint, value_type(std::forward<Args>(args)...));
    }

    template <typename K, typename... Args>
    std::pair<iterator, bool> try_emplace(K&& key, Args&&... args)
    {
        return insert_impl(key, std::piecewise_construct,
                           std::forward_as_tuple(std::forward<K>(key)),
                           std::forward_as_tuple(std::forward<Args>(args)...));
    }

    template <typename K, typename... Args>
    iterator try_emplace(const_iterator hint, K&& key, Args&&... args)
    {
        if (hint != cend() && compare_keys(KeySelect()(*hint), key)) {
            return mutable_iterator(hint);
        }

        return try_emplace(std::forward<K>(key), std::forward<Args>(args)...).first;
    }

    /**
     *  Here to avoid `template <class K> size_type erase(const K& key)`
     *  being used when e use a iterator instead of a const_iterator.
     */
    iterator erase(iterator pos)
    {
        erase_from_bucket(pos);

        /**
         *  Erase bucket used a backward shift after clearing the bucket.
         *  Check if there is a new value in the bucket, if not get the
         *  next non-empty.
         */
        if (pos.m_iterator->empty()) {
            ++pos;
        }

        return pos;
    }

    iterator erase(const_iterator pos)
    {
        return erase(mutable_iterator(pos));
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last) {
            return mutable_iterator(first);
        }

        auto first_mutable = mutable_iterator(first);
        auto last_mutable = mutable_iterator(last);
        for (auto it = first_mutable.m_iterator; it != last_mutable.m_iterator; ++it) {
            if (!it->empty()) {
                it->clear();
                m_nb_elements--;
            }
        }

        if (last_mutable == end()) {
            return end();
        }

        /*
         *  Backward shift on the values which come after the deleted values.
         *  We try to move the values closer to their ideal bucket.
         */
        size_t icloser_bucket = size_t(std::distance(m_buckets.begin(), first_mutable.m_iterator));
        size_t ito_move_closer_value = size_t(std::distance(m_buckets.begin(), last_mutable.m_iterator));
        assert(ito_move_closer_value > icloser_bucket);

        const size_t ireturn_bucket = ito_move_closer_value - std::min(ito_move_closer_value - icloser_bucket, size_t(m_buckets[ito_move_closer_value].dist_from_ideal_bucket()));

        while (ito_move_closer_value < m_buckets.size() && m_buckets[ito_move_closer_value].dist_from_ideal_bucket() > 0) {
            icloser_bucket = ito_move_closer_value - std::min(ito_move_closer_value - icloser_bucket, size_t(m_buckets[ito_move_closer_value].dist_from_ideal_bucket()));

            assert(m_buckets[icloser_bucket].empty());
            const distance_type new_distance = distance_type(m_buckets[ito_move_closer_value].dist_from_ideal_bucket() -
                                                             (ito_move_closer_value - icloser_bucket));
            m_buckets[icloser_bucket].set_value_of_empty_bucket(new_distance,
                                                                m_buckets[ito_move_closer_value].truncated_hash(),
                                                                std::move(m_buckets[ito_move_closer_value].value()));
            m_buckets[ito_move_closer_value].clear();

            ++icloser_bucket;
            ++ito_move_closer_value;
        }

        return iterator(m_buckets.begin() + ireturn_bucket);
    }

    template <typename K>
    size_type erase(const K& key)
    {
        return erase(key, hash_key(key));
    }

    template <typename K>
    size_type erase(const K& key, size_t hash)
    {
        auto it = find(key, hash);
        if (it != end()) {
            erase_from_bucket(it);

            return 1;
        } else {
            return 0;
        }
    }

    void swap(robin_hash& other)
    {
        std::swap(static_cast<Hash&>(*this), static_cast<Hash&>(other));
        std::swap(static_cast<KeyEqual&>(*this), static_cast<KeyEqual&>(other));
        std::swap(static_cast<GrowthPolicy&>(*this), static_cast<GrowthPolicy&>(other));
        std::swap(m_buckets, other.m_buckets);
        std::swap(m_bucket_count, other.m_bucket_count);
        std::swap(m_nb_elements, other.m_nb_elements);
        std::swap(m_load_threshold, other.m_load_threshold);
        std::swap(m_max_load_factor, other.m_max_load_factor);
        std::swap(m_grow_on_next_insert, other.m_grow_on_next_insert);
    }

    // LOOKUP

    template <typename K, typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value>::type* = nullptr>
    typename U::value_type& at(const K& key)
    {
        return at(key, hash_key(key));
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value>::type* = nullptr>
    typename U::value_type& at(const K& key, size_t hash)
    {
        return const_cast<typename U::value_type&>(static_cast<const robin_hash*>(this)->at(key, hash));
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value>::type* = nullptr>
    const typename U::value_type& at(const K& key) const
    {
        return at(key, hash_key(key));
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value>::type* = nullptr>
    const typename U::value_type& at(const K& key, size_t hash) const
    {
        auto it = find(key, hash);
        if (it != cend()) {
            return it.value();
        } else {
            throw std::out_of_range("Couldn't find key.");
        }
    }

    template <typename K, typename U = ValueSelect, typename std::enable_if<has_mapped_type<U>::value>::type* = nullptr>
    typename U::value_type& operator[](K&& key)
    {
        return try_emplace(std::forward<K>(key)).first.value();
    }

    template <typename K>
    size_type count(const K& key) const
    {
        return count(key, hash_key(key));
    }

    template <typename K>
    size_type count(const K& key, size_t hash) const
    {
        if (find(key, hash) != cend()) {
            return 1;
        } else {
            return 0;
        }
    }

    template <typename K>
    iterator find(const K& key)
    {
        return find_impl(key, hash_key(key));
    }

    template <typename K>
    iterator find(const K& key, size_t hash)
    {
        return find_impl(key, hash);
    }

    template <typename K>
    const_iterator find(const K& key) const
    {
        return find_impl(key, hash_key(key));
    }

    template <typename K>
    const_iterator find(const K& key, size_t hash) const
    {
        return find_impl(key, hash);
    }

    template <typename K>
    std::pair<iterator, iterator> equal_range(const K& key)
    {
        return equal_range(key, hash_key(key));
    }

    template <typename K>
    std::pair<iterator, iterator> equal_range(const K& key, size_t hash)
    {
        iterator it = find(key, hash);
        return std::make_pair(it, (it == end()) ? it : std::next(it));
    }

    template <typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
        return equal_range(key, hash_key(key));
    }

    template <typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& key, size_t hash) const
    {
        const_iterator it = find(key, hash);
        return std::make_pair(it, (it == cend()) ? it : std::next(it));
    }

    // BUCKET INTERFACE

    size_type bucket_count() const
    {
        return m_bucket_count;
    }

    size_type max_bucket_count() const
    {
        return std::min(GrowthPolicy::max_bucket_count(), m_buckets.max_size());
    }

    // HASH POLICY

    float load_factor() const
    {
        return float(m_nb_elements) / float(bucket_count());
    }

    float max_load_factor() const
    {
        return m_max_load_factor;
    }

    void max_load_factor(float ml)
    {
        m_max_load_factor = std::max(0.1f, std::min(ml, 0.95f));
        m_load_threshold = size_type(float(bucket_count())*m_max_load_factor);
    }

    void rehash(size_type count)
    {
        count = std::max(count, size_type(std::ceil(float(size()) / max_load_factor())));
        rehash_impl(count);
    }

    void reserve(size_type count)
    {
        rehash(size_type(std::ceil(float(count) / max_load_factor())));
    }

    // OBSERVERS

    hasher hash_function() const
    {
        return static_cast<const Hash&>(*this);
    }

    key_equal key_eq() const
    {
        return static_cast<const KeyEqual&>(*this);
    }

    // OTHER

    iterator mutable_iterator(const_iterator pos)
    {
        return iterator(m_buckets.begin() + std::distance(m_buckets.cbegin(), pos.m_iterator));
    }

private:
    template <typename K>
    size_t hash_key(const K& key) const
    {
        return Hash::operator()(key);
    }

    template <typename K1, typename K2>
    bool compare_keys(const K1& key1, const K2& key2) const
    {
        return KeyEqual::operator()(key1, key2);
    }

    size_t bucket_for_hash(size_t hash) const
    {
        return GrowthPolicy::bucket_for_hash(hash);
    }

    template <typename U = GrowthPolicy, typename std::enable_if<is_power_of_two_policy<U>::value>::type* = nullptr>
    size_t next_bucket(size_t index) const noexcept
    {
        assert(index < bucket_count());

        return (index + 1) & this->m_mask;
    }

    template <typename U = GrowthPolicy, typename std::enable_if<!is_power_of_two_policy<U>::value>::type* = nullptr>
    size_t next_bucket(size_t index) const noexcept
    {
        assert(index < bucket_count());

        index++;
        return (index != bucket_count()) ? index : 0;
    }

    template <typename K>
    iterator find_impl(const K& key, size_t hash)
    {
        return mutable_iterator(static_cast<const robin_hash*>(this)->find(key, hash));
    }

    template <typename K>
    const_iterator find_impl(const K& key, size_t hash) const
    {
        size_t ibucket = bucket_for_hash(hash);
        distance_type dist_from_ideal_bucket = 0;

        while (dist_from_ideal_bucket <= m_buckets[ibucket].dist_from_ideal_bucket()) {
            if ((!USE_STORED_HASH_ON_LOOKUP || m_buckets[ibucket].bucket_hash_equal(hash)) &&
               compare_keys(KeySelect()(m_buckets[ibucket].value()), key))
            {
                return const_iterator(m_buckets.begin() + ibucket);
            }

            ibucket = next_bucket(ibucket);
            dist_from_ideal_bucket++;
        }

        return cend();
    }

    void erase_from_bucket(iterator pos)
    {
        pos.m_iterator->clear();
        m_nb_elements--;

        /**
         *  Backward shift, swap the empty bucket, previous_ibucket,
         *  with the values on its right, ibucket, until we cross
         *  another empty bucket or if the other bucket has a
         *  distance_from_ideal_bucket == 0.
         *
         *  We try to move the values closer to their ideal bucket.
         */
        size_t previous_ibucket = size_t(std::distance(m_buckets.begin(), pos.m_iterator));
        size_t ibucket = next_bucket(previous_ibucket);

        while (m_buckets[ibucket].dist_from_ideal_bucket() > 0) {
            assert(m_buckets[previous_ibucket].empty());

            const distance_type new_distance = distance_type(m_buckets[ibucket].dist_from_ideal_bucket() - 1);
            m_buckets[previous_ibucket].set_value_of_empty_bucket(new_distance, m_buckets[ibucket].truncated_hash(),
                                                                  std::move(m_buckets[ibucket].value()));
            m_buckets[ibucket].clear();

            previous_ibucket = ibucket;
            ibucket = next_bucket(ibucket);
        }
    }

    template <typename K, typename... Args>
    std::pair<iterator, bool> insert_impl(const K& key, Args&&... value_type_args)
    {
        const size_t hash = hash_key(key);

        size_t ibucket = bucket_for_hash(hash);
        distance_type dist_from_ideal_bucket = 0;

        while (dist_from_ideal_bucket <= m_buckets[ibucket].dist_from_ideal_bucket()) {
            if ((!USE_STORED_HASH_ON_LOOKUP || m_buckets[ibucket].bucket_hash_equal(hash)) &&
               compare_keys(KeySelect()(m_buckets[ibucket].value()), key))
            {
                return std::make_pair(iterator(m_buckets.begin() + ibucket), false);
            }

            ibucket = next_bucket(ibucket);
            dist_from_ideal_bucket++;
        }

        if (grow_on_high_load()) {
            ibucket = bucket_for_hash(hash);
            dist_from_ideal_bucket = 0;

            while (dist_from_ideal_bucket <= m_buckets[ibucket].dist_from_ideal_bucket()) {
                ibucket = next_bucket(ibucket);
                dist_from_ideal_bucket++;
            }
        }

        if (m_buckets[ibucket].empty()) {
            m_buckets[ibucket].set_value_of_empty_bucket(dist_from_ideal_bucket, bucket_entry_type::truncate_hash(hash),
                                                         std::forward<Args>(value_type_args)...);
        } else {
            insert_value(ibucket, dist_from_ideal_bucket, bucket_entry_type::truncate_hash(hash),
                         std::forward<Args>(value_type_args)...);
        }

        m_nb_elements++;
        /*
         *  The value will be inserted in ibucket in any case, either
         *  because it was empty or by stealing the bucket (robin hood).
         */
        return std::make_pair(iterator(m_buckets.begin() + ibucket), true);
    }


    template <typename... Args>
    void insert_value(size_t ibucket, distance_type dist_from_ideal_bucket,
                      truncated_hash_type hash, Args&&... value_type_args)
    {
        insert_value(ibucket, dist_from_ideal_bucket, hash, value_type(std::forward<Args>(value_type_args)...));
    }

    void insert_value(size_t ibucket, distance_type dist_from_ideal_bucket,
                      truncated_hash_type hash, value_type&& value)
    {
        m_buckets[ibucket].swap_with_value_in_bucket(dist_from_ideal_bucket, hash, value);
        ibucket = next_bucket(ibucket);
        dist_from_ideal_bucket++;

        while (!m_buckets[ibucket].empty()) {
            if (dist_from_ideal_bucket > m_buckets[ibucket].dist_from_ideal_bucket()) {
                if (dist_from_ideal_bucket >= REHASH_ON_HIGH_NB_PROBES__NPROBES &&
                   load_factor() >= REHASH_ON_HIGH_NB_PROBES__MIN_LOAD_FACTOR)
                {
                    /**
                     *  The number of probes is really high, rehash the
                     *  map on the next insert. Difficult to do now as
                     *  rehash may throw.
                     */
                    m_grow_on_next_insert = true;
                }

                m_buckets[ibucket].swap_with_value_in_bucket(dist_from_ideal_bucket, hash, value);
            }

            ibucket = next_bucket(ibucket);
            dist_from_ideal_bucket++;
        }

        m_buckets[ibucket].set_value_of_empty_bucket(dist_from_ideal_bucket, hash, std::move(value));
    }


    void rehash_impl(size_type count)
    {
        robin_hash new_table(count, static_cast<Hash&>(*this), static_cast<KeyEqual&>(*this), get_allocator(), m_max_load_factor);

        const bool use_stored_hash = use_stored_hash_on_rehash(new_table.bucket_count());
        for (auto& bucket: m_buckets) {
            if (bucket.empty()) {
                continue;
            }

            const size_t hash = use_stored_hash ? bucket.truncated_hash() : new_table.hash_key(KeySelect()(bucket.value()));
            new_table.insert_value_on_rehash(new_table.bucket_for_hash(hash), 0, bucket_entry_type::truncate_hash(hash), std::move(bucket.value()));
        }

        new_table.m_nb_elements = m_nb_elements;
        new_table.swap(*this);
    }

    void insert_value_on_rehash(size_t ibucket, distance_type dist_from_ideal_bucket,
                                truncated_hash_type hash, value_type&& value)
    {
        while (true) {
            if (dist_from_ideal_bucket > m_buckets[ibucket].dist_from_ideal_bucket()) {
                if (m_buckets[ibucket].empty()) {
                    m_buckets[ibucket].set_value_of_empty_bucket(dist_from_ideal_bucket, hash, std::move(value));
                    return;
                } else {
                    m_buckets[ibucket].swap_with_value_in_bucket(dist_from_ideal_bucket, hash, value);
                }
            }

            dist_from_ideal_bucket++;
            ibucket = next_bucket(ibucket);
        }
    }

    /**
     *  Return true if the map has been rehashed.
     */
    bool grow_on_high_load()
    {
        if (m_grow_on_next_insert || size() >= m_load_threshold) {
            rehash_impl(GrowthPolicy::next_bucket_count());
            m_grow_on_next_insert = false;

            return true;
        }

        return false;
    }

public:
    static const size_type DEFAULT_INIT_BUCKETS_SIZE = 16;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 0.5f;

private:
    static const distance_type REHASH_ON_HIGH_NB_PROBES__NPROBES = 128;
    static constexpr float REHASH_ON_HIGH_NB_PROBES__MIN_LOAD_FACTOR = 0.15f;

private:
    buckets_container_type m_buckets;

    /**
     *  Used a lot in find, avoid the call to m_buckets.size()
     *  which is a bit slower.
     */
    size_type m_bucket_count;

    size_type m_nb_elements;

    size_type m_load_threshold;
    float m_max_load_factor;

    bool m_grow_on_next_insert;
};

}   /* robin_detail */

PYCPP_END_NAMESPACE
