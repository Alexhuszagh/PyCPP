//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Dictionary with function overload for missing values.
 */

 #pragma once

#include <pycpp/config.h>
#include <cassert>
#include <functional>
#include <map>
#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

template <typename T>
using default_map_callback = std::function<T()>;

// DECLARATION
// -----------

/**
 *  \brief Use default constructor to initialize value.
 */
template <typename T>
T default_constructor();

/**
 *  \brief Binary search tree implementation of the default map.
 */
template <
    typename Key,
    typename T,
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key, T>>,
    template <typename, typename, typename, typename> typename Map = std::map
>
struct default_map
{
public:
    // MEMBER TYPES
    // ------------
    using base_t = Map<Key, T, Compare, Alloc>;
    using self_t = default_map<Key, T, Compare, Alloc>;
    using callback_type = default_map_callback<T>;
    using key_type = typename base_t::key_type;
    using mapped_type = typename base_t::mapped_type;
    using value_type = typename base_t::value_type;
    using key_compare = typename base_t::key_compare;
    using value_compare = typename base_t::value_compare;
    using allocator_type = typename base_t::allocator_type;
    using reference = typename base_t::reference;
    using const_reference = typename base_t::const_reference;
    using const_pointer = typename base_t::const_pointer;
    using iterator = typename base_t::iterator;
    using const_iterator = typename base_t::const_iterator;
    using reverse_iterator = typename base_t::reverse_iterator;
    using const_reverse_iterator = typename base_t::const_reverse_iterator;
    using difference_type = typename base_t::difference_type;
    using size_type = typename base_t::size_type;

    // MEMBER FUNCTIONS
    // ----------------
    default_map(callback_type callback = default_constructor<value_type>);
    default_map(std::initializer_list<value_type>, callback_type callback = default_constructor<value_type>);
    template <typename Iter> default_map(Iter first, Iter last, callback_type callback = default_constructor<value_type>);
    default_map(const self_t&);
    self_t& operator=(const self_t&);
    default_map(self_t&&);
    self_t& operator=(self_t&&);

    // CAPACITY
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // ITERATORS
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // ELEMENT ACCESS
    mapped_type& operator[](const key_type& k);
    mapped_type& operator[](key_type&& k);
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;

    // OPERATIONS
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    iterator lower_bound(const key_type& k);
    const_iterator lower_bound(const key_type& k) const;
    iterator upper_bound(const key_type& k);
    const_iterator upper_bound(const key_type& k) const;
    std::pair<iterator, iterator> equal_range(const key_type& k);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

    // MODIFIERS
    // TODO:
//    void swap(self_t& rhs);

    // OBSERVERS
    allocator_type get_allocator() const noexcept;
    key_compare key_comp() const;
    value_compare value_comp() const;

private:
    base_t map_;
    callback_type callback_ = nullptr;

    // TODO: friend functions
};


/**
 *  \brief Hashmap implementation of the default map.
 */
template <
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key>,
    typename Alloc = std::allocator<std::pair<const Key, T>>,
    template <typename, typename, typename, typename, typename> typename Map = std::unordered_map
>
struct default_unordered_map
{
public:
    // MEMBER TYPES
    // ------------
    using base_t = Map<Key, T, Hash, Pred, Alloc>;
    using self_t = default_unordered_map<Key, T, Hash, Pred, Alloc>;
    using callback_type = default_map_callback<T>;
    using key_type = typename base_t::key_type;
    using mapped_type = typename base_t::mapped_type;
    using value_type = typename base_t::value_type;
    using hasher = typename base_t::hasher;
    using key_equal = typename base_t::key_equal;
    using allocator_type = typename base_t::allocator_type;
    using reference = typename base_t::reference;
    using const_reference = typename base_t::const_reference;
    using pointer = typename base_t::pointer;
    using const_pointer = typename base_t::const_pointer;
    using iterator = typename base_t::iterator;
    using const_iterator = typename base_t::const_iterator;
    using local_iterator = typename base_t::local_iterator;
    using const_local_iterator = typename base_t::const_local_iterator;
    using size_type = typename base_t::size_type;
    using difference_type = typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    default_unordered_map(callback_type callback = default_constructor<value_type>);
    default_unordered_map(std::initializer_list<value_type>, callback_type callback = default_constructor<value_type>);
    template <typename Iter> default_unordered_map(Iter first, Iter last, callback_type callback = default_constructor<value_type>);
    default_unordered_map(const self_t&);
    self_t& operator=(const self_t&);
    default_unordered_map(self_t&&);
    self_t& operator=(self_t&&);

    // CAPACITY
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // ITERATORS
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    local_iterator begin(size_type n) noexcept;
    const_local_iterator begin(size_type n) const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    local_iterator end(size_type n) noexcept;
    const_local_iterator end(size_type n) const noexcept;
    const_iterator cbegin() const noexcept;
    const_local_iterator cbegin(size_type n) const noexcept;
    const_iterator cend() const noexcept;
    const_local_iterator cend(size_type n) const noexcept;

    // ELEMENT ACCESS
    mapped_type& operator[](const key_type& k);
    mapped_type& operator[](key_type&& k);
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;

    // OPERATIONS
    iterator find(const key_type& k);
    const_iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    std::pair<iterator, iterator> equal_range(const key_type& k);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

    // MODIFIERS
    // TODO:
//    void swap(self_t& rhs);

    // BUCKETS
    size_type bucket_count() const noexcept;
    size_type max_bucket_count() const noexcept;
    size_type bucket_size(size_type n) const;
    size_type bucket(const key_type& k) const;

    // HASH POLICY
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float z);
    void rehash(size_type n);
    void reserve(size_type n);

    // OBSERVERS
    allocator_type get_allocator() const noexcept;
    hasher hash_function() const;
    key_equal key_eq() const;

private:
    base_t map_;
    callback_type callback_ = nullptr;

    // TODO: friend functions
};

// DEFINITION
// ----------

// FUNCTIONS

template <typename T>
T default_constructor()
{
    return T();
}

// DEFAULT MAP

template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
default_map<K, T, C, A, M>::default_map(callback_type callback):
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
default_map<K, T, C, A, M>::default_map(std::initializer_list<value_type> list, callback_type callback):
    map_(list.begin(), list.end()),
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
template <typename Iter>
default_map<K, T, C, A, M>::default_map(Iter first, Iter last, callback_type callback):
    map_(first, last),
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
default_map<K, T, C, A, M>::default_map(const self_t& rhs):
    map_(rhs.map_),
    callback_(rhs.callback_)
{}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::operator=(const self_t& rhs) -> self_t&
{
    map_ = rhs.map_;
    callback_ = rhs.callback_;
    return *this;
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
default_map<K, T, C, A, M>::default_map(self_t&& rhs)
{
    swap(rhs);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::operator=(self_t&& rhs) -> self_t&
{
    swap(rhs);
    return *this;
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
bool default_map<K, T, C, A, M>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::size() const noexcept -> size_type
{
    return map_.size();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::max_size() const noexcept -> size_type
{
    return map_.max_size();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::begin() noexcept -> iterator
{
    return map_.begin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::begin() const noexcept -> const_iterator
{
    return map_.begin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::end() noexcept -> iterator
{
    return map_.end();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::end() const noexcept -> const_iterator
{
    return map_.end();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::rbegin() noexcept -> reverse_iterator
{
    return map_.rbegin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::rbegin() const noexcept -> const_reverse_iterator
{
    return map_.rbegin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::rend() noexcept -> reverse_iterator
{
    return map_.rend();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::rend() const noexcept -> const_reverse_iterator
{
    return map_.rend();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::cbegin() const noexcept -> const_iterator
{
    return map_.cbegin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::cend() const noexcept -> const_iterator
{
    return map_.cend();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::crbegin() const noexcept -> const_reverse_iterator
{
    return map_.crbegin();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::crend() const noexcept -> const_reverse_iterator
{
    return map_.crend();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::operator[](const key_type& k) -> mapped_type&
{
    return map_[k];
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::operator[](key_type&& k) -> mapped_type&
{
    return map_[std::forward<key_type>(k)];
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::at(const key_type& k) -> mapped_type&
{
    return map_.at(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::at(const key_type& k) const -> const mapped_type&
{
    return map_.at(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::find(const key_type& k) -> iterator
{
    return map_.find(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::find(const key_type& k) const -> const_iterator
{
    return map_.find(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::count(const key_type& k) const -> size_type
{
    return map_.count(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::lower_bound(const key_type& k) -> iterator
{
    return map_.lower_bound(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::lower_bound(const key_type& k) const -> const_iterator
{
    return map_.lower_bound(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::upper_bound(const key_type& k) -> iterator
{
    return map_.upper_bound(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::upper_bound(const key_type& k) const -> const_iterator
{
    return map_.upper_bound(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::equal_range(const key_type& k) -> std::pair<iterator, iterator>
{
    return map_.equal_range(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::equal_range(const key_type& k) const -> std::pair<const_iterator, const_iterator>
{
    return map_.equal_range(k);
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::get_allocator() const noexcept -> allocator_type
{
    return map_.get_allocator();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::key_comp() const -> key_compare
{
    return map_.key_comp();
}


template <typename K, typename T, typename C, typename A, template <typename, typename, typename, typename> typename M>
auto default_map<K, T, C, A, M>::value_comp() const -> value_compare
{
    return map_.value_comp();
}

// DEFAULT UNORDERED MAP

template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
default_unordered_map<K, T, H, P, A, M>::default_unordered_map(callback_type callback):
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
default_unordered_map<K, T, H, P, A, M>::default_unordered_map(std::initializer_list<value_type> list, callback_type callback):
    map_(list.begin(), list.end()),
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
template <typename Iter>
default_unordered_map<K, T, H, P, A, M>::default_unordered_map(Iter first, Iter last, callback_type callback):
    map_(first, last),
    callback_(callback)
{
    assert(callback_ && "Callback cannot be null.");
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
default_unordered_map<K, T, H, P, A, M>::default_unordered_map(const self_t& rhs):
    map_(rhs.map_),
    callback_(rhs.callback_)
{}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::operator=(const self_t& rhs) -> self_t&
{
    map_ = rhs.map_;
    callback_ = rhs.callback_;
    return *this;
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
default_unordered_map<K, T, H, P, A, M>::default_unordered_map(self_t&& rhs)
{
    swap(rhs);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::operator=(self_t&& rhs) -> self_t&
{
    swap(rhs);
    return *this;
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
bool default_unordered_map<K, T, H, P, A, M>::empty() const noexcept
{
    return map_.empty();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::size() const noexcept -> size_type
{
    return map_.size();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::max_size() const noexcept -> size_type
{
    return map_.max_size();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::begin() noexcept -> iterator
{
    return map_.begin();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::begin() const noexcept -> const_iterator
{
    return map_.begin();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::begin(size_type n) noexcept -> local_iterator
{
    return map_.begin(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::begin(size_type n) const noexcept -> const_local_iterator
{
    return map_.begin(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::end() noexcept -> iterator
{
    return map_.end();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::end() const noexcept -> const_iterator
{
    return map_.end();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::end(size_type n) noexcept -> local_iterator
{
    return map_.end(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::end(size_type n) const noexcept -> const_local_iterator
{
    return map_.end(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::cbegin() const noexcept -> const_iterator
{
    return map_.cbegin();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::cbegin(size_type n) const noexcept -> const_local_iterator
{
    return map_.cbegin(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::cend() const noexcept -> const_iterator
{
    return map_.cend();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::cend(size_type n) const noexcept -> const_local_iterator
{
    return map_.cend(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::operator[](const key_type& k) -> mapped_type&
{
    return map_[k];
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::operator[](key_type&& k) -> mapped_type&
{
    return map_[std::forward<key_type>(k)];
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::at(const key_type& k) -> mapped_type&
{
    return map_.at(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::at(const key_type& k) const -> const mapped_type&
{
    return map_.at(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::find(const key_type& k) -> iterator
{
    return map_.find(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::find(const key_type& k) const -> const_iterator
{
    return map_.find(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::count(const key_type& k) const -> size_type
{
    return map_.count(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::equal_range(const key_type& k) -> std::pair<iterator, iterator>
{
    return map_.equal_range(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::equal_range(const key_type& k) const -> std::pair<const_iterator, const_iterator>
{
    return map_.equal_range(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::bucket_count() const noexcept -> size_type
{
    return map_.bucket_count();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::max_bucket_count() const noexcept -> size_type
{
    return map_.max_bucket_count();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::bucket_size(size_type n) const -> size_type
{
    return map_.bucket_size(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::bucket(const key_type& k) const -> size_type
{
    return map_.bucket(k);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::load_factor() const noexcept -> float
{
    return map_.load_factor();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::max_load_factor() const noexcept -> float
{
    return map_.max_load_factor();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::max_load_factor(float z) -> void
{
    return map_.max_load_factor(z);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::rehash(size_type n) -> void
{
    return map_.rehash(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::reserve(size_type n) -> void
{
    return map_.reserve(n);
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::get_allocator() const noexcept -> allocator_type
{
    return map_.get_allocator();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::hash_function() const -> hasher
{
    return map_.hash_function();
}


template <typename K, typename T, typename H, typename P, typename A, template <typename, typename, typename, typename, typename> typename M>
auto default_unordered_map<K, T, H, P, A, M>::key_eq() const -> key_equal
{
    return map_.key_eq();
}

PYCPP_END_NAMESPACE
