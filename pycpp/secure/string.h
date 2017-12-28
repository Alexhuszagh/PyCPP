//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure string implementation.
 *
 *  A secure string that wipes the buffer upon reallocation or deallocation
 *  to avoid passwords or other sensitive information being retained
 *  in memory. You should use `secure_string` for any sensitive data.
 *
 *  `secure_string` by default uses a secure memory allocator, from
 *  libsodium, which allows locking access, or setting read-only access
 *  to the buffer. Using a non-secure allocator will create a compile-time
 *  error.
 *
 *  `secure_string` does not aim for optimization, and therefore it does
 *  not use short-string optimization.
 *
 *  \synopsis
 *      template<
 *          typename Char,
 *          typename Traits = secure_char_traits<Char>,
 *          typename Allocator = secure_allocator<Char>
 *      >
 *      struct secure_basic_string
 *      {
 *      public:
 *          // MEMBER TYPES
 *          // ------------
 *          using view_type = basic_string_view<Char, Traits>;
 *          using traits_type = Traits;
 *          using value_type = typename traits_type::char_type;
 *          using allocator_type = Allocator;
 *          using size_type = typename allocator_traits<allocator_type>::size_type;
 *          using difference_type = typename allocator_traits<allocator_type>::difference_type;
 *          using reference = value_type&;
 *          using const_reference = const value_type&;
 *          using pointer = value_type*;
 *          using const_pointer = const value_type*;
 *          using iterator = pointer;
 *          using const_iterator = const_pointer;
 *          using reverse_iterator = std::reverse_iterator<iterator>;
 *          using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 *
 *          // MEMBER VARIABLES
 *          // ----------------
 *          static const size_type npos = SIZE_MAX;
 *
 *          // MEMBER FUNCTIONS
 *          // ----------------
 *          secure_basic_string();
 *          secure_basic_string(const allocator_type& alloc);
 *          secure_basic_string(const secure_basic_string& str);
 *          secure_basic_string(const secure_basic_string& str, const allocator_type& alloc);
 *          secure_basic_string& operator=(const secure_basic_string& str);
 *          secure_basic_string(secure_basic_string&& str);
 *          secure_basic_string(secure_basic_string&& str, const allocator_type& alloc);
 *          secure_basic_string& operator=(secure_basic_string&& str);
 *          ~secure_basic_string() noexcept;
 *
 *          secure_basic_string(const secure_basic_string& str, size_t pos, size_t len = npos, const allocator_type& alloc = allocator_type());
 *          secure_basic_string(const_pointer s, const allocator_type& alloc = allocator_type());
 *          secure_basic_string(const_pointer s, size_t n, const allocator_type& alloc = allocator_type());
 *          secure_basic_string(size_t n, value_type c, const allocator_type& alloc = allocator_type());
 *          template <typename Iter> secure_basic_string(Iter first, Iter last, const allocator_type& alloc = allocator_type());
 *          secure_basic_string(initializer_list<value_type> list, const allocator_type& alloc = allocator_type());
 *
 *          // MEMORY
 *          void noaccess() noexcept;
 *          void readonly() noexcept;
 *          void readwrite() noexcept;
 *          void mlock() noexcept;
 *          void munlock() noexcept;
 *
 *          // ITERATORS
 *          iterator begin() noexcept;
 *          iterator end() noexcept;
 *          const_iterator begin() const noexcept;
 *          const_iterator end() const noexcept;
 *          const_reverse_iterator rbegin() const noexcept;
 *          const_reverse_iterator rend() const noexcept;
 *          const_iterator cbegin() const noexcept;
 *          const_iterator cend() const noexcept;
 *          const_reverse_iterator crbegin() const noexcept;
 *          const_reverse_iterator crend() const noexcept;
 *
 *          // CAPACITY
 *          size_type size() const noexcept;
 *          size_type length() const noexcept;
 *          size_type capacity() const noexcept;
 *          bool empty() const noexcept;
 *          size_type max_size() const noexcept;
 *          void clear();
 *          void reserve(size_t n = 0);
 *          void shrink_to_fit();
 *          void resize(size_type n);
 *          void resize(size_type n, value_type c);
 *
 *          // ELEMENT ACCESS
 *          reference operator[](size_type pos) noexcept;
 *          const_reference operator[](size_type pos) const noexcept;
 *          reference at(size_type pos) noexcept;
 *          const_reference at(size_type pos) const noexcept;
 *          reference front() noexcept;
 *          const_reference front() const noexcept;
 *          reference back() noexcept;
 *          const_reference back() const noexcept;
 *
 *          // MODIFIERS
 *
 *          void push_back(value_type c);
 *          void pop_back();
 *          void swap(secure_basic_string&) noexcept;
 *
 *          // OPERATOR+=
 *          secure_basic_string& operator+=(const secure_basic_string& str);
 *          secure_basic_string& operator+=(const view_type& str);
 *          secure_basic_string& operator+=(const_pointer s);
 *          secure_basic_string& operator+=(value_type c);
 *          secure_basic_string& operator+=(initializer_list<value_type> list);
 *
 *          // APPEND
 *          secure_basic_string& append(const secure_basic_string& str);
 *          secure_basic_string& append(const view_type& str);
 *          secure_basic_string& append(const secure_basic_string& str, size_t subpos, size_t sublen);
 *          secure_basic_string& append(const_pointer s);
 *          secure_basic_string& append(const_pointer s, size_t n);
 *          secure_basic_string& append(size_t n, char c);
 *          template <typename Iter> secure_basic_string& append(Iter first, Iter last);
 *          secure_basic_string& append(initializer_list<value_type> list);
 *
 *          // ASSIGN
 *          secure_basic_string& assign(const secure_basic_string& str);
 *          secure_basic_string& assign(const view_type& str);
 *          secure_basic_string& assign(const secure_basic_string& str, size_t subpos, size_t sublen = npos);
 *          secure_basic_string& assign(const_pointer s);
 *          secure_basic_string& assign(const_pointer s, size_t n);
 *          secure_basic_string& assign(size_t n, value_type c);
 *          template <typename Iter> secure_basic_string& assign(Iter first, Iter last);
 *          secure_basic_string& assign(initializer_list<value_type> list);
 *          secure_basic_string& assign(secure_basic_string&& str) noexcept;
 *
 *          // INSERT
 *          secure_basic_string& insert(size_t pos, const basic_string_view<Char, Traits>& str);
 *          secure_basic_string& insert(size_t pos, const basic_string_view<Char, Traits>& str, size_t subpos, size_t sublen);
 *          secure_basic_string& insert(size_t pos, const char* s);
 *          secure_basic_string& insert(size_t pos, const char* s, size_t n);
 *          secure_basic_string& insert(size_t pos, size_t n, char c);
 *          iterator insert(const_iterator p, size_t n, char c);
 *          iterator insert(const_iterator p, char c);
 *          template <typename Iter> iterator insert(iterator p, Iter first, Iter last);
 *          template <typename Iter> iterator insert(const_iterator p, Iter first, Iter last);
 *          secure_basic_string& insert(const_iterator p, initializer_list<char> list);
 *
 *          // ERASE
 *          secure_basic_string& erase(size_t pos = 0, size_t len = npos);
 *          const_iterator erase(const_iterator p);
 *          const_iterator erase(const_iterator first, const_iterator last);
 *
 *          // REPLACE
 *          secure_basic_string& replace(size_t pos, size_t len, const basic_string_view<Char, Traits>& str);
 *          secure_basic_string& replace(const_iterator p1, const_iterator p2, const basic_string_view<Char, Traits>& str);
 *          secure_basic_string& replace(size_t pos, size_t len, const basic_string_view<Char, Traits>& str, size_t subpos, size_t sublen);
 *          secure_basic_string& replace(size_t pos, size_t len, size_t n, char c);
 *          secure_basic_string& replace(const_iterator p1, const_iterator p2, size_t n, char c);
 *          template <typename Iter> secure_basic_string& replace(const_iterator p1, const_iterator p2, Iter first, Iter last);
 *          secure_basic_string& replace(const_iterator p1, const_iterator p2, initializer_list<char> list);
 *
 *          // STRING OPERATIONS
 *          const_pointer c_str() const noexcept;
 *          const_pointer data() const noexcept;
 *          view_type view() const noexcept;
 *          allocator_type get_allocator() const noexcept;
 *          size_t copy(pointer s, size_t len, size_t pos = 0) const;
 *
 *          // FIND
 *          size_type find(const secure_basic_string& str, size_type pos = 0) const noexcept;
 *          size_type find(const view_type& str, size_type pos = 0) const noexcept;
 *          size_type find(const_pointer array, size_type pos = 0) const;
 *          size_type find(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type find(value_type c, size_type pos = 0) const noexcept;
 *
 *          // FIND FIRST OF
 *          size_type find_first_of(const secure_basic_string& str, size_type pos = 0) const noexcept;
 *          size_type find_first_of(const view_type& str, size_type pos = 0) const noexcept;
 *          size_type find_first_of(const_pointer array, size_type pos = 0) const;
 *          size_type find_first_of(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
 *
 *          // FIND FIRST NOT OF
 *          size_type find_first_not_of(const secure_basic_string& str, size_type pos = 0) const noexcept;
 *          size_type find_first_not_of(const view_type& str, size_type pos = 0) const noexcept;
 *          size_type find_first_not_of(const_pointer array, size_type pos = 0) const;
 *          size_type find_first_not_of(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
 *
 *          // RFIND
 *          size_type rfind(const secure_basic_string& str, size_type pos = npos) const noexcept;
 *          size_type rfind(const view_type& str, size_type pos = npos) const noexcept;
 *          size_type rfind(const_pointer array, size_type pos = npos) const;
 *          size_type rfind(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type rfind(value_type c, size_type pos = npos) const noexcept;
 *
 *          // FIND LAST OF
 *          size_type find_last_of(const secure_basic_string& str, size_type pos = npos) const noexcept;
 *          size_type find_last_of(const view_type& str, size_type pos = npos) const noexcept;
 *          size_type find_last_of(const_pointer array, size_type pos = npos) const;
 *          size_type find_last_of(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type find_last_of(value_type c, size_type pos = npos) const noexcept;
 *
 *          // FIND LAST NOT OF
 *          size_type find_last_not_of(const secure_basic_string& str, size_type pos = npos) const noexcept;
 *          size_type find_last_not_of(const view_type& str, size_type pos = npos) const noexcept;
 *          size_type find_last_not_of(const_pointer array, size_type pos = npos) const;
 *          size_type find_last_not_of(const_pointer cstring, size_type pos, size_type length) const;
 *          size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;
 *
 *          // COMPARE
 *          int compare(const secure_basic_string& str) const noexcept;
 *          int compare(const view_type& str) const noexcept;
 *          int compare(size_type pos, size_type len, const secure_basic_string& str) const;
 *          int compare(size_type pos, size_type len, const view_type& str) const;
 *          int compare(size_type pos, size_type len, const secure_basic_string& str, size_type subpos, size_type sublen) const;
 *          int compare(size_type pos, size_type len, const view_type& str, size_type subpos, size_type sublen) const;
 *          int compare(const_pointer s) const;
 *          int compare(size_type pos, size_type len, const_pointer s) const;
 *          int compare(size_type pos, size_type len, const_pointer s, size_type n) const;
 *
 *          secure_basic_string substr(size_type pos = 0, size_type len = npos) const;
 *
 *          // CONVERSIONS
 *          explicit operator bool() const noexcept;
 *          explicit operator view_type() const noexcept;
 *      };
 *
 *
 *      template <typename C, typename T, typename A>
 *      void swap(secure_basic_string<C, T, A>& lhs,
 *          secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      basic_istream<C, T> & operator>>(basic_istream<C, T> &stream,
 *          secure_basic_string<C, T, A>& str);
 *
 *      template <typename C, typename T, typename A>
 *      basic_ostream<C, T> & operator<<(basic_ostream<C, T> &stream,
 *          secure_basic_string<C, T, A>& str);
 *
 *      // RELATIONAL OPERATORS
 *      template <typename C, typename T, typename A>
 *      bool operator==(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator==(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator==(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator==(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator==(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator!=(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator!=(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator!=(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator!=(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator!=(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<=(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<=(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<=(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<=(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator<=(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>=(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>=(const basic_string_view<C, T>& lhs,
 *          const secure_basic_string<C, T, A>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>=(const secure_basic_string<C, T, A>& lhs,
 *          const basic_string_view<C, T>& rhs) noexcept;
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>=(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      bool operator>=(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      // CONCATENATION OPERATORS
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
 *          secure_basic_string<C, T, A>&& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
 *          secure_basic_string<C, T, A>&& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
 *          const C* rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const C* lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const C* lhs,
 *          secure_basic_string<C, T, A>&& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
 *          C rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
 *          C rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(C lhs,
 *          const secure_basic_string<C, T, A>& rhs);
 *
 *      template <typename C, typename T, typename A>
 *      secure_basic_string<C, T, A> operator+(C lhs,
 *          secure_basic_string<C, T, A>&& rhs);
 *
 *      template <typename C, typename T1, typename A, typename T2>
 *      secure_basic_string<C, T1, A> operator+(const secure_basic_string<C, T1, A>& lhs,
 *          const basic_string_view<C, T2>& rhs);
 *
 *      template <typename C, typename T1, typename A, typename T2>
 *      secure_basic_string<C, T1, A> operator+(secure_basic_string<C, T1, A>&& lhs,
 *          const basic_string_view<C, T2>& rhs);
 *
 *      template <typename C, typename T1, typename A, typename T2>
 *      secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs,
 *          const secure_basic_string<C, T1, A>& rhs);
 *
 *      template <typename C, typename T1, typename A, typename T2>
 *      secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs,
 *          secure_basic_string<C, T1, A>&& rhs);
 *
 *      // ALIASES
 *      using secure_string = secure_basic_string<char>;
 *      using secure_wstring = secure_basic_string<wchar_t>;
 *      using secure_u16string = secure_basic_string<char16_t>;
 *      using secure_u32string = secure_basic_string<char32_t>;
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/misc/compressed_pair.h>
#include <pycpp/secure/allocator.h>
#include <pycpp/secure/char_traits.h>
#include <pycpp/secure/stdlib.h>
#include <pycpp/sfinae/is_specialization.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/string_view.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

namespace string_detail
{
// DETAIL
// ------

// Slow route for input iterators
template <typename String, typename ConstIter, typename Iter>
enable_if_t<is_input_iterator<Iter>::value, String&>
replace(String& str, ConstIter p1, ConstIter p2, Iter first, Iter last)
{
    String tmp(first, last);
    return str.replace(p1, p2, tmp);
}


// Optimization for forward iterable iterables
template <typename String, typename ConstIter, typename Iter>
enable_if_t<is_forward_iterable<Iter>::value, String&>
replace(String& str, ConstIter p1, ConstIter p2, Iter first, Iter last)
{
    ConstIter it = str.erase(p1, p2);
    str.insert(it, first, last);
    return str;
}


// Slow route for input iterators
template <typename String, typename ConstIter, typename Iter>
enable_if_t<is_input_iterator<Iter>::value, typename String::iterator>
insert(String& str, ConstIter p, size_t& size, Iter first, Iter last)
{
    size_t pos = p - str.begin();
    String tmp(first, last);
    str.insert(p, tmp);

    return str.begin() + pos;
}


// Optimization for forward iterable iterables
template <typename String, typename ConstIter, typename Iter>
enable_if_t<is_forward_iterable<Iter>::value, typename String::iterator>
insert(String& str, ConstIter p, size_t& size, Iter first, Iter last)
{
    using traits_type = typename String::traits_type;

    // reallocate if necessary
    size_t n = distance(first, last);
    size_t pos = p - str.begin();
    size_t move = str.size() - pos;
    size_t new_size = str.size() + n;
    if (new_size >= str.capacity()) {
        str.reserve(max(new_size+1, 2 * str.capacity()));
    }

    // move
    char* src = &str[0] + pos;
    char* dst = src + n;
    traits_type::move(dst, src, move);

    // assign
    for (; first != last; ++src, ++first) {
        traits_type::assign(*src, *first);
    }
    // update size
    size = new_size;

    return str.begin() + pos;
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
template<
    typename Char,
    typename Traits = secure_char_traits<Char>,
    typename Allocator = secure_allocator<Char>
>
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
    using size_type = typename allocator_traits<allocator_type>::size_type;
    using difference_type = typename allocator_traits<allocator_type>::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;

    // MEMBER VARIABLES
    // ----------------
    static const size_type npos = SIZE_MAX;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS

    secure_basic_string():
        capacity_(15),
        length_(0)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), 15);
    }

    secure_basic_string(const allocator_type& alloc):
        capacity_(15),
        length_(0),
        data_(alloc)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), 15);
    }

    secure_basic_string(const self_t& str):
        capacity_(str.capacity_),
        length_(str.length_)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), str.capacity_);
        traits_type::copy(get<0>(data_), get<0>(str.data_), capacity_);
    }

    secure_basic_string(const self_t& str,
            const allocator_type& alloc):
        capacity_(str.capacity_),
        length_(str.length_),
        data_(alloc)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), str.capacity_);
        traits_type::copy(get<0>(data_), get<0>(str.data_), capacity_);
    }

    self_t& operator=(const self_t& str)
    {
        if (this != &str) {
            reset();
            capacity_ = str.capacity_;
            length_ = str.length_;
            get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), str.capacity_);
            traits_type::copy(get<0>(data_), get<0>(str.data_), capacity_);
        }

        return *this;
    }

    secure_basic_string(self_t&& str):
        capacity_(15),
        length_(0)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), 15);
        swap(str);
    }

    secure_basic_string(self_t&& str,
            const allocator_type& alloc):
        capacity_(15),
        length_(0),
        data_(alloc)
    {
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), 15);
        swap(str);
    }

    self_t& operator=(self_t&& str)
    {
        swap(str);
        return *this;
    }

    ~secure_basic_string() noexcept
    {
        reset();
    }

    secure_basic_string(const self_t& str,
            size_t pos,
            size_t len = npos,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        size_type n = str.size();
        if (pos > n) {
            throw out_of_range("secure_basic_string::secure_basic_string().");
        }

        length_ = min(len, n - pos);
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);
        traits_type::copy(get<0>(data_), str.data() + pos, length_);
    }

    secure_basic_string(const_pointer s,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        length_ = traits_type::length(s);
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);
        traits_type::copy(get<0>(data_), s, length_);
        get<0>(data_)[length_] = value_type();
    }

    secure_basic_string(const_pointer s,
            size_t n,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        length_ = n;
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);
        traits_type::copy(get<0>(data_), s, n);
        get<0>(data_)[length_] = value_type();
    }

    secure_basic_string(size_t n,
            value_type c,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        length_ = n;
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);
        traits_type::assign(get<0>(data_), n, c);
        get<0>(data_)[n] = value_type();
    }

    template <typename Iter>
    secure_basic_string(Iter first, Iter last,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        length_ = distance(first, last);
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);

        size_t n = 0;
        for (; first != last; ++first) {
            get<0>(data_)[n++] = *first;
        }
        get<0>(data_)[length_] = value_type();
    }

    secure_basic_string(initializer_list<value_type> list,
            const allocator_type& alloc = allocator_type()):
        data_(alloc)
    {
        length_ = list.size();
        capacity_ = length_ + 1;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), capacity_);
        traits_type::copy(get<0>(data_), list.begin(), length_);
        get<0>(data_)[length_] = value_type();
    }

    // MEMORY

    void noaccess() noexcept
    {
        static_assert(is_specialization<allocator_type, secure_allocator>::value, "");
        secure_mprotect_noaccess(get<0>(data_));
    }

    void readonly() noexcept
    {
        static_assert(is_specialization<allocator_type, secure_allocator>::value, "");
        secure_mprotect_readonly(get<0>(data_));
    }

    void readwrite() noexcept
    {
        static_assert(is_specialization<allocator_type, secure_allocator>::value, "");
        secure_mprotect_readwrite(get<0>(data_));
    }

    void mlock() noexcept
    {
        static_assert(is_specialization<allocator_type, secure_allocator>::value, "");
        secure_mlock(get<0>(data_), capacity_ * sizeof(value_type));
    }

    void munlock() noexcept
    {
        static_assert(is_specialization<allocator_type, secure_allocator>::value, "");
        secure_munlock(get<0>(data_), capacity_ * sizeof(value_type));
    }

    // ITERATORS

    iterator begin() noexcept
    {
        return iterator(get<0>(data_));
    }

    iterator end() noexcept
    {
        return begin() + size();
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(get<0>(data_));
    }

    const_iterator end() const noexcept
    {
        return begin() + size();
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    // CAPACITY

    size_type size() const noexcept
    {
        return length_;
    }

    size_type length() const noexcept
    {
        return size();
    }

    size_type capacity() const noexcept
    {
        return capacity_;
    }

    bool empty() const noexcept
    {
        return size() == 0;
    }

    size_type max_size() const noexcept
    {
        return allocator_traits<allocator_type>::max_size(get<1>(data_));
    }

    void clear()
    {
        if (!empty()) {
            reset();
            init();
        }
    }

    void reserve(size_t n = 0)
    {
        if (n > max_size()) {
            throw out_of_range("Cannot allocate above max_size.");
        }

        size_t r = max(n, size());
        if (r != capacity()) {
            reallocate(r);
        }
    }

    void shrink_to_fit()
    {
        reserve();
    }

    void resize(size_type n)
    {
        resize(n, value_type());
    }

    void resize(size_type n, value_type c)
    {
        if (n > length()) {
            append(n - length(), c);
        } else {
            length_ = n;
            get<0>(data_)[n] = value_type();
        }
    }

    // ELEMENT ACCESS
    reference operator[](size_type pos) noexcept
    {
        assert(pos <= size() && "string index out of bounds");
        return *(get<0>(data_) + pos);
    }

    const_reference operator[](size_type pos) const noexcept
    {
        assert(pos <= size() && "string index out of bounds");
        return *(get<0>(data_) + pos);
    }

    reference at(size_type pos) noexcept
    {
        return operator[](pos);
    }

    const_reference at(size_type pos) const noexcept
    {
        return operator[](pos);
    }

    reference front() noexcept
    {
        assert(!empty() && "string::front(): string is empty");
        return *(get<0>(data_));
    }

    const_reference front() const noexcept
    {
        assert(!empty() && "string::front(): string is empty");
        return *get<0>(data_);
    }

    reference back() noexcept
    {
        assert(!empty() && "string::back(): string is empty");
        return *(get<0>(data_) + length_ - 1);
    }

    const_reference back() const noexcept
    {
        assert(!empty() && "string::back(): string is empty");
        return *(get<0>(data_) + length_ - 1);
    }

    // MODIFIERS

    void push_back(value_type c)
    {
        if (capacity() == 0) {
            reallocate(15);
        } else if (size() == capacity()-1) {
            reallocate(2 * capacity());
        }

        // assign
        get<0>(data_)[length_++] = c;
        get<0>(data_)[length_] = value_type();
    }

    void pop_back()
    {
        assert(!empty() && "string::pop_back(): string is empty");
        --length_;
        get<0>(data_)[length_] = value_type();
    }

    void swap(self_t& rhs) noexcept
    {
        using PYCPP_NAMESPACE::swap;
        swap(capacity_, rhs.capacity_);
        swap(length_, rhs.length_);
        swap(data_, rhs.data_);
    }

    // OPERATOR+=

    self_t& operator+=(const self_t& str)
    {
        return append(str);
    }

    self_t& operator+=(const view_type& str)
    {
        return append(str);
    }

    self_t& operator+=(const_pointer s)
    {
        return append(s);
    }

    self_t& operator+=(value_type c)
    {
        push_back(c);
        return *this;
    }

    self_t& operator+=(initializer_list<value_type> list)
    {
        return append(list.begin(), list.end());
    }

    // APPEND

    self_t& append(const self_t& str)
    {
        return append(view_type(str));
    }

    self_t& append(const view_type& str)
    {
        size_t n = str.length();
        size_t r = length() + n;
        if (r >= capacity()) {
            reallocate(max(r+1, 2 * capacity()));
        }

        traits_type::copy(get<0>(data_) + length(), str.data(), n);
        length_ += n;
        return *this;
    }

    self_t& append(const self_t& str,
        size_t subpos,
        size_t sublen)
    {
        return append(view_type(str).substr(subpos, sublen));
    }

    self_t& append(const_pointer s)
    {
        return append(view_type(s));
    }

    self_t& append(const_pointer s, size_t n)
    {
        return append(view_type(s, n));
    }

    self_t& append(size_t n, char c)
    {
        size_t r = length() + n;
        if (r >= capacity()) {
            reallocate(max(r+1, 2 * capacity()));
        }

        while (n--) {
            get<0>(data_)[length_++] = c;
        }
        get<0>(data_)[length_] = value_type();

        return *this;
    }

    template <typename Iter>
    self_t& append(Iter first, Iter last)
    {
        size_t n = distance(first, last);
        size_t r = length() + n;
        if (r >= capacity()) {
            reallocate(max(r+1, 2 * capacity()));
        }

        for (; first != last; ++first) {
            get<0>(data_)[length_++] = *first;
        }
        get<0>(data_)[length_] = value_type();

        return *this;
    }

    self_t& append(initializer_list<value_type> list)
    {
        return append(list.begin(), list.size());
    }

    // ASSIGN

    self_t& assign(const self_t& str)
    {
        return assign(view_type(str));
    }

    self_t& assign(const view_type& str)
    {
        size_t n = str.length();
        size_t r = length() + n;
        if (r >= capacity()) {
            reallocate(max(r+1, 2 * capacity()));
        }

        length_ = n;
        traits_type::copy(get<0>(data_), str.data(), n);
        get<0>(data_)[n] = value_type();

        return *this;
    }

    self_t& assign(const self_t& str,
        size_t subpos,
        size_t sublen = npos)
    {
        return assign(view_type(str).substr(subpos, sublen));
    }

    self_t& assign(const_pointer s)
    {
        assert(s && "secure_basic_string:: assign() nullptr.");
        return assign(view_type(s));
    }

    self_t& assign(const_pointer s, size_t n)
    {
        assert(s && "secure_basic_string:: assign() nullptr.");
        return assign(view_type(s, n));
    }

    self_t& assign(size_t n, value_type c)
    {
        size_t r = length() + n;
        if (r >= capacity()) {
            reallocate(max(r+1, 2 * capacity()));
        }

        length_ = n;
        traits_type::assign(get<0>(data_), n, c);
        get<0>(data_)[n] = value_type();

        return *this;
    }

    template <typename Iter>
    self_t& assign(Iter first, Iter last)
    {
        return assign(self_t(first, last));
    }

    self_t& assign(initializer_list<value_type> list)
    {
        return assign(list.begin(), list.size());
    }

    self_t& assign(self_t&& str) noexcept
    {
        swap(str);
        return *this;
    }

    // INSERT

    self_t& insert(size_t pos,
        const basic_string_view<Char, Traits>& str)
    {
        return insert(pos, str.data(), str.size());
    }

    self_t& insert(size_t pos,
        const basic_string_view<Char, Traits>& str,
        size_t subpos,
        size_t sublen)
    {
        return insert(pos, str.substr(subpos, sublen));
    }

    self_t& insert(size_t pos, const char* s)
    {
        return insert(pos, s, traits_type::length(s));
    }

    self_t& insert(size_t pos, const char* s, size_t n)
    {
        if (pos > size()) {
            throw out_of_range("secure_basic_string::erase().");
        }

        const_iterator p = begin() + pos;
        insert(p, s, s + n);
        return *this;
    }

    self_t& insert(size_t pos, size_t n, char c)
    {
        if (pos > size()) {
            throw out_of_range("secure_basic_string::insert().");
        }

        const_iterator p = begin() + pos;
        insert(p, n, c);
        return *this;
    }

    iterator insert(const_iterator p, size_t n, char c)
    {
        // reallocate if necessary
        size_t pos = p - begin();
        size_t move = size() - pos;
        size_t new_size = size() + n;
        if (new_size > capacity()) {
            reserve(new_size);
        }

        // move
        char* src = get<0>(data_) + pos;
        char* dst = src + n;
        traits_type::move(dst, src, move);

        // assign
        traits_type::assign(src, n, c);
        length_ += n;

        return begin() + pos;
    }

    iterator insert(const_iterator p, char c)
    {
        return insert(p, 1, c);
    }

    template <typename Iter>
    iterator insert(iterator p, Iter first, Iter last)
    {
        return insert(const_iterator(p), first, last);
    }

    template <typename Iter>
    iterator insert(const_iterator p, Iter first, Iter last)
    {
        return string_detail::insert(*this, p, length_, first, last);
    }

    self_t& insert(const_iterator p, initializer_list<char> list)
    {
        insert(p, list.begin(), list.end());
        return *this;
    }

    // ERASE

    self_t& erase(size_t pos = 0, size_t len = npos)
    {
        if (pos > size()) {
            throw out_of_range("secure_basic_string::erase().");
        }

        size_type move = size() - pos - len;
        if (move != 0) {
            traits_type::move(get<0>(data_) + pos, get<0>(data_) + pos + len, move);
        }
        length_ -= len;
        get<0>(data_)[length_] = value_type();

        return *this;
    }

    const_iterator erase(const_iterator p)
    {
        assert(p != end() && "Erase called with a non-dereferencable iterator.");

        size_type pos = static_cast<size_type>(p - begin());
        return erase(pos, 1);
    }

    const_iterator erase(const_iterator first, const_iterator last)
    {
        size_type r = first - begin();
        erase(r, last - first);

        return begin() + r;
    }

    // REPLACE

    self_t& replace(size_t pos,
        size_t len,
        const basic_string_view<Char, Traits>& str)
    {
        if (pos > size()) {
            throw out_of_range("secure_basic_string::replace().");
        }

        const_iterator p1 = begin() + pos;
        const_iterator p2 = (len == npos || pos + len >= size()) ? end() : p1 + len;
        return replace(p1, p2, str);
    }

    self_t& replace(const_iterator p1,
        const_iterator p2,
        const basic_string_view<Char, Traits>& str)
    {
        return replace(p1, p2, str.begin(), str.end());
    }

    self_t& replace(size_t pos,
        size_t len,
        const basic_string_view<Char, Traits>& str,
        size_t subpos,
        size_t sublen)
    {
        return replace(pos, len, str.substr(subpos, sublen));
    }

    self_t& replace(size_t pos, size_t len, size_t n, char c)
    {
        if (pos > size()) {
            throw out_of_range("secure_basic_string::replace().");
        }

        const_iterator p1 = begin() + pos;
        const_iterator p2 = (len == npos || pos + len >= size()) ? end() : p1 + len;
        return replace(p1, p2, n, c);
    }

    self_t& replace(const_iterator p1,
        const_iterator p2,
        size_t n,
        char c)
    {
        auto it = erase(p1, p2);
        insert(it, n, c);
        return *this;
    }

    template <typename Iter>
    self_t& replace(const_iterator p1,
        const_iterator p2,
        Iter first,
        Iter last)
    {
        return string_detail::replace(*this, p1, p2, first, last);
    }

    self_t& replace(const_iterator p1,
        const_iterator p2,
        initializer_list<char> list)
    {
        return replace(p1, p2, list.begin(), list.end());
    }

    // STRING OPERATIONS

    const_pointer c_str() const noexcept
    {
        return get<0>(data_);
    }

    const_pointer data() const noexcept
    {
        return get<0>(data_);
    }

    view_type view() const noexcept
    {
        return view_type(data(), size());
    }

    allocator_type get_allocator() const noexcept
    {
        return get<1>(data_);
    }

    size_t copy(pointer s, size_t len, size_t pos = 0) const
    {
        if (pos > length()) {
            throw out_of_range("secure_basic_string:: copy().");
        }

        size_type rlen = min(len, length() - pos);
        traits_type::copy(s, data() + pos, rlen);
        return rlen;
    }

    // FIND

    size_type find(const self_t& str,
        size_type pos = 0) const noexcept
    {
        return find(view_type(str), pos);
    }

    size_type find(const view_type& str,
        size_type pos = 0) const noexcept
    {
        return view_type(*this).find(str, pos);
    }

    size_type find(const_pointer s,
        size_type pos = 0) const
    {
        return find(view_type(s), pos);
    }

    size_type find(const_pointer s,
        size_type pos,
        size_type n) const
    {
        return find(view_type(s, n), pos);
    }

    size_type find(value_type c,
        size_type pos = 0) const noexcept
    {
        return find(view_type(&c, 1), pos);
    }

    // FIND FIRST OF

    size_type find_first_of(const self_t& str,
        size_type pos = 0) const noexcept
    {
        return find_first_of(view_type(str), pos);
    }

    size_type find_first_of(const view_type& str,
        size_type pos = 0) const noexcept
    {
        return view_type(*this).find_first_of(str, pos);
    }

    size_type find_first_of(const_pointer s,
        size_type pos = 0) const
    {
        return find_first_of(view_type(s), pos);
    }

    size_type find_first_of(const_pointer s,
        size_type pos,
        size_type n) const
    {
        return find_first_of(view_type(s, n), pos);
    }

    size_type find_first_of(value_type c,
        size_type pos = 0) const noexcept
    {
        return find_first_of(view_type(&c, 1), pos);
    }

    // FIND FIRST NOT OF

    size_type find_first_not_of(const self_t& str,
        size_type pos = 0) const noexcept
    {
        return find_first_not_of(view_type(str), pos);
    }

    size_type find_first_not_of(const view_type& str,
        size_type pos = 0) const noexcept
    {
        return view_type(*this).find_first_not_of(str, pos);
    }

    size_type find_first_not_of(const_pointer s,
        size_type pos = 0) const
    {
        return find_first_not_of(view_type(s), pos);
    }

    size_type find_first_not_of(const_pointer s,
        size_type pos, size_type n) const
    {
        return find_first_not_of(view_type(s, n), pos);
    }

    size_type find_first_not_of(value_type c,
        size_type pos = 0) const noexcept
    {
        return find_first_not_of(view_type(&c, 1), pos);
    }

    // RFIND

    size_type rfind(const self_t& str,
        size_type pos = npos) const noexcept
    {
        return rfind(view_type(str), pos);
    }

    size_type rfind(const view_type& str,
        size_type pos = npos) const noexcept
    {
        return view_type(*this).rfind(str, pos);
    }

    size_type rfind(const_pointer s,
        size_type pos = npos) const
    {
        return rfind(view_type(s), pos);
    }

    size_type rfind(const_pointer s,
        size_type pos,
        size_type n) const
    {
        return rfind(view_type(s, n), pos);
    }

    size_type rfind(value_type c,
        size_type pos = npos) const noexcept
    {
        return rfind(view_type(&c, 1), pos);
    }

    // FIND LAST OF

    size_type find_last_of(const self_t& str,
        size_type pos = npos) const noexcept
    {
        return find_last_of(view_type(str), pos);
    }

    size_type find_last_of(const view_type& str,
        size_type pos = npos) const noexcept
    {
        return view_type(*this).find_last_of(str, pos);
    }

    size_type find_last_of(const_pointer s,
        size_type pos = npos) const
    {
        return find_last_of(view_type(s), pos);
    }

    size_type find_last_of(const_pointer s,
        size_type pos,
        size_type n) const
    {
        return find_last_of(view_type(s, n), pos);
    }

    size_type find_last_of(value_type c,
        size_type pos = npos) const noexcept
    {
        return find_last_of(view_type(&c, 1), pos);
    }

    // FIND LAST NOT OF

    size_type find_last_not_of(const self_t& str,
        size_type pos = npos) const noexcept
    {
        return find_last_not_of(view_type(str), pos);
    }

    size_type find_last_not_of(const view_type& str,
        size_type pos = npos) const noexcept
    {
        return view_type(*this).find_last_not_of(str, pos);
    }

    size_type find_last_not_of(const_pointer s,
        size_type pos = npos) const
    {
        return find_last_not_of(view_type(s), pos);
    }

    size_type find_last_not_of(const_pointer s,
        size_type pos,
        size_type n) const
    {
        return find_last_not_of(view_type(s, n), pos);
    }

    size_type find_last_not_of(value_type c,
        size_type pos = npos) const noexcept
    {
        return find_last_not_of(view_type(&c, 1), pos);
    }

    // COMPARE

    int compare(const self_t& str) const noexcept
    {
        return compare(view_type(str));
    }

    int compare(const view_type& str) const noexcept
    {
        return view_type(*this).compare(str);
    }

    int compare(size_type pos,
        size_type len,
        const self_t& str) const
    {
        return view_type(*this, pos, len).compare(str);
    }

    int compare(size_type pos,
        size_type len,
        const view_type& str) const
    {
        return view_type(*this, pos, len).compare(str);
    }

    int compare(size_type pos,
        size_type len,
        const self_t& str,
        size_type subpos,
        size_type sublen) const
    {
        view_type lhs(*this, pos, len);
        view_type rhs(str, subpos, sublen);
        return lhs.compare(rhs);
    }

    int compare(size_type pos,
        size_type len,
        const view_type& str,
        size_type subpos,
        size_type sublen) const
    {
        view_type lhs(*this, pos, len);
        view_type rhs(str, subpos, sublen);
        return lhs.compare(rhs);
    }

    int compare(const_pointer s) const
    {
        return compare(view_type(s));
    }

    int compare(size_type pos,
        size_type len,
        const_pointer s) const
    {
        return view_type(*this, pos, len).compare(s);
    }

    int compare(size_type pos,
        size_type len,
        const_pointer s,
        size_type n) const
    {
        return view_type(*this, pos, len).compare(s, n);
    }

    self_t substr(size_type pos = 0,
        size_type len = npos) const
    {
        return self_t(*this, pos, len);
    }

    // CONVERSIONS

    explicit operator bool() const noexcept
    {
        return !empty();
    }

    explicit operator view_type() const noexcept
    {
        return view_type(get<0>(data_), length_);
    }

private:
    size_type capacity_;
    size_type length_;
    compressed_pair<pointer, allocator_type> data_;

    // Initialize the buffer to a reasonable default size.
    // Use 15, since it's the minimum to avoid short-string
    // optimization.
    void init()
    {
        capacity_ = 15;
        length_ = 0;
        get<0>(data_) = allocator_traits<allocator_type>::allocate(get<1>(data_), 15);
    }

    // Free the underlying buffer and set capacity to 0
    void reset() noexcept
    {
        allocator_traits<allocator_type>::deallocate(get<1>(data_), get<0>(data_), capacity_);
        capacity_ = length_ = 0;
    }

    // Reallocate the underlying buffer to a new size
    void reallocate(size_type n)
    {
        // create new buffer
        pointer buf = allocator_traits<allocator_type>::allocate(get<1>(data_), n+1, get<0>(data_));
        traits_type::copy(buf, get<0>(data_), length_+1);

        // clear existing buffer
        allocator_traits<allocator_type>::deallocate(get<1>(data_), get<0>(data_), capacity_);

        // store data
        capacity_ = n;
        get<0>(data_) = buf;
    }

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename C, typename T, typename A>
    friend basic_istream<C, T>& operator>>(basic_istream<C, T>& stream, secure_basic_string<C, T, A>& str);
};


// SPECIALIZATION
// --------------

template<
    typename Char,
    typename Traits,
    typename Allocator
>
struct is_relocatable<secure_basic_string<Char, Traits, Allocator>>: is_relocatable<Allocator>
{};

// IMPLEMENTATION
// --------------


template <typename C, typename T, typename A>
const typename secure_basic_string<C, T, A>::size_type secure_basic_string<C, T, A>::npos;

template <typename C, typename T, typename A>
inline void swap(secure_basic_string<C, T, A>& lhs,
    secure_basic_string<C, T, A>& rhs)
{
    lhs.swap(rhs);
}


template <typename C, typename T, typename A>
inline basic_istream<C, T> & operator>>(basic_istream<C, T> &stream,
    secure_basic_string<C, T, A>& str)
{
    return stream.read(get<0>(str.data_), str.length_);
}


template <typename C, typename T, typename A>
inline basic_ostream<C, T> & operator<<(basic_ostream<C, T> &stream,
    secure_basic_string<C, T, A>& str)
{
    return stream.write(str.data(), str.length());
}

// RELATIONAL OPERATORS

template <typename C, typename T, typename A>
inline bool operator==(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && T::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}


template <typename C, typename T, typename A>
inline bool operator==(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
inline bool operator==(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T, typename A>
inline bool operator==(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) == rhs;
}


template <typename C, typename T, typename A>
inline bool operator==(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return lhs == basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
inline bool operator!=(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{

    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
inline bool operator!=(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
inline bool operator!=(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
inline bool operator!=(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
inline bool operator!=(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename T, typename A>
inline bool operator<(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename C, typename T, typename A>
inline bool operator<(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs < rhs.view();
}


template <typename C, typename T, typename A>
inline bool operator<(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() < rhs;
}


template <typename C, typename T, typename A>
inline bool operator<(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) < rhs;
}


template <typename C, typename T, typename A>
inline bool operator<(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return lhs < basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
inline bool operator<=(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename C, typename T, typename A>
inline bool operator<=(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs <= rhs.view();
}


template <typename C, typename T, typename A>
inline bool operator<=(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() <= rhs;
}


template <typename C, typename T, typename A>
inline bool operator<=(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) <= rhs;
}


template <typename C, typename T, typename A>
inline bool operator<=(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return lhs <= basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
inline bool operator>(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return rhs < lhs;
}


template <typename C, typename T, typename A>
inline bool operator>(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs > rhs.view();
}


template <typename C, typename T, typename A>
inline bool operator>(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() > rhs;
}


template <typename C, typename T, typename A>
inline bool operator>(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) > rhs;
}


template <typename C, typename T, typename A>
inline bool operator>(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return lhs > basic_string_view<C, T>(rhs);
}


template <typename C, typename T, typename A>
inline bool operator>=(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename C, typename T, typename A>
inline bool operator>=(const basic_string_view<C, T>& lhs,
    const secure_basic_string<C, T, A>& rhs) noexcept
{
    return lhs >= rhs.view();
}


template <typename C, typename T, typename A>
inline bool operator>=(const secure_basic_string<C, T, A>& lhs,
    const basic_string_view<C, T>& rhs) noexcept
{
    return lhs.view() >= rhs;
}


template <typename C, typename T, typename A>
inline bool operator>=(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return basic_string_view<C, T>(lhs) >= rhs;
}


template <typename C, typename T, typename A>
inline bool operator>=(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    return lhs >= basic_string_view<C, T>(rhs);
}

// CONCATENATION OPERATORS
// Do not overload `string`, since it cannot securely
// store data.

template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
    secure_basic_string<C, T, A>&& rhs)
{
    return move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    return move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
    secure_basic_string<C, T, A>&& rhs)
{
    return move(rhs.insert(0, lhs.data(), lhs.size()));
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
    const C* rhs)
{
    secure_basic_string<C, T, A> output;
    size_t rhs_length = T::length(rhs);
    output.reserve(lhs.size() + rhs_length);
    output.append(lhs.data(), lhs.size());
    output.append(rhs);
    return output;
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
    const C* rhs)
{
    return move(lhs.append(rhs));
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const C* lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    size_t lhs_length = T::length(lhs);
    output.reserve(lhs_length + rhs.size());
    output.append(lhs, lhs_length);
    output.append(rhs.data(), rhs.size());
    return  output;
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const C* lhs,
    secure_basic_string<C, T, A>&& rhs)
{
    return move(rhs.insert(0, lhs, T::length(lhs)));
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(const secure_basic_string<C, T, A>& lhs,
    C rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(lhs.size() + 1);
    output.append(lhs.data(), lhs.size());
    output.push_back(rhs);
    return output;
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(secure_basic_string<C, T, A>&& lhs,
    C rhs)
{
    lhs.push_back(rhs);
    return move(lhs);
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(C lhs,
    const secure_basic_string<C, T, A>& rhs)
{
    secure_basic_string<C, T, A> output;
    output.reserve(1 + rhs.size());
    output.push_back(lhs);
    output.append(rhs.data(), rhs.size());
    return  output;
}


template <typename C, typename T, typename A>
inline secure_basic_string<C, T, A> operator+(C lhs, secure_basic_string<C, T, A>&& rhs)
{
    return move(rhs.insert(size_t(0), size_t(1), lhs));
}


template <typename C, typename T1, typename A, typename T2>
inline secure_basic_string<C, T1, A> operator+(const secure_basic_string<C, T1, A>& lhs,
    const basic_string_view<C, T2>& rhs)
{
    secure_basic_string<C, T1, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T1, typename A, typename T2>
inline secure_basic_string<C, T1, A> operator+(secure_basic_string<C, T1, A>&& lhs,
    const basic_string_view<C, T2>& rhs)
{
    return move(lhs.append(rhs.data(), rhs.size()));
}


template <typename C, typename T1, typename A, typename T2>
inline secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs,
    const secure_basic_string<C, T1, A>& rhs)
{
    secure_basic_string<C, T1, A> output;
    output.reserve(lhs.size() + rhs.size());
    output.append(lhs.data(), lhs.size());
    output.append(rhs.data(), rhs.size());
    return output;
}


template <typename C, typename T1, typename A, typename T2>
inline secure_basic_string<C, T1, A> operator+(const basic_string_view<C, T2>& lhs,
    secure_basic_string<C, T1, A>&& rhs)
{
    return move(rhs.insert(0, lhs.data(), lhs.size()));
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
