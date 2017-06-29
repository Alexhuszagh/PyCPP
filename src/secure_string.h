//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Secure string implementation.
 *
 *  A secure string that wipes the buffer upon reallocation or deallocation
 *  to avoid passwords or other sensitive information being retained
 *  in memory. You should use secure_string for any sensitive data.
 *
 *  secure_string doesn't aim for optimization, and therefore it does
 *  not use short-string optimization techniques.
 */

#pragma once

#include "string_view.h"

// FUNCTIONS
// ---------

/**
 *  \brief Securely clear buffer, preventing compiler optimizations.
 *
 *  Use this rather memset.
 */
void secure_zero(void* dst, size_t bytes);

// OBJECTS
// -------


/**
 *  \brief Secure string wrapper.
 *
 *  STL-like string that wipes memory during all allocation events,
 *  to avoid leaving sensitvie information in memory.
 */
template<typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
struct secure_basic_string
{
public:
    // MEMBER TYPES
    // ------------
    typedef secure_basic_string<Char, Traits, Allocator> self;
    typedef basic_string_view<Char, Traits> view;
    typedef Traits traits_type;
    typedef typename traits_type::char_type value_type;
    typedef Allocator allocator_type;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER VARIABLES
    // ----------------
    static const size_type npos = SIZE_MAX;

    // MEMBER FUNCTIONS
    // ----------------
    secure_basic_string();
    secure_basic_string(const self& str);
    secure_basic_string& operator=(const self& str);
    secure_basic_string(self&& str);
    secure_basic_string& operator=(self&& str);
    ~secure_basic_string();

    secure_basic_string(const self& str, size_t pos, size_t len = npos);
    secure_basic_string(const_pointer s);
    secure_basic_string(const_pointer s, size_t n);
    secure_basic_string(size_t n, value_type c);
    template <typename Iter> secure_basic_string(Iter first, Iter last);

    // ITERATORS
    iterator begin();
    iterator end();
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
    size_type length() const;
    size_type capacity() const;
    bool empty() const noexcept;
    size_type max_size() const noexcept;
    void clear();
    void reserve(size_t n = 0);
    void shrink_to_fit();
    void resize(size_type n);
    void resize(size_type n, value_type c);

    // ELEMENT ACCESS
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // MODIFIERS
    self& operator+=(const self& str);
    self& operator+=(const view& str);
    self& operator+=(const_pointer s);
    self& operator+=(value_type c);
    self& operator+=(std::initializer_list<value_type> list);
    self& append(const self& str);
    self& append(const view& str);
    self& append(const self& str, size_t subpos, size_t sublen);
    self& append(const_pointer s);
    self& append(const_pointer s, size_t n);
    self& append(size_t n, char c);
    template <typename Iter> self& append(Iter first, Iter last);
    self& append(std::initializer_list<value_type> list);
    void push_back(value_type c);
    self& assign(const self& str);
    self& assign(const view& str);
    self& assign(const self& str, size_t subpos, size_t sublen = npos);
    self& assign(const_pointer s);
    self& assign(const_pointer s, size_t n);
    self& assign(size_t n, value_type c);
    template <typename Iter> self& assign(Iter first, Iter last);
    self& assign(std::initializer_list<value_type> list);
    self& assign(self&& str) noexcept;
//    insert();
//    erase();
//    replace();
    void pop_back();
    void swap(self& other);

    // STRING OPERATIONS
    const_pointer c_str() const noexcept;
    const_pointer data() const noexcept;
    allocator_type get_allocator() const noexcept;
    size_t copy(pointer s, size_t len, size_t pos = 0) const;

    // FIND
    size_type find(const self& str, size_type pos = 0) const noexcept;
    size_type find(const view& str, size_type pos = 0) const noexcept;
    size_type find(const_pointer array, size_type pos = 0) const;
    size_type find(const_pointer cstring, size_type pos, size_type length) const;
    size_type find(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST OF
    size_type find_first_of(const self& str, size_type pos = 0) const noexcept;
    size_type find_first_of(const view& str, size_type pos = 0) const noexcept;
    size_type find_first_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST NOT OF
    size_type find_first_not_of(const self& str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const view& str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;

    // RFIND
    size_type rfind(const self& str, size_type pos = 0) const noexcept;
    size_type rfind(const view& str, size_type pos = 0) const noexcept;
    size_type rfind(const_pointer array, size_type pos = 0) const;
    size_type rfind(const_pointer cstring, size_type pos, size_type length) const;
    size_type rfind(value_type c, size_type pos = 0) const noexcept;

    // FIND LAST OF
    size_type find_last_of(const self& str, size_type pos = 0) const noexcept;
    size_type find_last_of(const view& str, size_type pos = 0) const noexcept;
    size_type find_last_of(const_pointer array, size_type pos = 0) const;
    size_type find_last_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_of(value_type c, size_type pos = 0) const noexcept;

    // FIND LAST NOT OF
    size_type find_last_not_of(const self& str, size_type pos = 0) const noexcept;
    size_type find_last_not_of(const view& str, size_type pos = 0) const noexcept;
    size_type find_last_not_of(const_pointer array, size_type pos = 0) const;
    size_type find_last_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_not_of(value_type c, size_type pos = 0) const noexcept;

    // COMPARE
    int compare(const self& str) const noexcept;
    int compare(const view& str) const noexcept;
    int compare(size_type pos, size_type len, const self& str) const;
    int compare(size_type pos, size_type len, const view& str) const;
    int compare(size_type pos, size_type len, const self& str, size_type subpos, size_type sublen) const;
    int compare(size_type pos, size_type len, const view& str, size_type subpos, size_type sublen) const;
    int compare(const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s, size_type n) const;

    self substr(size_type pos = 0, size_type len = npos) const;

    // CONVERSIONS
    explicit operator bool() const;
    explicit operator view() const;

private:
    size_type capacity_;
    size_type length_;
    pointer data_;

    void init();
    void reset();
    void reallocate(size_type n);

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename C, typename T, typename A>
    friend void swap(secure_basic_string<C, T, A>& lhs, secure_basic_string<C, T, A>& rhs);

//    template <typename C, typename T, typename A>
//    friend istream<C, T>& operator>>(istream<C, T>& stream, secure_basic_string<C, T, A>& str);
//
//    template <typename C, typename T, typename A>
//    friend ostream<C, T>& operator<<(ostream<C, T>& stream, secure_basic_string<C, T, A>& str);
//
//    // RELATIONAL OPERATORS
//    template <typename C, typename T, typename A>
//    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator==(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator==(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const C* rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator!=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator!=(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const C* rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const C* rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator<=(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const C* rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const C* rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;
//
//    template <typename C, typename T, typename A>
//    friend bool operator>=(const C* lhs, const secure_basic_string<C, T, A>& rhs);
//
//    template <typename C, typename T, typename A>
//    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const C* rhs);
};


// IMPLEMENTATION
// --------------


template <typename C, typename T, typename A>
const typename secure_basic_string<C, T, A>::size_type secure_basic_string<C, T, A>::npos;

template <typename C, typename T, typename A>
void swap(secure_basic_string<C, T, A>& lhs, secure_basic_string<C, T, A>& rhs)
{
    lhs.swap(rhs);
}


template <typename C, typename T, typename A>
std::basic_istream<C, T> & operator>>(std::basic_istream<C, T> &stream, secure_basic_string<C, T, A>& str)
{
    return stream.read(const_cast<char*>(str.data_), str.length_);
}


template <typename C, typename T, typename A>
std::basic_ostream<C, T> & operator<<(std::basic_ostream<C, T> &stream, secure_basic_string<C, T, A>& str)
{
    return stream.write(str.data(), str.length());
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    size_t left_size = left.size();
    return left_size == rhs.size() && T::compare(left.data(), rhs.data(), left_size) == 0;
}


template <typename C, typename T, typename A>
bool operator==(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return secure_basic_string<C, T, A>(left) == rhs;
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& left, const basic_string_view<C, T>& rhs) noexcept
{
    return left == secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator==(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return secure_basic_string<C, T, A>(left) == rhs;
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return left == secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{

    return !(left == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(left == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& left,
    const basic_string_view<C, T>& rhs) noexcept
{
    return !(left == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return !(left == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return !(left == rhs);
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return left.compare(rhs) < 0;
}


template <typename C, typename T, typename A>
bool operator<(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return secure_basic_string<C, T, A>(left) < rhs;
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& left, const basic_string_view<C, T>& rhs) noexcept
{
    return left < secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator<(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return secure_basic_string<C, T, A>(left) < rhs;
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return left < secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(rhs < left);
}


template <typename C, typename T, typename A>
bool operator<=(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return secure_basic_string<C, T, A>(left) <= rhs;
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& left, const basic_string_view<C, T>& rhs) noexcept
{
    return left <= secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator<=(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return secure_basic_string<C, T, A>(left) <= rhs;
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return left <= secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return rhs < left;
}


template <typename C, typename T, typename A>
bool operator>(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return secure_basic_string<C, T, A>(left) > rhs;
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& left, const basic_string_view<C, T>& rhs) noexcept
{
    return left > secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator>(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return secure_basic_string<C, T, A>(left) > rhs;
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return left > secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(left < rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const basic_string_view<C, T>& left, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return secure_basic_string<C, T, A>(left) >= rhs;
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& left, const basic_string_view<C, T>& rhs) noexcept
{
    return left >= secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const C* left, const secure_basic_string<C, T, A>& rhs)
{
    return secure_basic_string<C, T, A>(left) >= rhs;
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& left, const C* rhs)
{
    return left >= secure_basic_string<C, T, A>(rhs);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string():
    capacity_(15),
    length_(0),
    data_(allocator_type().allocate(15))
{
    // clear buffer on initialization
    secure_zero(data_, capacity_ * sizeof(value_type));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const self& str):
    capacity_(str.capacity_),
    length_(str.length_),
    data_(allocator_type().allocate(str.capacity_))
{
    traits_type::copy(data_, str.data_, capacity_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator=(const self& str) -> self&
{
    reset();
    capacity_ = str.capacity_;
    length_ = str.length_;
    data_ = allocator_type().allocate(str.capacity_);
    traits_type::copy(data_, str.data_, capacity_);
    return *this;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(self&& str):
    capacity_(15),
    length_(0),
    data_(allocator_type().allocate(15))
{
    swap(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator=(self&& str) -> self&
{
    swap(str);
    return *this;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::~secure_basic_string()
{
    reset();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const self& str, size_t pos, size_t len)
{
    size_type n = str.size();
    if (pos > n) {
        throw std::out_of_range("secure_basic_string::secure_basic_string().");
    }

    length_ = std::min(len, n - pos);
    capacity_ = length_ + 1;
    data_ = allocator_type().allocate(capacity_);
    traits_type::copy(data_, str.data() + pos, length_);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const_pointer s)
{
    length_ = traits_type::length(s);
    capacity_ = length_ + 1;
    data_ = allocator_type().allocate(capacity_);
    traits_type::copy(data_, s, length_);
    data_[length_] = value_type();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const_pointer s, size_t n)
{
    length_ = n;
    capacity_ = length_ + 1;
    data_ = allocator_type().allocate(capacity_);
    traits_type::copy(data_, s, n);
    data_[length_] = value_type();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(size_t n, value_type c)
{
    length_ = n;
    capacity_ = length_ + 1;
    data_ = allocator_type().allocate(capacity_);
    traits_type::assign(data_, n, c);
    data_[n] = value_type();
}


template <typename C, typename T, typename A>
template <typename Iter>
secure_basic_string<C, T, A>::secure_basic_string(Iter first, Iter last)
{
    length_ = std::distance(first, last);
    capacity_ = length_ + 1;
    data_ = allocator_type().allocate(capacity_);

    size_t n = 0;
    for (; first != last; ++first) {
        data_[n++] = *first;
    }
    data_[length_] = value_type();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::begin() -> iterator
{
    return const_cast<iterator>(data_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::end() -> iterator
{
    return begin() + length_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::begin() const -> const_iterator
{
    return data_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::end() const -> const_iterator
{
    return data_ + length_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::cbegin() const -> const_iterator
{
    return begin();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::cend() const -> const_iterator
{
    return end();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::crbegin() const -> const_reverse_iterator
{
    return rbegin();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::crend() const -> const_reverse_iterator
{
    return rend();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::size() const -> size_type
{
    return length_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::length() const -> size_type
{
    return length_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::capacity() const -> size_type
{
    return capacity_;
}


template <typename C, typename T, typename A>
bool secure_basic_string<C, T, A>::empty() const noexcept
{
    return length_ == 0;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::max_size() const noexcept -> size_type
{
    return allocator_type().max_size();
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::clear()
{
    if (!empty()) {
        reset();
        init();
    }
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::reserve(size_t n)
{
    if (n > max_size()) {
        throw std::out_of_range("Cannot allocate above max_size.");
    }

    size_t r = std::max(n, size());
    if (r != capacity()) {
        reallocate(r);
    }
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::shrink_to_fit()
{
    reserve();
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::resize(size_type n)
{
    resize(n, value_type());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::resize(size_type n, value_type c)
{
    if (n > length()) {
        append(n - length(), c);
    } else {
        length_ = n;
        data_[n] = value_type();
    }
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator[](size_type pos) -> reference
{
    assert(pos <= size() && "string index out of bounds");
    return *(data_ + pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator[](size_type pos) const -> const_reference
{
    assert(pos <= size() && "string index out of bounds");
    return *(data_ + pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::at(size_type pos) -> reference
{
    return operator[](pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::at(size_type pos) const -> const_reference
{
    return operator[](pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::front() -> reference
{
    assert(!empty() && "string::front(): string is empty");
    return *(data_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::front() const -> const_reference
{
    assert(!empty() && "string::front(): string is empty");
    return *data_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::back() -> reference
{
    assert(!empty() && "string::back(): string is empty");
    return *(data_ + length_ - 1);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::back() const -> const_reference
{
    assert(!empty() && "string::back(): string is empty");
    return *(data_ + length_ - 1);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const self& str) -> self&
{
    return append(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const view& str) -> self&
{
    return append(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const_pointer s) -> self&
{
    return append(s);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(value_type c) -> self&
{
    push_back(c);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(std::initializer_list<value_type> list) -> self&
{
    return append(list);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const self& str) -> self&
{
    return append(view(str));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const view& str) -> self&
{
    size_t n = str.length();
    size_t r = length() + n;
    if (r > capacity()) {
        reallocate(std::max(r, 2 * capacity()));
    }

    traits_type::copy(data() + length(), str.data(), n);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const self& str, size_t subpos, size_t sublen) -> self&
{
    return append(view(str).substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const_pointer s) -> self&
{
    return append(view(s));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const_pointer s, size_t n) -> self&
{
    return append(view(s, n));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(size_t n, char c) -> self&
{
    size_t r = length() + n;
    if (r > capacity()) {
        reallocate(std::max(r, 2 * capacity()));
    }

    while (n--) {
        data_[length_++] = c;
    }
    data_[length_] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::append(Iter first, Iter last) -> self&
{
    size_t n = std::distance(first, last);
    size_t r = length() + n;
    if (r > capacity()) {
        reallocate(std::max(r, 2 * capacity()));
    }

    for (; first != last; ++first) {
        data_[length_++] = *first;
    }
    data_[length_] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(std::initializer_list<value_type> list) -> self&
{
    return append(list.begin(), list.size());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::push_back(value_type c)
{
    if (capacity() == 0) {
        reallocate(15);
    } else if (size() == capacity()-1) {
        reallocate(2 * capacity());
    }

    // assign
    data_[length_++] = c;
    data_[length_] = value_type();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const self& str) -> self&
{
    return assign(view(str));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const view& str) -> self&
{
    size_t n = str.length();
    size_t r = length() + n;
    if (r > capacity()) {
        reallocate(std::max(r, 2 * capacity()));
    }

    length_ = n;
    traits_type::assign(data_, str.data(), n);
    traits_type::assign(data_[n], value_type());

    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const self& str, size_t subpos, size_t sublen) -> self&
{
    return assign(view(str).substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const_pointer s) -> self&
{
    return assign(view(s));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const_pointer s, size_t n) -> self&
{
    return assign(view(s, n));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(size_t n, value_type c) -> self&
{
    size_t r = length() + n;
    if (r > capacity()) {
        reallocate(std::max(r, 2 * capacity()));
    }

    length_ = n;
    traits_type::assign(data_, n, c);
    traits_type::assign(data_[n], value_type());

    return *this;
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::assign(Iter first, Iter last) -> self&
{
    return assign(self(first, last));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(std::initializer_list<value_type> list) -> self&
{
    return assign(list.begin(), list.size());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(self&& str) noexcept -> self&
{
    swap(str);
    return *this;
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::pop_back()
{
    assert(!empty() && "string::pop_back(): string is empty");
    --length_;
    data_[length_] = value_type();
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::swap(self& other)
{
    std::swap(capacity_, other.capacity_);
    std::swap(length_, other.length_);
    std::swap(data_, other.data_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::c_str() const noexcept -> const_pointer
{
    return data_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::data() const noexcept -> const_pointer
{
    return data_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::get_allocator() const noexcept -> allocator_type
{
    return allocator_type();
}


template <typename C, typename T, typename A>
size_t secure_basic_string<C, T, A>::copy(pointer s, size_t len, size_t pos) const
{
    if (pos > length()) {
        throw std::out_of_range("secure_basic_string:: copy().");
    }

    size_type rlen = std::min(len, length() - pos);
    traits_type::copy(s, data() + pos, rlen);
    return rlen;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const view& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::find(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::find(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::find(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const view& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::find_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::find_of(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::find_of(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find_not_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const view& str, size_type pos) const noexcept -> size_type
{
    auto found = ::find_not_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::find_not_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::find_not_of(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::find_not_of(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const view& str, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::rfind(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::rfind(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind_of(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const view& str, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind_of(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::rfind_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::rfind_of(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind_of(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const self& str, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind_not_of(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = ::rfind_not_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = ::rfind_not_of(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = ::rfind_not_of(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const self& str) const noexcept
{
    return compare(view(str));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const view& str) const noexcept
{
    return view(*this).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const self& str) const
{
    return view(*this, pos, len).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const view& str) const
{
    return view(*this, pos, len).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len,
    const self& str,
    size_type subpos,
    size_type sublen) const
{
    return view(*this, pos, len).compare(view(str, subpos, sublen));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const view& str, size_type subpos, size_type sublen) const
{
    return view(*this, pos, len).compare(view(str, subpos, sublen));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const_pointer s) const
{
    return compare(view(s));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const_pointer s) const
{
    return view(*this, pos, len).compare(s);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const_pointer s, size_type n) const
{
    return view(*this, pos, len).compare(s, n);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::substr(size_type pos, size_type len) const -> self
{
    return self(*this, pos, len);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::operator bool() const
{
    return !empty();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::operator view() const
{
    return view(data_, length_);
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::init()
{
    capacity_ = 15;
    length_ = 0;
    data_ = allocator_type().allocate(15);
    secure_zero(data_, capacity_ * sizeof(value_type));
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::reset()
{
    // clear our buffer before deallocating it
    secure_zero(data_, capacity_ * sizeof(value_type));
    allocator_type().deallocate(data_, capacity_);
    capacity_ = length_ = 0;
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::reallocate(size_type n)
{
    // create new buffer
    pointer buf = allocator_type().allocate(n+1, data_);
    traits_type::copy(buf, data_, length_+1);

    // clear existing buffer
    secure_zero(data_, capacity_ * sizeof(value_type));
    allocator_type().deallocate(data_, capacity_);

    // store data
    capacity_ = n;
    data_ = buf;
}


// ALIAS
// -----

typedef secure_basic_string<char> secure_string;
typedef secure_basic_string<wchar_t> secure_wstring;
typedef secure_basic_string<char16_t> secure_u16string;
typedef secure_basic_string<char32_t> secure_u32string;
