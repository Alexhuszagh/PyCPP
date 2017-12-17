//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Intrusive singly-linked list.
 */

#pragma once

#include <pycpp/stl/iterator.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
// TODO:

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief POD base class for a node of an `intrusive_forward_list`.
 *
 *  You should subclass this class to create the custom data type
 * for your intrusive forward list.
 */
struct intrusive_forward_list_node
{
    intrusive_forward_list_node* next = nullptr;
};


/**
 *  \brief Iterator type to wrap nodes.
 */
template <typename T>
struct intrusive_forward_list_iterator: iterator<forward_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = intrusive_forward_list_iterator<T>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    intrusive_forward_list_iterator(T* node = nullptr);
    intrusive_forward_list_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
//    template <typename U>
//    enable_if_t<is_same<T, add_const_t<U>>::value, self_t&>
//    operator=(const intrusive_forward_list_iterator<U, DifferenceType, Pointer, Reference>&);

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    pointer operator->();
    const_pointer operator->() const;
    reference operator*();
    const_reference operator*() const;

//    // OTHER
//    void swap(self_t&);
//
//    // RELATIONAL
//    bool operator==(const self_t&) const;
//    bool operator!=(const self_t&) const;

    // TODO: here...
private:
    template <typename U>
    friend struct intrusive_forward_list;

    T* node_;
};


/**
 *  \brief Intrusive forward list container.
 */
template <typename T>
struct intrusive_forward_list
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = intrusive_forward_list<T>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    // TODO: need conversion to and from iterators
    using iterator = intrusive_forward_list_iterator<T>;
    using const_iterator = intrusive_forward_list_iterator<const T>;

    // CONSTRUCTORS
    intrusive_forward_list() noexcept;
    ~intrusive_forward_list() noexcept;

//    // construct/copy/destroy:
//    intrusive_forward_list() noexcept;
//    template <class Iter>
//    intrusive_forward_list(Iter first, Iter last);
//    intrusive_forward_list(intrusive_forward_list&& x) noexcept;
//    ~intrusive_forward_list() noexcept;
//    intrusive_forward_list& operator=(intrusive_forward_list&& x) noexcept;
//    template <class Iter>
//      void assign(Iter first, Iter last);

    // ITERATORS
    iterator before_begin() noexcept;
    const_iterator before_begin() const noexcept;
    const_iterator cbefore_begin() const noexcept;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // CAPACITY
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // ELEMENT ACCESS
    reference front() noexcept;
    const_reference front() const noexcept;

    // MODIFIERS
    void push_front(value_type& x) noexcept;
    void pop_front() noexcept;
    iterator insert_after(const_iterator position, value_type& x) noexcept;
    template <typename Iter> iterator insert_after(const_iterator position, Iter first, Iter last);
    iterator erase_after(const_iterator position) noexcept;
    iterator erase_after(const_iterator first, const_iterator last) noexcept;
    void swap(self_t&) noexcept;
    void clear() noexcept;
//
//    // list operations:
//    void splice_after(const_iterator position, intrusive_forward_list& x) noexcept;
//    void splice_after(const_iterator position, intrusive_forward_list&& x) noexcept;
//    void splice_after(const_iterator position, intrusive_forward_list& x,
//                      const_iterator i) noexcept;
//    void splice_after(const_iterator position, intrusive_forward_list&& x,
//                      const_iterator i) noexcept;
//    void splice_after(const_iterator position, intrusive_forward_list& x,
//                      const_iterator first, const_iterator last) noexcept;
//    void splice_after(const_iterator position, intrusive_forward_list&& x,
//                      const_iterator first, const_iterator last) noexcept;
//
//    void remove(const T& value);
//    template <class Predicate> void remove_if(Predicate pred);
//
//    void unique();
//    template <class BinaryPredicate>
//    void unique(BinaryPredicate binary_pred);
//
//    void merge(list& x);
//    void merge(list&& x);
//    template <class Compare> void merge(list& x, Compare comp);
//    template <class Compare> void merge(list&& x, Compare comp);
//
//    void sort();
//    template <class Compare> void sort(Compare comp);
//
//    void reverse() noexcept;

private:
    intrusive_forward_list_node sentinel_;
};

// IMPLEMENTATION
// --------------

// ITERATOR

template <typename T>
intrusive_forward_list_iterator<T>::intrusive_forward_list_iterator(T* node):
    node_(node)
{}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator++() -> self_t&
{
    node_ = node_->next;
}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator->() -> pointer
{
    return addressof(operator*());
}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator->() const -> const_pointer
{
    return addressof(operator*());
}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator*() -> reference
{
    return *node_;
}


template <typename T>
auto intrusive_forward_list_iterator<T>::operator*() const -> const_reference
{
    return *node_;
}

// LIST

template <typename T>
intrusive_forward_list<T>::intrusive_forward_list() noexcept
{
    sentinel_.next = &sentinel_;
}


template <typename T>
intrusive_forward_list<T>::~intrusive_forward_list() noexcept
{}


template <typename T>
auto intrusive_forward_list<T>::before_begin() noexcept -> iterator
{
    return iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
auto intrusive_forward_list<T>::before_begin() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
auto intrusive_forward_list<T>::cbefore_begin() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
auto intrusive_forward_list<T>::begin() noexcept -> iterator
{
    return iterator(static_cast<T*>(sentinel_.next));
}


template <typename T>
auto intrusive_forward_list<T>::begin() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(sentinel_.next));
}


template <typename T>
auto intrusive_forward_list<T>::cbegin() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(sentinel_.next));
}


template <typename T>
auto intrusive_forward_list<T>::end() noexcept -> iterator
{
    return iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
auto intrusive_forward_list<T>::end() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
auto intrusive_forward_list<T>::cend() const noexcept -> const_iterator
{
    return const_iterator(static_cast<T*>(&sentinel_));
}


template <typename T>
bool intrusive_forward_list<T>::empty() const noexcept
{
    return sentinel_.next == &sentinel_;
}


template <typename T>
auto intrusive_forward_list<T>::size() const noexcept -> size_type
{
    return distance(begin(), end());
}


template <typename T>
auto intrusive_forward_list<T>::max_size() const noexcept -> size_type
{
    return numeric_limits<size_type>::max() / sizeof(T);
}


template <typename T>
auto intrusive_forward_list<T>::front() noexcept -> reference
{
    return *begin();
}


template <typename T>
auto intrusive_forward_list<T>::front() const noexcept -> const_reference
{
    return *begin();
}


template <typename T>
void intrusive_forward_list<T>::push_front(value_type& x) noexcept
{
    intrusive_forward_list_node* next = sentinel_.next;
    sentinel_.next = &x;
    x.next = next;
}


template <typename T>
void intrusive_forward_list<T>::pop_front() noexcept
{
    intrusive_forward_list_node* front = sentinel_.next;
    sentinel_.next = front->next;
}


template <typename T>
auto intrusive_forward_list<T>::insert_after(const_iterator position, value_type& x) noexcept -> iterator
{
    intrusive_forward_list_node* before, *after;
    before = position.node_;
    after = before->next;

    before->next = &x;
    x.next = after;
}


template <typename T>
template <typename Iter>
auto intrusive_forward_list<T>::insert_after(const_iterator position, Iter first, Iter last) -> iterator
{
    iterator it(position.node_);
    for (; first != last; ++first) {
        it = insert_after(it, *first);
    }
    return it;
}


template <typename T>
auto intrusive_forward_list<T>::erase_after(const_iterator position) noexcept -> iterator
{
    intrusive_forward_list_node* before, *after;
    before = position.node_;
    after = before->next;

    before->next = after->next;
    return iterator(before->next);
}


template <typename T>
auto intrusive_forward_list<T>::erase_after(const_iterator first, const_iterator last) noexcept -> iterator
{
    for (; first != last; ) {
        first = erase_after(first);
    }
    return iterator(last.node_);
}

// TODO: here

template <typename T>
void intrusive_forward_list<T>::swap(self_t& rhs) noexcept
{
    intrusive_forward_list_node* lhs_next, *rhs_next;

    // swap the first item
    rhs_next = rhs.sentinel_.next;
    rhs.sentinel_.next = sentinel_.next;
    sentinel_.next = rhs_next;

    // find node before end
    lhs_next = rhs.sentinel_.next;
    rhs_next = sentinel_.next;
    for (; lhs_next->next != &sentinel_; ++lhs_next)
        ;
    for (; rhs_next->next != &rhs.sentinel_; ++rhs_next)
        ;

    // swap the node before the end
    lhs_next->next = &rhs.sentinel_;
    rhs_next->next = &sentinel_;
}


template <typename T>
void intrusive_forward_list<T>::clear() noexcept
{
    sentinel_.next = &sentinel_;
}

//  // Comparison operators
//  template <class T, class Tag>
//  bool operator==(const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//  template <class T, class Tag>
//  bool operator< (const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//  template <class T, class Tag>
//  bool operator!=(const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//  template <class T, class Tag>
//  bool operator> (const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//  template <class T, class Tag>
//  bool operator>=(const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//  template <class T, class Tag>
//  bool operator<=(const intrusive_forward_list<T,Tag>& x,
//                  const intrusive_forward_list<T,Tag>& y);
//
//  // Specialized algorithms:
//  template <class T, class Tag>
//  void swap(intrusive_forward_list<T,Tag>& x,
//            intrusive_forward_list<T,Tag>& y);

PYCPP_END_NAMESPACE
