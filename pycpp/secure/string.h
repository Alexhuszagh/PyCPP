//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure string implementation.
 *
 *  A secure string that wipes the buffer upon reallocation or deallocation
 *  to avoid passwords or other sensitive information being retained
 *  in memory. You should use secure_string for any sensitive data.
 *
 *  secure_string by default uses a secure memory allocator, from
 *  libsodium, which allows locking access, or setting read-only access
 *  to the buffer. Using a non-secure allocator will create a compile-time
 *  error.
 *
 *  secure_string does not aim for optimization, and therefore it does
 *  not use short-string optimization techniques.
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/misc/compressed_pair.h>
#include <pycpp/secure/allocator.h>
#include <pycpp/secure/char_traits.h>
#include <pycpp/secure/stdlib.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

namespace string_detail
{
// DETAIL
// ------

// Slow route for input iterators
template <typename String, typename ConstIter, typename Iter>
typename std::enable_if<is_input_iterator<Iter>::value, String&>::type
replace(String& string, ConstIter p1, ConstIter p2, Iter first, Iter last)
{
    std::string str(first, last);
    return string.replace(p1, p2, str);
}


// Optimization for forward iterable iterables
template <typename String, typename ConstIter, typename Iter>
typename std::enable_if<is_forward_iterable<Iter>::value, String&>::type
replace(String& string, ConstIter p1, ConstIter p2, Iter first, Iter last)
{
    ConstIter it = string.erase(p1, p2);
    string.insert(it, first, last);
    return string;
}


// Slow route for input iterators
template <typename String, typename ConstIter, typename Iter>
typename std::enable_if<is_input_iterator<Iter>::value, typename String::iterator>::type
insert(String& string, ConstIter p, size_t& size, Iter first, Iter last)
{
    size_t pos = p - string.begin();
    std::string str(first, last);
    string.insert(p, str);

    return string.begin() + pos;
}


// Optimization for forward iterable iterables
template <typename String, typename ConstIter, typename Iter>
typename std::enable_if<is_forward_iterable<Iter>::value, typename String::iterator>::type
insert(String& string, ConstIter p, size_t& size, Iter first, Iter last)
{
    using traits_type = typename String::traits_type;

    // reallocate if necessary
    size_t n = std::distance(first, last);
    size_t pos = p - string.begin();
    size_t move = string.size() - pos;
    size_t new_size = string.size() + n;
    if (new_size >= string.capacity()) {
        string.reserve(std::max(new_size+1, 2 * string.capacity()));
    }

    // move
    char* src = &string[0] + pos;
    char* dst = src + n;
    traits_type::move(dst, src, move);

    // assign
    for (; first != last; ++src, ++first) {
        traits_type::assign(*src, *first);
    }
    // update size
    size = new_size;

    return string.begin() + pos;
}

}   /* string_detail*/

// OBJECTS
// -------

/**
 *  \brief Secure string wrapper.
 *
 *  STL-like string that wipes memory during all allocation events,
 *  to avoid leaving sensitvie information in memory.
 */
template<typename Char, typename Traits = secure_char_traits<Char>, typename Allocator = secure_allocator<Char>>
struct secure_basic_string
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = secure_basic_string<Char, Traits, Allocator>;
    using view_type = basic_string_view<Char, Traits>;
    using traits_type = Traits;
    using value_type = typename traits_type::char_type;
    using allocator_type = Allocator;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using reference = typename allocator_type::reference;
    using const_reference = typename allocator_type::const_reference;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // MEMBER VARIABLES
    // ----------------
    static const size_type npos = SIZE_MAX;

    // MEMBER FUNCTIONS
    // ----------------
    secure_basic_string();
    secure_basic_string(const allocator_type& alloc);
    secure_basic_string(const self_t& str);
    secure_basic_string(const self_t& str, const allocator_type& alloc);
    secure_basic_string& operator=(const self_t& str);
    secure_basic_string(self_t&& str);
    secure_basic_string(self_t&& str, const allocator_type& alloc);
    secure_basic_string& operator=(self_t&& str);
    ~secure_basic_string();

    secure_basic_string(const self_t& str, size_t pos, size_t len = npos, const allocator_type& alloc = allocator_type());
    secure_basic_string(const_pointer s, const allocator_type& alloc = allocator_type());
    secure_basic_string(const_pointer s, size_t n, const allocator_type& alloc = allocator_type());
    secure_basic_string(size_t n, value_type c, const allocator_type& alloc = allocator_type());
    template <typename Iter> secure_basic_string(Iter first, Iter last, const allocator_type& alloc = allocator_type());
    secure_basic_string(std::initializer_list<value_type> list, const allocator_type& alloc = allocator_type());

    // MEMORY
    void noaccess();
    void readonly();
    void readwrite();
    void mlock();
    void munlock();

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
    self_t& operator+=(const self_t& str);
    self_t& operator+=(const view_type& str);
    self_t& operator+=(const_pointer s);
    self_t& operator+=(value_type c);
    self_t& operator+=(std::initializer_list<value_type> list);
    self_t& append(const self_t& str);
    self_t& append(const view_type& str);
    self_t& append(const self_t& str, size_t subpos, size_t sublen);
    self_t& append(const_pointer s);
    self_t& append(const_pointer s, size_t n);
    self_t& append(size_t n, char c);
    template <typename Iter> self_t& append(Iter first, Iter last);
    self_t& append(std::initializer_list<value_type> list);
    void push_back(value_type c);
    self_t& assign(const self_t& str);
    self_t& assign(const view_type& str);
    self_t& assign(const self_t& str, size_t subpos, size_t sublen = npos);
    self_t& assign(const_pointer s);
    self_t& assign(const_pointer s, size_t n);
    self_t& assign(size_t n, value_type c);
    template <typename Iter> self_t& assign(Iter first, Iter last);
    self_t& assign(std::initializer_list<value_type> list);
    self_t& assign(self_t&& str) noexcept;
    self_t& insert(size_t pos, const basic_string_view<Char, Traits>& str);
    self_t& insert(size_t pos, const basic_string_view<Char, Traits>& str, size_t subpos, size_t sublen);
    self_t& insert(size_t pos, const char* s);
    self_t& insert(size_t pos, const char* s, size_t n);
    self_t& insert(size_t pos, size_t n, char c);
    iterator insert(const_iterator p, size_t n, char c);
    iterator insert(const_iterator p, char c);
    template <typename Iter> iterator insert(iterator p, Iter first, Iter last);
    template <typename Iter> iterator insert(const_iterator p, Iter first, Iter last);
    self_t& insert(const_iterator p, std::initializer_list<char> list);
    self_t& erase(size_t pos = 0, size_t len = npos);
    const_iterator erase(const_iterator p);
    const_iterator erase(const_iterator first, const_iterator last);
    self_t& replace(size_t pos, size_t len, const basic_string_view<Char, Traits>& str);
    self_t& replace(const_iterator p1, const_iterator p2, const basic_string_view<Char, Traits>& str);
    self_t& replace(size_t pos, size_t len, const basic_string_view<Char, Traits>& str, size_t subpos, size_t sublen);
    self_t& replace(size_t pos, size_t len, size_t n, char c);
    self_t& replace(const_iterator p1, const_iterator p2, size_t n, char c);
    template <typename Iter> self_t& replace(const_iterator p1, const_iterator p2, Iter first, Iter last);
    self_t& replace(const_iterator p1, const_iterator p2, std::initializer_list<char> list);
    void pop_back();
    void swap(self_t& other);

    // STRING OPERATIONS
    const_pointer c_str() const noexcept;
    const_pointer data() const noexcept;
    view_type view() const noexcept;
    allocator_type get_allocator() const noexcept;
    size_t copy(pointer s, size_t len, size_t pos = 0) const;

    // FIND
    size_type find(const self_t& str, size_type pos = 0) const noexcept;
    size_type find(const view_type& str, size_type pos = 0) const noexcept;
    size_type find(const_pointer array, size_type pos = 0) const;
    size_type find(const_pointer cstring, size_type pos, size_type length) const;
    size_type find(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST OF
    size_type find_first_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_first_of(const view_type& str, size_type pos = 0) const noexcept;
    size_type find_first_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST NOT OF
    size_type find_first_not_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const view_type& str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;

    // RFIND
    size_type rfind(const self_t& str, size_type pos = npos) const noexcept;
    size_type rfind(const view_type& str, size_type pos = npos) const noexcept;
    size_type rfind(const_pointer array, size_type pos = npos) const;
    size_type rfind(const_pointer cstring, size_type pos, size_type length) const;
    size_type rfind(value_type c, size_type pos = npos) const noexcept;

    // FIND LAST OF
    size_type find_last_of(const self_t& str, size_type pos = npos) const noexcept;
    size_type find_last_of(const view_type& str, size_type pos = npos) const noexcept;
    size_type find_last_of(const_pointer array, size_type pos = npos) const;
    size_type find_last_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_of(value_type c, size_type pos = npos) const noexcept;

    // FIND LAST NOT OF
    size_type find_last_not_of(const self_t& str, size_type pos = npos) const noexcept;
    size_type find_last_not_of(const view_type& str, size_type pos = npos) const noexcept;
    size_type find_last_not_of(const_pointer array, size_type pos = npos) const;
    size_type find_last_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;

    // COMPARE
    int compare(const self_t& str) const noexcept;
    int compare(const view_type& str) const noexcept;
    int compare(size_type pos, size_type len, const self_t& str) const;
    int compare(size_type pos, size_type len, const view_type& str) const;
    int compare(size_type pos, size_type len, const self_t& str, size_type subpos, size_type sublen) const;
    int compare(size_type pos, size_type len, const view_type& str, size_type subpos, size_type sublen) const;
    int compare(const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s, size_type n) const;

    self_t substr(size_type pos = 0, size_type len = npos) const;

    // CONVERSIONS
    explicit operator bool() const;
    explicit operator view_type() const;

private:
    size_type capacity_;
    size_type length_;
    compressed_pair<pointer, allocator_type> data_;

    void init();
    void reset();
    void reallocate(size_type n);

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename C, typename T, typename A>
    friend void swap(secure_basic_string<C, T, A>& lhs, secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend std::basic_istream<C, T>& operator>>(std::basic_istream<C, T>& stream, secure_basic_string<C, T, A>& str);

    template <typename C, typename T, typename A>
    friend std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& stream, secure_basic_string<C, T, A>& str);

    // RELATIONAL OPERATORS
    template <typename C, typename T, typename A>
    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator==(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator==(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator==(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator!=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator!=(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator!=(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator<(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<=(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator<=(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator>(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>=(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend bool operator>=(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    // CONCATENATION OPERATORS
    // Do not overload `std::string`, since it cannot securely
    // store data.
    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, secure_basic_string<C, T, A>&& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, secure_basic_string<C, T, A>&& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, const C* rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const C* lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const C* lhs, secure_basic_string<C, T, A>&& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, C rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, C rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(C lhs, const secure_basic_string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend secure_basic_string<C, T, A> operator+(C lhs, secure_basic_string<C, T, A>&& rhs);

    template <typename C, typename T1, typename A, typename T2>
    friend secure_basic_string<C, T1, A> operator+(const secure_basic_string<C, T1, A>& lhs, const basic_string_view<C, T2>& rhs);

    template <typename C, typename T1, typename A, typename T2>
    friend secure_basic_string<C, T1, A> operator+(secure_basic_string<C, T1, A>&& lhs, const basic_string_view<C, T2>& rhs);

    template <typename C, typename T1, typename A, typename T2>
    friend secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs, const secure_basic_string<C, T1, A>& rhs);

    template <typename C, typename T1, typename A, typename T2>
    friend secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs, secure_basic_string<C, T1, A>&& rhs);
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
    return stream.read(const_cast<char*>(str.data_.first()), str.length_);
}


template <typename C, typename T, typename A>
std::basic_ostream<C, T> & operator<<(std::basic_ostream<C, T> &stream, secure_basic_string<C, T, A>& str)
{
    return stream.write(str.data(), str.length());
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && T::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}


template <typename C, typename T, typename A>
bool operator==(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T, typename A>
bool operator==(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T, typename A>
bool operator==(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{

    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename C, typename T, typename A>
bool operator<(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs < rhs.view();
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() < rhs;
}


template <typename C, typename T, typename A>
bool operator<(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) < rhs;
}


template <typename C, typename T, typename A>
bool operator<(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return lhs < basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename C, typename T, typename A>
bool operator<=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs <= rhs.view();
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() <= rhs;
}


template <typename C, typename T, typename A>
bool operator<=(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) <= rhs;
}


template <typename C, typename T, typename A>
bool operator<=(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return lhs <= basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return rhs < lhs;
}


template <typename C, typename T, typename A>
bool operator>(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs > rhs.view();
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() > rhs;
}


template <typename C, typename T, typename A>
bool operator>(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) > rhs;
}


template <typename C, typename T, typename A>
bool operator>(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return lhs > basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const basic_string_view<C, T>& lhs, const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs >= rhs.view();
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() >= rhs;
}


template <typename C, typename T, typename A>
bool operator>=(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) >= rhs;
}


template <typename C, typename T, typename A>
bool operator>=(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    return lhs >= basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, secure_basic_string<C, T, A>&& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, const secure_basic_string<C, T, A>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, secure_basic_string<C, T, A>&& rhs)
{
    return std::move(rhs.insert(0, lhs.data(), lhs.size()));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, const C* rhs)
{
    secure_basic_string<C, T, A> output;
    size_t rhs_length = T::length(rhs);
    output.reserve(lhs.size() + rhs_length);
    output.append(lhs.data(), lhs.size());
    output.append(rhs);
    return output;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, const C* rhs)
{
    return std::move(lhs.append(rhs));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const C* lhs, const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    size_t lhs_length = T::length(lhs);
    output.reserve(lhs_length + rhs.size());
    output.append(lhs, lhs_length);
    output.append(rhs.data(), rhs.size());
    return  output;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const C* lhs, secure_basic_string<C, T, A>&& rhs)
{
    return std::move(rhs.insert(0, lhs, T::length(lhs)));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs, C rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(lhs.size() + 1);
    output.append(lhs.data(), lhs.size());
    output.push_back(rhs);
    return output;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs, C rhs)
{
    lhs.push_back(rhs);
    return std::move(lhs);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(C lhs, const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(1 + rhs.size());
    output.push_back(lhs);
    output.append(rhs.data(), rhs.size());
    return  output;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A> operator+(C lhs, secure_basic_string<C, T, A>&& rhs)
{
    return std::move(rhs.insert(size_t(0), size_t(1), lhs));
}


template <typename C, typename T1, typename A, typename T2>
secure_basic_string<C, T1, A> operator+(const secure_basic_string<C, T1, A>& lhs, const basic_string_view<C, T2>& rhs)
{
    secure_basic_string<C, T1, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T1, typename A, typename T2>
secure_basic_string<C, T1, A> operator+(secure_basic_string<C, T1, A>&& lhs, const basic_string_view<C, T2>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T1, typename A, typename T2>
secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs, const secure_basic_string<C, T1, A>& rhs)
{
    secure_basic_string<C, T1, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T1, typename A, typename T2>
secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs, secure_basic_string<C, T1, A>&& rhs)
{
    return std::move(rhs.insert(0, lhs.data(), lhs.size()));
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string():
    capacity_(15),
    length_(0)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), 15);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const allocator_type& alloc):
    capacity_(15),
    length_(0),
    data_(alloc)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), 15);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const self_t& str):
    capacity_(str.capacity_),
    length_(str.length_)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), str.capacity_);
    traits_type::copy(data_.first(), str.data_.first(), capacity_);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const self_t& str, const allocator_type& alloc):
    capacity_(str.capacity_),
    length_(str.length_),
    data_(alloc)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), str.capacity_);
    traits_type::copy(data_.first(), str.data_.first(), capacity_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator=(const self_t& str) -> self_t&
{
    if (this != &str) {
        reset();
        capacity_ = str.capacity_;
        length_ = str.length_;
        data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), str.capacity_);
        traits_type::copy(data_.first(), str.data_.first(), capacity_);
    }

    return *this;
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(self_t&& str):
    capacity_(15),
    length_(0)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), 15);
    swap(str);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(self_t&& str, const allocator_type& alloc):
    capacity_(15),
    length_(0),
    data_(alloc)
{
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), 15);
    swap(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator=(self_t&& str) -> self_t&
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
secure_basic_string<C, T, A>::secure_basic_string(const self_t& str, size_t pos, size_t len, const allocator_type& alloc):
    data_(alloc)
{
    size_type n = str.size();
    if (pos > n) {
        throw std::out_of_range("secure_basic_string::secure_basic_string().");
    }

    length_ = std::min(len, n - pos);
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);
    traits_type::copy(data_.first(), str.data() + pos, length_);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const_pointer s, const allocator_type& alloc):
    data_(alloc)
{
    length_ = traits_type::length(s);
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);
    traits_type::copy(data_.first(), s, length_);
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(const_pointer s, size_t n, const allocator_type& alloc):
    data_(alloc)
{
    length_ = n;
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);
    traits_type::copy(data_.first(), s, n);
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(size_t n, value_type c, const allocator_type& alloc):
    data_(alloc)
{
    length_ = n;
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);
    traits_type::assign(data_.first(), n, c);
    data_.first()[n] = value_type();
}


template <typename C, typename T, typename A>
template <typename Iter>
secure_basic_string<C, T, A>::secure_basic_string(Iter first, Iter last, const allocator_type& alloc):
    data_(alloc)
{
    length_ = std::distance(first, last);
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);

    size_t n = 0;
    for (; first != last; ++first) {
        data_.first()[n++] = *first;
    }
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::secure_basic_string(std::initializer_list<value_type> list, const allocator_type& alloc):
    data_(alloc)
{
    length_ = list.size();
    capacity_ = length_ + 1;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), capacity_);
    traits_type::copy(data_.first(), list.begin(), length_);
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::noaccess()
{
    secure_mprotect_noaccess(data_.first());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::readonly()
{
    secure_mprotect_readonly(data_.first());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::readwrite()
{
    secure_mprotect_readwrite(data_.first());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::mlock()
{
    secure_mlock(data_.first(), capacity_ * sizeof(value_type));
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::munlock()
{
    secure_munlock(data_.first(), capacity_ * sizeof(value_type));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::begin() -> iterator
{
    return const_cast<iterator>(data_.first());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::end() -> iterator
{
    return begin() + length_;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::begin() const -> const_iterator
{
    return data_.first();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::end() const -> const_iterator
{
    return data_.first() + length_;
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
    return allocator_traits<allocator_type>::max_size(data_.second());
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
        data_.first()[n] = value_type();
    }
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator[](size_type pos) -> reference
{
    assert(pos <= size() && "string index out of bounds");
    return *(data_.first() + pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator[](size_type pos) const -> const_reference
{
    assert(pos <= size() && "string index out of bounds");
    return *(data_.first() + pos);
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
    return *(data_.first());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::front() const -> const_reference
{
    assert(!empty() && "string::front(): string is empty");
    return *data_.first();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::back() -> reference
{
    assert(!empty() && "string::back(): string is empty");
    return *(data_.first() + length_ - 1);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::back() const -> const_reference
{
    assert(!empty() && "string::back(): string is empty");
    return *(data_.first() + length_ - 1);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const self_t& str) -> self_t&
{
    return append(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const view_type& str) -> self_t&
{
    return append(str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(const_pointer s) -> self_t&
{
    return append(s);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(value_type c) -> self_t&
{
    push_back(c);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::operator+=(std::initializer_list<value_type> list) -> self_t&
{
    return append(list);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const self_t& str) -> self_t&
{
    return append(view_type(str));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const view_type& str) -> self_t&
{
    size_t n = str.length();
    size_t r = length() + n;
    if (r >= capacity()) {
        reallocate(std::max(r+1, 2 * capacity()));
    }

    traits_type::copy(data_.first() + length(), str.data(), n);
    length_ += n;
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const self_t& str, size_t subpos, size_t sublen) -> self_t&
{
    return append(view_type(str).substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const_pointer s) -> self_t&
{
    return append(view_type(s));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(const_pointer s, size_t n) -> self_t&
{
    return append(view_type(s, n));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(size_t n, char c) -> self_t&
{
    size_t r = length() + n;
    if (r >= capacity()) {
        reallocate(std::max(r+1, 2 * capacity()));
    }

    while (n--) {
        data_.first()[length_++] = c;
    }
    data_.first()[length_] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::append(Iter first, Iter last) -> self_t&
{
    size_t n = std::distance(first, last);
    size_t r = length() + n;
    if (r >= capacity()) {
        reallocate(std::max(r+1, 2 * capacity()));
    }

    for (; first != last; ++first) {
        data_.first()[length_++] = *first;
    }
    data_.first()[length_] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::append(std::initializer_list<value_type> list) -> self_t&
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
    data_.first()[length_++] = c;
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const self_t& str) -> self_t&
{
    return assign(view_type(str));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const view_type& str) -> self_t&
{
    size_t n = str.length();
    size_t r = length() + n;
    if (r >= capacity()) {
        reallocate(std::max(r+1, 2 * capacity()));
    }

    length_ = n;
    traits_type::copy(data_.first(), str.data(), n);
    data_.first()[n] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const self_t& str, size_t subpos, size_t sublen) -> self_t&
{
    return assign(view_type(str).substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const_pointer s) -> self_t&
{
    assert(s && "secure_basic_string:: assign() nullptr.");
    return assign(view_type(s));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(const_pointer s, size_t n) -> self_t&
{
    assert(s && "secure_basic_string:: assign() nullptr.");
    return assign(view_type(s, n));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(size_t n, value_type c) -> self_t&
{
    size_t r = length() + n;
    if (r >= capacity()) {
        reallocate(std::max(r+1, 2 * capacity()));
    }

    length_ = n;
    traits_type::assign(data_.first(), n, c);
    data_.first()[n] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::assign(Iter first, Iter last) -> self_t&
{
    return assign(self_t(first, last));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(std::initializer_list<value_type> list) -> self_t&
{
    return assign(list.begin(), list.size());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::assign(self_t&& str) noexcept -> self_t&
{
    swap(str);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(size_t pos, const basic_string_view<C, T>& str) -> self_t&
{
    return insert(pos, str.data(), str.size());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(size_t pos, const basic_string_view<C, T>& str, size_t subpos, size_t sublen) -> self_t&
{
    return insert(pos, str.substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(size_t pos, const char* s) -> self_t&
{
    return insert(pos, s, traits_type::length(s));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(size_t pos, const char* s, size_t n) -> self_t&
{
    if (pos > size()) {
        throw std::out_of_range("secure_basic_string::erase().");
    }

    const_iterator p = begin() + pos;
    insert(p, s, s + n);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(size_t pos, size_t n, char c) -> self_t&
{
    if (pos > size()) {
        throw std::out_of_range("secure_basic_string::insert().");
    }

    const_iterator p = begin() + pos;
    insert(p, n, c);
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(const_iterator p, size_t n, char c) -> iterator
{
    // reallocate if necessary
    size_t pos = p - begin();
    size_t move = size() - pos;
    size_t new_size = size() + n;
    if (new_size > capacity()) {
        reserve(new_size);
    }

    // move
    char* src = data_.first() + pos;
    char* dst = src + n;
    traits_type::move(dst, src, move);

    // assign
    traits_type::assign(src, n, c);
    length_ += n;

    return begin() + pos;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(const_iterator p, char c) -> iterator
{
    return insert(p, 1, c);
}



template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::insert(iterator p, Iter first, Iter last) -> iterator
{
    return insert(const_iterator(p), first, last);
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::insert(const_iterator p, Iter first, Iter last) -> iterator
{
    return string_detail::insert(*this, p, length_, first, last);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::insert(const_iterator p, std::initializer_list<char> list) -> self_t&
{
    insert(p, list.begin(), list.end());
    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::erase(size_t pos, size_t len) -> self_t&
{
    if (pos > size()) {
        throw std::out_of_range("secure_basic_string::erase().");
    }

    size_type move = size() - pos - len;
    if (move != 0) {
        traits_type::move(data_.first() + pos, data_.first() + pos + len, move);
    }
    length_ -= len;
    data_.first()[length_] = value_type();

    return *this;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::erase(const_iterator p) -> const_iterator
{
    assert(p != end() && "Erase called with a non-dereferencable iterator.");

    size_type pos = static_cast<size_type>(p - begin());
    return erase(pos, 1);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::erase(const_iterator first, const_iterator last) -> const_iterator
{
    size_type r = first - begin();
    erase(r, last - first);

    return begin() + r;
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(size_t pos, size_t len, const basic_string_view<C, T>& str) -> self_t&
{
    if (pos > size()) {
        throw std::out_of_range("secure_basic_string::replace().");
    }

    const_iterator p1 = begin() + pos;
    const_iterator p2 = (len == npos || pos + len >= size()) ? end() : p1 + len;
    return replace(p1, p2, str);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(const_iterator p1, const_iterator p2, const basic_string_view<C, T>& str) -> self_t&
{
    return replace(p1, p2, str.begin(), str.end());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(size_t pos, size_t len, const basic_string_view<C, T>& str, size_t subpos, size_t sublen) -> self_t&
{
    return replace(pos, len, str.substr(subpos, sublen));
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(size_t pos, size_t len, size_t n, char c) -> self_t&
{
    if (pos > size()) {
        throw std::out_of_range("secure_basic_string::replace().");
    }

    const_iterator p1 = begin() + pos;
    const_iterator p2 = (len == npos || pos + len >= size()) ? end() : p1 + len;
    return replace(p1, p2, n, c);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(const_iterator p1, const_iterator p2, size_t n, char c) -> self_t&
{
    auto it = erase(p1, p2);
    insert(it, n, c);
    return *this;
}


template <typename C, typename T, typename A>
template <typename Iter>
auto secure_basic_string<C, T, A>::replace(const_iterator p1, const_iterator p2, Iter first, Iter last) -> self_t&
{
    return string_detail::replace(*this, p1, p2, first, last);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::replace(const_iterator p1, const_iterator p2, std::initializer_list<char> list) -> self_t&
{
    return replace(p1, p2, list.begin(), list.end());
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::pop_back()
{
    assert(!empty() && "string::pop_back(): string is empty");
    --length_;
    data_.first()[length_] = value_type();
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::swap(self_t& other)
{
    std::swap(capacity_, other.capacity_);
    std::swap(length_, other.length_);
    std::swap(data_, other.data_);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::c_str() const noexcept -> const_pointer
{
    return data_.first();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::data() const noexcept -> const_pointer
{
    return data_.first();
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::view() const noexcept -> view_type
{
    return view_type(data(), size());
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::get_allocator() const noexcept -> allocator_type
{
    return data_.second;
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
auto secure_basic_string<C, T, A>::find(const self_t& str, size_type pos) const noexcept -> size_type
{
    return find(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).find(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const_pointer s, size_type pos) const -> size_type
{
    return find(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find(value_type c, size_type pos) const noexcept -> size_type
{
    return find(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    return find_first_of(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).find_first_of(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const_pointer s, size_type pos) const -> size_type
{
    return find_first_of(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_first_of(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_first_of(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    return find_first_not_of(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).find_first_not_of(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const_pointer s, size_type pos) const -> size_type
{
    return find_first_not_of(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_first_not_of(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_first_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_first_not_of(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const self_t& str, size_type pos) const noexcept -> size_type
{
    return rfind(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).rfind(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const_pointer s, size_type pos) const -> size_type
{
    return rfind(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return rfind(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::rfind(value_type c, size_type pos) const noexcept -> size_type
{
    return rfind(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    return find_last_of(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).find_last_of(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const_pointer s, size_type pos) const -> size_type
{
    return find_last_of(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_last_of(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_last_of(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    return find_last_not_of(view_type(str), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const view_type& str, size_type pos) const noexcept -> size_type
{
    return view_type(*this).find_last_not_of(str, pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const_pointer s, size_type pos) const -> size_type
{
    return find_last_not_of(view_type(s), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_last_not_of(view_type(s, n), pos);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::find_last_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_last_not_of(view_type(&c, 1), pos);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const self_t& str) const noexcept
{
    return compare(view_type(str));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const view_type& str) const noexcept
{
    return view_type(*this).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const self_t& str) const
{
    return view_type(*this, pos, len).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const view_type& str) const
{
    return view_type(*this, pos, len).compare(str);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len,
    const self_t& str,
    size_type subpos,
    size_type sublen) const
{
    return view_type(*this, pos, len).compare(view_type(str, subpos, sublen));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const view_type& str, size_type subpos, size_type sublen) const
{
    return view_type(*this, pos, len).compare(view_type(str, subpos, sublen));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(const_pointer s) const
{
    return compare(view_type(s));
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const_pointer s) const
{
    return view_type(*this, pos, len).compare(s);
}


template <typename C, typename T, typename A>
int secure_basic_string<C, T, A>::compare(size_type pos, size_type len, const_pointer s, size_type n) const
{
    return view_type(*this, pos, len).compare(s, n);
}


template <typename C, typename T, typename A>
auto secure_basic_string<C, T, A>::substr(size_type pos, size_type len) const -> self_t
{
    return self_t(*this, pos, len);
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::operator bool() const
{
    return !empty();
}


template <typename C, typename T, typename A>
secure_basic_string<C, T, A>::operator view_type() const
{
    return view_type(data_.first(), length_);
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::init()
{
    capacity_ = 15;
    length_ = 0;
    data_.first() = allocator_traits<allocator_type>::allocate(data_.second(), 15);
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::reset()
{
    allocator_traits<allocator_type>::deallocate(data_.second(), data_.first(), capacity_);
    capacity_ = length_ = 0;
}


template <typename C, typename T, typename A>
void secure_basic_string<C, T, A>::reallocate(size_type n)
{
    // create new buffer
    pointer buf = allocator_traits<allocator_type>::allocate(data_.second(), n+1, data_.first());
    traits_type::copy(buf, data_.first(), length_+1);

    // clear existing buffer
    allocator_traits<allocator_type>::deallocate(data_.second(), data_.first(), capacity_);

    // store data
    capacity_ = n;
    data_.first() = buf;
}

// ALIAS
// -----

using secure_string = secure_basic_string<char>;
using secure_wstring = secure_basic_string<wchar_t>;
using secure_u16string = secure_basic_string<char16_t>;
using secure_u32string = secure_basic_string<char32_t>;

PYCPP_END_NAMESPACE


namespace std
{
// SPECIALIZATION
// --------------

PYCPP_USING_NAMESPACE

PYCPP_SPECIALIZE_HASH_STRING(hash, secure_string);
PYCPP_SPECIALIZE_HASH_STRING(hash, secure_wstring);
PYCPP_SPECIALIZE_HASH_STRING(hash, secure_u16string);
PYCPP_SPECIALIZE_HASH_STRING(hash, secure_u32string);

}   /* std */


PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

PYCPP_SPECIALIZE_HASH_REFERENCE(hash, secure_string);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, secure_wstring);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, secure_u16string);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, secure_u32string);

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
