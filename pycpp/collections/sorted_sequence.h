//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Self-sorting linear sequence.
 *
 *  Sorted sequences produce faster iteration and lookup times than
 *  a `std::set`, albeit at the cost of asymptotically worse insertion
 *  times (`O(n)` compared to `O(logn)`). Therefore, when the number
 *  of lookups dramatically exceeds the number of insertions,
 *  using a `sorted_sequence` may produce much better overall
 *  performance.
 *
 *  Unlike other non-associative containers, this does not support
 *  mutable access to elements, such as via iteration,  `at()`, or
 *  `operator[]`, since these may interfere with ordering. Element
 *  access, via `operator[]`, `at()`, `front()`, and `back()`, operate
 *  like `the underlying container, providing access to items by
 *  position. Meanwhile, lookup methods, like `find()`, `count()`,
 *  `lower_bound()`, `upper_bound()`, and `equal_range()` behave like
 *  an associative container. Similarly, the modifiers `insert()`,
 *  `erase()`, `swap()`, `clear()`, `emplace()`, and `emplace_hint`
 *  behave like associative containers.
 *
 *  The container should support a minimal subset of STL methods,
 *  including range, initializer list, copy, and move construction,
 *  and be a random-access container, which is supported by both
 *  `std::vector` and `std::deque` in the STL.
 *
 *  Implemented based on the following paper by Matt Austern:
 *      lafstern.org/matt/col1.pdf
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/sfinae/reserve.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

template <
    typename T,
    typename Compare = less<T>,
    typename Alloc = allocator<T>,
    template <typename, typename> class Container = vector
>
struct sorted_sequence
{
    // MEMBER TYPES
    // ------------
    using self_t = sorted_sequence<T, Compare, Alloc, Container>;
    using key_type = T;
    using value_type = T;
    using container_type = Container<T, Alloc>;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename container_type::const_iterator;
    using const_iterator = typename container_type::const_iterator;
    using reverse_iterator = typename container_type::const_reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;

    // MEMBER FUNCTIONS
    // ----------------
    sorted_sequence();
    explicit sorted_sequence(const allocator_type&);
    template <typename Iter> sorted_sequence(Iter first, Iter last, const allocator_type& = allocator_type());
    sorted_sequence(const self_t&, const allocator_type& = allocator_type());
    sorted_sequence(self_t&&, const allocator_type& = allocator_type());
    sorted_sequence(initializer_list<value_type>);
    sorted_sequence(initializer_list<value_type>, const allocator_type&);
    self_t& operator=(const self_t&);
    self_t& operator=(self_t&&);
    self_t& operator=(initializer_list<value_type>);

    // ITERATORS
    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    // CAPACITY
    size_type size() const;
    size_type max_size() const;
    bool empty() const noexcept;

    // ELEMENT ACCESS
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const;
    const_reference front() const;
    const_reference back() const;

    // ELEMENT LOOKUP
    const_iterator find(const key_type&) const;
    size_type count(const key_type&) const;
    const_iterator lower_bound(const key_type&) const;
    const_iterator upper_bound(const key_type&) const;
    pair<const_iterator, const_iterator> equal_range(const key_type&) const;

    // MODIFIERS
    template <typename Iter> void assign(Iter first, Iter last);
    void assign(initializer_list<value_type>);
    pair<iterator,bool> insert(const key_type&);
    template <typename U> pair<iterator,bool> insert(U&&);
    iterator insert(const_iterator, const key_type&);
    template <typename U> iterator insert(const_iterator, U&&);
    template <typename Iter> void insert(Iter first, Iter last);
    void insert(initializer_list<value_type>);
    iterator erase(const_iterator position);
    size_type erase(const key_type&);
    iterator erase(const_iterator first, const_iterator last);
    void swap(self_t& rhs);
    void clear();
    template <typename... Ts> pair<iterator, bool> emplace(Ts&&...);
    template <typename... Ts> iterator emplace_hint(const_iterator, Ts&&...);

    // OBSERVERS
    key_compare key_comp() const noexcept;
    value_compare value_comp() const noexcept;
    allocator_type get_allocator() const noexcept;

    // RELATION OPERATORS
    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator==(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator!=(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator<(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator<=(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator>(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

    template <typename U, typename C, typename A, template <typename, typename> class _>
    friend bool operator>=(const sorted_sequence<U, C, A, _>& lhs, const sorted_sequence<U, C, A, _>& rhs);

private:
    container_type container_;
};


// IMPLEMENTATION
// --------------

template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence():
    container_()
{}


template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence(const allocator_type& alloc):
    container_(alloc)
{}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename Iter>
sorted_sequence<T, C, A, _>::sorted_sequence(Iter first, Iter last, const allocator_type& alloc):
    container_(alloc)
{
    assign(first, last);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence(const self_t& rhs, const allocator_type& alloc):
    container_(rhs.container_, alloc)
{}


template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence(self_t&& rhs, const allocator_type& alloc):
    container_(std::move(rhs.container_), alloc)
{}


template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence(initializer_list<value_type> list)
{
    assign(list.begin(), list.end());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
sorted_sequence<T, C, A, _>::sorted_sequence(initializer_list<value_type> list, const allocator_type& alloc):
    container_(alloc)
{
    assign(list.begin(), list.end());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::operator=(const self_t& rhs) -> self_t&
{
    container_ = rhs.container_;
    return *this;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::operator=(self_t&& rhs) -> self_t&
{
    container_ = std::move(rhs.container_);
    return *this;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::operator=(initializer_list<value_type> list) -> self_t&
{
    assign(list.begin(), list.end());
    return *this;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::begin() const -> const_iterator
{
    return container_.begin();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::end() const -> const_iterator
{
    return container_.end();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::rbegin() const -> const_reverse_iterator
{
    return container_.rbegin();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::rend() const -> const_reverse_iterator
{
    return container_.rend();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::cbegin() const -> const_iterator
{
    return container_.cbegin();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::cend() const -> const_iterator
{
    return container_.cend();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::crbegin() const -> const_reverse_iterator
{
    return container_.crbegin();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::crend() const -> const_reverse_iterator
{
    return container_.crend();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::size() const -> size_type
{
    return container_.size();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::max_size() const -> size_type
{
    return container_.max_size();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool sorted_sequence<T, C, A, _>::empty() const noexcept
{
    return container_.empty();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::operator[](size_type pos) const -> const_reference
{
    return container_[pos];
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::at(size_type pos) const -> const_reference
{
    return container_.at(pos);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::front() const -> const_reference
{
    return container_.front();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::back() const -> const_reference
{
    return container_.back();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::find(const key_type& key) const -> const_iterator
{
    const_iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, *it)) {
        return end();
    }
    return it;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::count(const key_type& key) const -> size_type
{
    if (find(key) != end()) {
        return 1;
    }
    return 0;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::lower_bound(const key_type& key) const -> const_iterator
{
    return std::lower_bound(begin(), end(), key, key_comp());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::upper_bound(const key_type& key) const -> const_iterator
{
    return std::upper_bound(begin(), end(), key, key_comp());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::equal_range(const key_type& key) const -> pair<const_iterator, const_iterator>
{
    return std::make_pair(lower_bound(key), upper_bound(key));
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename Iter>
void sorted_sequence<T, C, A, _>::assign(Iter first, Iter last)
{
    container_.assign(first, last);
    std::sort(container_.begin(), container_.end(), key_comp());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
void sorted_sequence<T, C, A, _>::assign(initializer_list<value_type> list)
{
    assign(list.begin(), list.end());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::insert(const key_type& key) -> pair<iterator,bool>
{
    const_iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, *it)) {
        // item not found, inserting value.
        it = container_.insert(it, key);
        return std::make_pair(it, true);
    }
    // item found, returning equivalent value.
    return std::make_pair(it, false);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename U>
auto sorted_sequence<T, C, A, _>::insert(U&& k) -> pair<iterator,bool>
{
    key_type key(std::forward<U>(k));
    const_iterator it = lower_bound(key);
    if (it == end() || key_comp()(key, *it)) {
        // item not found, inserting value.
        it = container_.insert(it, std::move(key));
        return std::make_pair(it, true);
    }
    // item found, returning equivalent value.
    return std::make_pair(it, false);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::insert(const_iterator position, const key_type& key) -> iterator
{
    const_iterator it;
    if (position == end()) {
        // hint is at end
        if (container_.empty() || key_comp()(back(), key)) {
            // container is larger than any value, insert at end
            it = end();
        } else {
            // invalid hint, just ignore the hint
            it = lower_bound(key);
        }
    } else if (key_comp()(key, *position)) {
        // key is less than hint
        it = std::lower_bound(begin(), position, key, key_comp());
    } else {
        // key is greater than or equal to hint
        it = std::lower_bound(position, end(), key, key_comp());
    }

    // insert item
    if (it == end() || key_comp()(key, *it)) {
        // item not found, inserting value.
        it = container_.insert(it, key);
        return std::make_pair(it, true);
    }
    // item found, returning equivalent value.
    return std::make_pair(it, false);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename U>
auto sorted_sequence<T, C, A, _>::insert(const_iterator position, U&& k) -> iterator
{
    key_type key(std::forward<U>(k));
    const_iterator it;
    if (position == end()) {
        // hint is at end
        if (container_.empty() || key_comp()(back(), key)) {
            // container is larger than any value, insert at end
            it = end();
        } else {
            // invalid hint, just ignore the hint
            it = lower_bound(key);
        }
    } else if (key_comp()(key, *position)) {
        // key is less than hint
        it = std::lower_bound(begin(), position, key, key_comp());
    } else {
        // key is greater than or equal to hint
        it = std::lower_bound(position, end(), key, key_comp());
    }

    // insert item
    if (it == end() || key_comp()(key, *it)) {
        // item not found, inserting value.
        return container_.insert(it, std::move(key));
    }
    // item found, returning equivalent value.
    return it;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename Iter>
void sorted_sequence<T, C, A, _>::insert(Iter first, Iter last)
{
    if (is_forward_iterable<Iter>::value) {
        // reserve the underlying container if we can
        size_t distance = std::distance(first, last);
        size_t size = container_.size();
        reserve()(container_, size + distance);

        // shortcut if inserting >25% items, shortens time complexity
        if (size < 4 || size / 4 > distance) {
            container_.insert(first, last);
            std::sort(container_.begin(), container_.end(), key_comp());
            return;
        }
    }

    // insert elements
    for (; first != last; ++first) {
        insert(*first);
    }
}


template <typename T, typename C, typename A, template <typename, typename> class _>
void sorted_sequence<T, C, A, _>::insert(initializer_list<value_type> list)
{
    insert(list.begin(), list.end());
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::erase(const_iterator position) -> iterator
{
    return container_.erase(position);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::erase(const key_type& key) -> size_type
{
    const_iterator it = find(key);
    if (it == end()) {
        return 0;
    }
    erase(it);
    return 1;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::erase(const_iterator first, const_iterator last) -> iterator
{
    return container_.erase(first, last);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
void sorted_sequence<T, C, A, _>::swap(self_t& rhs)
{
    std::swap(container_, rhs.container_);
}


template <typename T, typename C, typename A, template <typename, typename> class _>
void sorted_sequence<T, C, A, _>::clear()
{
    container_.clear();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename... Ts>
auto sorted_sequence<T, C, A, _>::emplace(Ts&&... ts) -> pair<iterator, bool>
{
    return insert(key_type(std::forward<Ts>(ts)...));
}


template <typename T, typename C, typename A, template <typename, typename> class _>
template <typename... Ts>
auto sorted_sequence<T, C, A, _>::emplace_hint(const_iterator position, Ts&&... ts) -> iterator
{
    return insert(position, key_type(std::forward<Ts>(ts)...));
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::key_comp() const noexcept -> key_compare
{
    return key_compare();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::value_comp() const noexcept -> value_compare
{
    return value_compare();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
auto sorted_sequence<T, C, A, _>::get_allocator() const noexcept -> allocator_type
{
    return container_.get_allocator();
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator==(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ == rhs.container_;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator!=(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ != rhs.container_;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator<(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ < rhs.container_;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator<=(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ <= rhs.container_;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator>(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ > rhs.container_;
}


template <typename T, typename C, typename A, template <typename, typename> class _>
bool operator>=(const sorted_sequence<T, C, A, _>& lhs, const sorted_sequence<T, C, A, _>& rhs)
{
    return lhs.container_ >= rhs.container_;
}

PYCPP_END_NAMESPACE
