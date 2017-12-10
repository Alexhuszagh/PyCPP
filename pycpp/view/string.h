//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Zero-copy string view container.
 */

#pragma once

#include <pycpp/config.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <string>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Searches range1 for first occurence of range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_find(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept;

/**
 *  \brief Searches range1 for first occurence of any character that matches in range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_find_of(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept;

/**
 *  \brief Searches range1 for first occurence of any character not found in range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_find_not_of(Iter1 first, size_t length, Iter2 substr, size_t sublen);

/**
 *  \brief Searches range1 for last occurence of range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_rfind(Iter1 last, size_t length, Iter2 substr, size_t sublen);

/**
 *  \brief Searches range1 for last occurence of any character found in range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_rfind_of(Iter1 last, size_t length, Iter2 substr, size_t sublen);

/**
 *  \brief Searches range1 for last occurence of any character not found in range2.
 */
template <typename Iter1, typename Iter2>
Iter1 string_rfind_not_of(Iter1 last, size_t length, Iter2 substr, size_t sublen);

// OBJECTS
// -------


/**
 *  \brief STL string wrapper.
 *
 *  Binds a pointer and the string length, accepting either a C++
 *  string, a null-terminated string, a character array and length,
 *  or an begin/end pointer pair.
 *
 *  \warning The lifetime of the source data must outlive the wrapper.
 *  The wrapper has **no** ownership, and is merely an STL-like wrapper
 *  for performance reasons.
 */
template <typename Char, typename Traits = std::char_traits<Char>>
class basic_string_view
{
public:
    // MEMBER TEMPLATES
    template <typename C, typename T = std::char_traits<C>>
    using view = basic_string_view<C, T>;

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    using string = std::basic_string<C, T, A>;

    template <typename C, typename T = std::char_traits<C>>
    using istream = std::basic_istream<C, T>;

    template <typename C, typename T = std::char_traits<C>>
    using ostream = std::basic_ostream<C, T>;

    // MEMBER TYPES
    // ------------
    using self_t = basic_string_view<Char, Traits>;
    using stl_type = string<Char, Traits>;
    using value_type = Char;
    using traits_type = Traits;
    using reference = Char&;
    using const_reference = const Char&;
    using pointer = Char*;
    using const_pointer = const Char*;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<pointer>;
    using const_reverse_iterator = std::reverse_iterator<const_pointer>;

    // MEMBER VARIABLES
    // ----------------
    static const size_type npos = SIZE_MAX;

    // MEMBER FUNCTIONS
    // ----------------
    basic_string_view() = default;
    basic_string_view(const self_t& str);
    self_t& operator=(const self_t& str);
    basic_string_view(self_t&& str);
    self_t& operator=(self_t&& str);

    template <typename A> basic_string_view(const string<Char, Traits, A>& str);
    basic_string_view(const self_t& str, size_type pos, size_type len = npos);
    template <typename A> basic_string_view(const string<Char, Traits, A>& str, size_type pos, size_type len = npos);
    basic_string_view(const_pointer str);
    basic_string_view(const_pointer str, size_type n);
    basic_string_view(const_pointer begin, const_pointer end);
    self_t& operator=(const_pointer str);

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
    size_type length() const;
    bool empty() const noexcept;

    // ELEMENT ACCESS
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const;
    const_reference front() const;
    const_reference back() const;

    // MODIFIERS
    self_t& assign(const self_t& str);
    self_t& assign(const stl_type& str);
    self_t& assign(const self_t& str, size_type subpos, size_type sublen);
    self_t& assign(const stl_type& str, size_type subpos, size_type sublen);
    self_t& assign(const_pointer s);
    self_t& assign(const_pointer s, size_type n);
    void swap(self_t& other);

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    self_t& operator--();
    self_t operator--(int);
    self_t& operator+=(size_type shift);
    self_t operator+(size_type shift);
    self_t& operator-=(size_type shift);
    self_t operator-(size_type shift);

    // STRING OPERATIONS
    bool is_null_terminated() const noexcept;
    const_pointer c_str() const noexcept;
    const_pointer data() const noexcept;

    // FIND
    size_type find(const self_t& str, size_type pos = 0) const noexcept;
    size_type find(const stl_type &str, size_type pos = 0) const;
    size_type find(const_pointer array, size_type pos = 0) const;
    size_type find(const_pointer cstring, size_type pos, size_type length) const;
    size_type find(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST OF
    size_type find_first_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_first_of(const stl_type &str, size_type pos = 0) const;
    size_type find_first_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;

    // FIND FIRST NOT OF
    size_type find_first_not_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const stl_type& str, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer array, size_type pos = 0) const;
    size_type find_first_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;

    // RFIND
    size_type rfind(const self_t& str, size_type pos = 0) const noexcept;
    size_type rfind(const stl_type& str, size_type pos = 0) const;
    size_type rfind(const_pointer array, size_type pos = 0) const;
    size_type rfind(const_pointer cstring, size_type pos, size_type length) const;
    size_type rfind(value_type c, size_type pos = 0) const noexcept;

    // FIND LAST OF
    size_type find_last_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_last_of(const stl_type& str, size_type pos = 0) const;
    size_type find_last_of(const_pointer array, size_type pos = 0) const;
    size_type find_last_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_of(value_type c, size_type pos = 0) const noexcept;

    // FIND LAST NOT OF
    size_type find_last_not_of(const self_t& str, size_type pos = 0) const noexcept;
    size_type find_last_not_of(const stl_type &str, size_type pos = 0) const;
    size_type find_last_not_of(const_pointer array, size_type pos = 0) const;
    size_type find_last_not_of(const_pointer cstring, size_type pos, size_type length) const;
    size_type find_last_not_of(value_type c, size_type pos = 0) const noexcept;

    // COMPARE
    int compare(const self_t& str) const noexcept;
    int compare(const stl_type& str) const noexcept;
    int compare(size_type pos, size_type len, const self_t& str) const;
    int compare(size_type pos, size_type len, const stl_type& str) const;
    int compare(size_type pos, size_type len, const self_t& str, size_type subpos, size_type sublen) const;
    int compare(size_type pos, size_type len, const stl_type& str, size_type subpos, size_type sublen) const;
    int compare(const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s) const;
    int compare(size_type pos, size_type len, const_pointer s, size_type n) const;

    self_t substr(size_type pos = 0, size_type len = npos) const;

    // CONVERSIONS
    explicit operator bool() const;
    explicit operator stl_type() const;

private:
    const_pointer data_ = nullptr;
    size_t length_ = 0;

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename C, typename T>
    friend void swap(view<C, T>& lhs, view<C, T>& rhs);

    template <typename C, typename T>
    friend istream<C, T>& operator>>(istream<C, T>& stream, view<C, T>& str);

    template <typename C, typename T>
    friend ostream<C, T>& operator<<(ostream<C, T>& stream, view<C, T>& str);

    // RELATIONAL OPERATORS
    template <typename C, typename T>
    friend bool operator==(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator==(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator==(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator==(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator==(const view<C, T>& lhs, const C* rhs);

    template <typename C, typename T>
    friend bool operator!=(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator!=(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator!=(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator!=(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator!=(const view<C, T>& lhs, const C* rhs);

    template <typename C, typename T>
    friend bool operator<(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator<(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator<(const view<C, T>& lhs, const C* rhs);

    template <typename C, typename T>
    friend bool operator<=(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<=(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator<=(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator<=(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator<=(const view<C, T>& lhs, const C* rhs);

    template <typename C, typename T>
    friend bool operator>(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator>(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator>(const view<C, T>& lhs, const C* rhs);

    template <typename C, typename T>
    friend bool operator>=(const view<C, T>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>=(const string<C, T, A>& lhs, const view<C, T>& rhs) noexcept;

    template <typename C, typename T, typename A>
    friend bool operator>=(const view<C, T>& lhs, const string<C, T, A>& rhs) noexcept;

    template <typename C, typename T>
    friend bool operator>=(const C* lhs, const view<C, T>& rhs);

    template <typename C, typename T>
    friend bool operator>=(const view<C, T>& lhs, const C* rhs);

    // CONCATENATION OPERATORS
    template <typename C, typename T, typename A>
    friend string<C, T, A> operator+(const string<C, T, A>& lhs, const view<C, T>& rhs);

    template <typename C, typename T, typename A>
    friend string<C, T, A> operator+(string<C, T, A>&& lhs, const view<C, T>& rhs);

    template <typename C, typename T, typename A>
    friend string<C, T, A> operator+(const view<C, T>& lhs, const string<C, T, A>& rhs);

    template <typename C, typename T, typename A>
    friend string<C, T, A> operator+(const view<C, T>& lhs, string<C, T, A>&& rhs);
};


// FUNCTIONS
// ---------


template <typename Iter1, typename Iter2>
Iter1 string_find(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept
{
    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length >= sublen; --length, ++first) {
        if (std::equal(substr_first, substr_last, first)) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 string_find_of(Iter1 first, size_t length, Iter2 substr, size_t sublen) noexcept
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length, ++first) {
        if (std::any_of(substr_first, substr_last, [first](Char c) {
            return *first == c;
        })) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 string_find_not_of(Iter1 first, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length, ++first) {
        if (std::none_of(substr_first, substr_last, [first](Char c) {
            return *first == c;
        })) {
            return first;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 string_rfind(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    if (sublen == 0) {
        return last;
    }

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    last -= sublen - 1;
    for (; length >= sublen; --length) {
        if (std::equal(substr_first, substr_last, --last)) {
            return last;
        }
    }

    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 string_rfind_of(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length) {
        --last;
        if (std::any_of(substr_first, substr_last, [last](Char c) {
            return *last == c;
        })) {
            return last;
        }
    }
    return nullptr;
}


template <typename Iter1, typename Iter2>
Iter1 string_rfind_not_of(Iter1 last, size_t length, Iter2 substr, size_t sublen)
{
    typedef typename std::iterator_traits<Iter1>::value_type Char;

    auto substr_first = substr;
    auto substr_last = substr_first + sublen;
    for (; length; --length) {
        --last;
        if (std::none_of(substr_first, substr_last, [last](Char c) {
            return *last == c;
        })) {
            return last;
        }
    }
    return nullptr;
}


// OBJECTS
// -------

template <typename C, typename T>
const typename basic_string_view<C, T>::size_type basic_string_view<C, T>::npos;

template <typename C, typename T>
void swap(basic_string_view<C, T>& lhs, basic_string_view<C, T>& rhs)
{
    lhs.swap(rhs);
}


template <typename C, typename T>
std::basic_istream<C, T> & operator>>(std::basic_istream<C, T> &stream, basic_string_view<C, T>& str)
{
    return stream.read(const_cast<char*>(str.data_), str.length_);
}


template <typename C, typename T>
std::basic_ostream<C, T> & operator<<(std::basic_ostream<C, T> &stream, basic_string_view<C, T>& str)
{
    return stream.write(str.data(), str.length());
}


template <typename C, typename T>
bool operator==(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && T::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}


template <typename C, typename T, typename A>
bool operator==(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T, typename A>
bool operator==(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator==(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T>
bool operator==(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator!=(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{

    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
bool operator!=(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T>
bool operator!=(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T>
bool operator!=(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T>
bool operator<(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename C, typename T, typename A>
bool operator<(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) < rhs;
}


template <typename C, typename T, typename A>
bool operator<(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return lhs < basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator<(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return basic_string_view<C, T>(lhs) < rhs;
}


template <typename C, typename T>
bool operator<(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return lhs < basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator<=(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename C, typename T, typename A>
bool operator<=(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) <= rhs;
}


template <typename C, typename T, typename A>
bool operator<=(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return lhs <= basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator<=(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return basic_string_view<C, T>(lhs) <= rhs;
}


template <typename C, typename T>
bool operator<=(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return lhs <= basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator>(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return rhs < lhs;
}


template <typename C, typename T, typename A>
bool operator>(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) > rhs;
}


template <typename C, typename T, typename A>
bool operator>(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return lhs > basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator>(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return basic_string_view<C, T>(lhs) > rhs;
}


template <typename C, typename T>
bool operator>(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return lhs > basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator>=(const basic_string_view<C, T>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename C, typename T, typename A>
bool operator>=(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) >= rhs;
}


template <typename C, typename T, typename A>
bool operator>=(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs) noexcept
{
    return lhs >= basic_string_view<C, T>(rhs);
}


template <typename C, typename T>
bool operator>=(const C* lhs, const basic_string_view<C, T>& rhs)
{
    return basic_string_view<C, T>(lhs) >= rhs;
}


template <typename C, typename T>
bool operator>=(const basic_string_view<C, T>& lhs, const C* rhs)
{
    return lhs >= basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
std::basic_string<C, T, A> operator+(const std::basic_string<C, T, A>& lhs, const basic_string_view<C, T>& rhs)
{
    std::basic_string<C, T, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T, typename A>
std::basic_string<C, T, A> operator+(std::basic_string<C, T, A>&& lhs, const basic_string_view<C, T>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T, typename A>
std::basic_string<C, T, A> operator+(const basic_string_view<C, T>& lhs, const std::basic_string<C, T, A>& rhs)
{
    std::basic_string<C, T, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T, typename A>
std::basic_string<C, T, A> operator+(const basic_string_view<C, T>& lhs, std::basic_string<C, T, A>&& rhs)
{
    return std::move(rhs.insert(0, lhs.data(), lhs.size()));
}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const self_t& str):
    data_(str.data()),
    length_(str.length())
{}


template <typename C, typename T>
basic_string_view<C, T>&  basic_string_view<C, T>::operator=(const self_t& str)
{
    data_ = str.data_;
    length_ = str.length_;
    return *this;
}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(self_t&& str)
{
    swap(str);
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator=(self_t&& str) -> self_t&
{
    swap(str);
    return *this;
}


template <typename C, typename T>
template <typename A>
basic_string_view<C, T>::basic_string_view(const string<C, T, A>& str):
    data_(str.data()),
    length_(str.length())
{}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const self_t& str, size_type pos, size_type len)
{
    size_type size_ = str.size();
    if (pos > size_) {
        throw std::out_of_range("basic_string_view::basic_string_view().");
    }
    data_ = str.data() + pos;
    length_ = std::min(len, size_ - pos);
}


template <typename C, typename T>
template <typename A>
basic_string_view<C, T>::basic_string_view(const string<C, T, A>& str, size_type pos, size_type len)
{
    operator=(self_t(str), pos, len);
}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const C* str)
{
    data_ = str;
    length_ = str ? traits_type::length(str) : 0;
}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const C* str, size_t n)
{
    data_ = str;
    length_ = n;
}


template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const C* begin, const C* end):
    data_(begin),
    length_(end - begin)
{}


template <typename C, typename T>
basic_string_view<C, T>&  basic_string_view<C, T>::operator=(const C* str)
{
    assign(str);
    return *this;
}


template <typename C, typename T>
auto basic_string_view<C, T>::begin() const -> const_iterator
{
    return data_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::end() const -> const_iterator
{
    return data_ + length_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename C, typename T>
auto basic_string_view<C, T>::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename C, typename T>
auto basic_string_view<C, T>::cbegin() const -> const_iterator
{
    return begin();
}


template <typename C, typename T>
auto basic_string_view<C, T>::cend() const -> const_iterator
{
    return end();
}


template <typename C, typename T>
auto basic_string_view<C, T>::crbegin() const -> const_reverse_iterator
{
    return rbegin();
}


template <typename C, typename T>
auto basic_string_view<C, T>::crend() const -> const_reverse_iterator
{
    return rend();
}


template <typename C, typename T>
auto basic_string_view<C, T>::size() const -> size_type
{
    return length_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::length() const -> size_type
{
    return length_;
}


template <typename C, typename T>
bool basic_string_view<C, T>::empty() const noexcept
{
    return length_ == 0;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator[](size_type pos) const -> const_reference
{
    assert(pos <= size() && "string index out of bounds");
    return *(data_ + pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::at(size_type pos) const -> const_reference
{
    return operator[](pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::front() const -> const_reference
{
    assert(!empty() && "string::front(): string is empty");
    return *data_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::back() const -> const_reference
{
    assert(!empty() && "string::back(): string is empty");
    return *(data_ + length_ - 1);
}


template <typename C, typename T>
basic_string_view<C, T>&  basic_string_view<C, T>::assign(const self_t& str)
{
    data_ = str.data();
    length_ = str.length();
    return *this;
}


template <typename C, typename T>
basic_string_view<C, T>&  basic_string_view<C, T>::assign(const stl_type& str)
{
    return assign(basic_string_view<C, T>(str));
}


template <typename C, typename T>
auto basic_string_view<C, T>::assign(const self_t& str, size_type subpos, size_type sublen) -> self_t&
{
    size_type size = str.size();
    if (subpos > size) {
        throw std::out_of_range("basic_string_view::assign().");
    }
    return assign(str.data() + subpos, std::min(sublen, size - subpos));
}


template <typename C, typename T>
auto basic_string_view<C, T>::assign(const stl_type& str, size_type subpos, size_type sublen) -> self_t&
{
    return assign(basic_string_view<C, T>(str), subpos, sublen);
}


template <typename C, typename T>
auto basic_string_view<C, T>::assign(const_pointer s) -> self_t&
{
    data_ = s;
    length_ = s ? traits_type::length(s) : 0;
    return *this;
}


template <typename C, typename T>
auto basic_string_view<C, T>::assign(const_pointer s, size_type n) -> self_t&
{
    data_ = s;
    length_ = n;
    return *this;
}


template <typename C, typename T>
void basic_string_view<C, T>::swap(basic_string_view<C, T>& other)
{
    std::swap(data_, other.data_);
    std::swap(length_, other.length_);
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator++() -> self_t&
{
    if (length_) {
        ++data_;
        --length_;
    }
    if (empty()) {
        data_ = nullptr;
    }

    return *this;
}


template <typename C, typename T>
basic_string_view<C, T> basic_string_view<C, T>::operator++(int)
{
    self_t copy(*this);
    operator++();

    return copy;
}


template <typename C, typename T>
basic_string_view<C, T>&  basic_string_view<C, T>::operator--()
{
    if (!empty()) {
        --data_;
        ++length_;
    }
    return *this;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator--(int) -> self_t
{
    self_t copy(*this);
    operator--();

    return copy;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator+=(size_type shift) -> self_t&
{
    size_type offset = std::min<size_type>(shift, length_);
    data_ += offset;
    length_ -= offset;

    return *this;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator+(size_type shift) -> self_t
{
    self_t copy(*this);
    copy += shift;
    return copy;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator-=(size_type shift) -> self_t&
{
    if (!empty()) {
        data_ -= shift;
        length_ += shift;
    }
    return *this;
}


template <typename C, typename T>
auto basic_string_view<C, T>::operator-(size_type shift) -> self_t
{
    self_t copy(*this);
    copy -= shift;
    return copy;
}


template <typename C, typename T>
bool basic_string_view<C, T>::is_null_terminated() const noexcept
{
    return data_ == nullptr || *end() == value_type();
}


template <typename C, typename T>
auto basic_string_view<C, T>::c_str() const noexcept -> const_pointer
{
    return data_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::data() const noexcept -> const_pointer
{
    return data_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_find(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const stl_type& str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    const size_t length = str.size();
    auto found = string_find(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_find(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_find(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_find(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_find_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const stl_type &str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    size_type length = str.size();
    auto found = string_find_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_find_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_find_of(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_find_of(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_find_not_of(data()+pos, size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const stl_type &str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    size_type length = str.size();
    auto found = string_find_not_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_find_not_of(data()+pos, size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_find_not_of(data()+pos, size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_find_not_of(data()+pos, size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const stl_type &str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    size_type length = str.size();
    auto found = string_rfind(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_rfind(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_rfind(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind_of(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const stl_type &str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    size_type length = str.size();
    auto found = string_rfind_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_rfind_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_rfind_of(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind_of(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const self_t& str, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind_not_of(end(), size()-pos, str.data(), str.size());
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const stl_type &str, size_type pos) const -> size_type
{
    const_pointer first = str.data();
    size_type length = str.size();
    auto found = string_rfind_not_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const_pointer array, size_type pos) const -> size_type
{
    const_pointer first = array;
    size_type length = traits_type::length(array);
    auto found = string_rfind_not_of(end(), size()-pos, first, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const_pointer array, size_type pos, size_type length) const -> size_type
{
    auto found = string_rfind_not_of(end(), size()-pos, array, length);
    return found ? found - data() : npos;
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    auto found = string_rfind_not_of(end(), size()-pos, &c, 1);
    return found ? found - data() : npos;
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(const self_t& str) const noexcept
{
    size_type lhs_size = size();
    size_type rhs_size = str.size();
    int result = traits_type::compare(data(), str.data(), std::min(lhs_size, rhs_size));
    if (result != 0) {
        return result;
    } else if (lhs_size < rhs_size) {
        return -1;
    } else if (lhs_size > rhs_size) {
        return 1;
    }
    return 0;
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(const stl_type& str) const noexcept
{
    return compare(self_t(str));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len, const self_t& str) const
{
    return self_t(*this, pos, len).compare(str);
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len, const stl_type& str) const
{
    return self_t(*this, pos, len).compare(str);
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len,
    const self_t& str,
    size_type subpos,
    size_type sublen) const
{
    return self_t(*this, pos, len).compare(self_t(str, subpos, sublen));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len, const stl_type& str, size_type subpos, size_type sublen) const
{
    return self_t(*this, pos, len).compare(self_t(str, subpos, sublen));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(const_pointer s) const
{
    return compare(self_t(s));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len, const_pointer s) const
{
    return self_t(*this, pos, len).compare(s);
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos, size_type len, const_pointer s, size_type n) const
{
    return self_t(*this, pos, len).compare(s, n);
}


template <typename C, typename T>
auto basic_string_view<C, T>::substr(size_type pos, size_type len) const -> self_t
{
    return self_t(*this, pos, len);
}


template <typename C, typename T>
basic_string_view<C, T>::operator bool() const
{
    return !empty();
}


template <typename C, typename T>
basic_string_view<C, T>::operator stl_type() const
{
    return stl_type(data_, length_);
}

// TYPES
// -----

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;
typedef basic_string_view<char16_t> u16string_view;
typedef basic_string_view<char32_t> u32string_view;

PYCPP_END_NAMESPACE

// TODO: need to specialize with my new hash, not std::hash...
namespace std
{
// SPECIALIZATION
// --------------

template <>
struct hash<PYCPP_NAMESPACE::string_view>;

template <>
struct hash<PYCPP_NAMESPACE::wstring_view>;

template <>
struct hash<PYCPP_NAMESPACE::u16string_view>;

template <>
struct hash<PYCPP_NAMESPACE::u32string_view>;

}   /* std */
