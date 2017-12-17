//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Zero-copy string view container.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/string.h>

#if defined(HAVE_CPP17)             // HAVE_CPP17
#   include <string_view>
#else                               // !HAVE_CPP17
#   include <pycpp/stl/algorithm.h>
#   include <pycpp/stl/iostream.h>
#   include <pycpp/stl/iterator.h>
#   include <pycpp/stl/limits.h>
#   include <pycpp/stl/stdexcept.h>
#   include <assert.h>
#endif                              // HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_CPP17)             // HAVE_CPP17

// ALIAS
// -----

using std::basic_string_view;
using std::string_view;
using std::wstring_view;
using std::u16string_view;
using std::u32string_view;

#else                               // !HAVE_CPP17

// FORWARD
// -------

template <
    typename Char,
    typename Traits = char_traits<Char>
>
class basic_string_view;

// ALIAS
// -----

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

// DECLARATIIONS
// -------------

// OBJECTS

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
template <typename Char, typename Traits>
class basic_string_view
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = Char;
    using traits_type = Traits;
    using reference = Char&;
    using const_reference = const Char&;
    using pointer = Char*;
    using const_pointer = const Char*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<pointer>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_pointer>;

    // MEMBER VARIABLES
    // ----------------
    static const size_type npos = SIZE_MAX;

    // MEMBER FUNCTIONS
    // ----------------
    basic_string_view() noexcept = default;
    basic_string_view(const basic_string_view<Char, Traits>& str) noexcept = default;
    basic_string_view(const_pointer str, size_type count);
    basic_string_view(const_pointer str);
    basic_string_view<Char, Traits>& operator=(const basic_string_view<Char, Traits>& str) = default;

    // We cannot extend std::basic_string, so provide overloads for
    // `basic_string_view(const basic_string&)`.
    template <typename A> basic_string_view(const basic_string<Char, Traits, A>& str);

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
    size_type size() const noexcept;
    size_type length() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;

    // ELEMENT ACCESS
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const;
    const_reference front() const;
    const_reference back() const;
    const_pointer data() const noexcept;

    // MODIFIERS
    void remove_prefix(size_type n);
    void remove_suffix(size_type n);
    void swap(basic_string_view<Char, Traits>& rhs) noexcept;

    // OPERATIONS
    size_type copy(value_type* dest, size_type count, size_type pos = 0) const;
    basic_string_view<Char, Traits> substr(size_type pos = 0, size_type n = npos) const;

    // COMPARE
    int compare(basic_string_view<Char, Traits> s) const noexcept;
    int compare(size_type pos1, size_type n1, basic_string_view<Char, Traits> s) const;
    int compare(size_type pos1, size_type n1, basic_string_view<Char, Traits> s, size_type pos2, size_type n2) const;
    int compare(const_pointer s) const;
    int compare(size_type pos1, size_type n1, const_pointer s) const;
    int compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const;

    // STARTS WITH
    bool starts_with(basic_string_view<Char, Traits> x) const noexcept;
    bool starts_with(value_type x) const noexcept;
    bool starts_with(const_pointer x) const;

    // ENDS WITH
    bool ends_width(basic_string_view<Char, Traits> x) const noexcept;
    bool ends_width(value_type x) const noexcept;
    bool ends_width(const_pointer x) const;

    // FIND
    size_type find(basic_string_view<Char, Traits> s, size_type pos = 0) const noexcept;
    size_type find(value_type c, size_type pos = 0) const noexcept;
    size_type find(const_pointer s, size_type pos, size_type n) const;
    size_type find(const_pointer s, size_type pos = 0) const;

    // RFIND
    size_type rfind(basic_string_view<Char, Traits> s, size_type pos = npos) const noexcept;
    size_type rfind(value_type c, size_type pos = npos) const noexcept;
    size_type rfind(const_pointer s, size_type pos, size_type n) const;
    size_type rfind(const_pointer s, size_type pos = npos) const;

    // FIND FIRST OF
    size_type find_first_of(basic_string_view<Char, Traits> s, size_type pos = 0) const noexcept;
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
    size_type find_first_of(const_pointer s, size_type pos, size_type n) const;
    size_type find_first_of(const_pointer s, size_type pos = 0) const;

    // FIND LAST OF
    size_type find_last_of(basic_string_view<Char, Traits> s, size_type pos = npos) const noexcept;
    size_type find_last_of(value_type c, size_type pos = npos) const noexcept;
    size_type find_last_of(const_pointer s, size_type pos, size_type n) const;
    size_type find_last_of(const_pointer s, size_type pos = npos) const;

    // FIND FIRST NOT OF
    size_type find_first_not_of(basic_string_view<Char, Traits> s, size_type pos = 0) const noexcept;
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const;
    size_type find_first_not_of(const_pointer s, size_type pos = 0) const;

    // FIND LAST NOT OF
    size_type find_last_not_of(basic_string_view<Char, Traits> s, size_type pos = npos) const noexcept;
    size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;
    size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const;
    size_type find_last_not_of(const_pointer s, size_type pos = npos) const;

    // CONVERSION
    // In C++17, `basic_string` has a constructor from
    // `basic_string_view` we need to emulate.
    template <typename A> explicit operator basic_string<Char, Traits, A>() const;

private:
    const_pointer data_ = nullptr;
    size_t length_ = 0;
};

// IMPLEMENTATION
// --------------

// FUNCTIONS

template <typename Iter, typename C, typename T>
Iter find_not_of(Iter first, Iter last, basic_string_view<C, T> s) noexcept
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    using traits_type = T;

    for (; first != last ; ++first) {
        if (traits_type::find(s.data(), s.size(), *first) == 0) {
            return first;
        }
    }
    return last;
}

// OBJECTS

template <typename C, typename T>
const typename basic_string_view<C, T>::size_type basic_string_view<C, T>::npos;

template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const_pointer str, size_type count):
    data_(str),
    length_(count)
{}

template <typename C, typename T>
basic_string_view<C, T>::basic_string_view(const_pointer str):
    data_(str),
    length_(traits_type::length(str))
{}


template <typename C, typename T>
template <typename A>
basic_string_view<C, T>::basic_string_view(const basic_string<C, T, A>& str):
    data_(str.data()),
    length_(str.length())
{}


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
auto basic_string_view<C, T>::size() const noexcept -> size_type
{
    return length_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::length() const noexcept -> size_type
{
    return length_;
}


template <typename C, typename T>
auto basic_string_view<C, T>::max_size() const noexcept -> size_type
{
    return numeric_limits<size_type>::max();
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
    if (pos > size()) {
        throw out_of_range("string_view::at");
    }
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
auto basic_string_view<C, T>::data() const noexcept -> const_pointer
{
    return data_;
}



template <typename C, typename T>
void basic_string_view<C, T>::remove_prefix(size_type n)
{
    assert(n <= size() && "vector_view::remove_prefix greater than size.");
    data_ += n;
    length_ -= n;
}


template <typename C, typename T>
void basic_string_view<C, T>::remove_suffix(size_type n)
{
    assert(n <= size() && "vector_view::remove_suffix greater than size.");
    length_ -= n;
}


template <typename C, typename T>
void basic_string_view<C, T>::swap(basic_string_view<C, T>& rhs) noexcept
{
    PYCPP_NAMESPACE::swap(data_, rhs.data_);
    PYCPP_NAMESPACE::swap(length_, rhs.length_);
}


template <typename C, typename T>
auto basic_string_view<C, T>::copy(value_type* dst, size_type count, size_type pos) const -> size_type
{
    if (pos > size()) {
        throw out_of_range("basic_string_view::copy");
    }

    size_type length = min(count, size() - pos);
    traits_type::copy(dst, data() + pos, length * sizeof(value_type));

    return length;
}


template <typename C, typename T>
basic_string_view<C, T> basic_string_view<C, T>::substr(size_type pos, size_type n) const
{
    if (pos > size()) {
        throw out_of_range("basic_string_view::substr");
    }

    return basic_string_view(data() + pos, min(n, size() - pos));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(basic_string_view<C, T> s) const noexcept
{
    size_type lhs_size = size();
    size_type rhs_size = s.size();
    int result = traits_type::compare(data(), s.data(), min(lhs_size, rhs_size));
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
int basic_string_view<C, T>::compare(size_type pos1, size_type n1, basic_string_view<C, T> s) const
{
    return substr(pos1, n1).compare(s);
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos1, size_type n1, basic_string_view<C, T> s, size_type pos2, size_type n2) const
{
    return substr(pos1, n1).compare(s.substr(pos2, n2));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(const_pointer s) const
{
    return compare(basic_string_view<C, T>(s));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos1, size_type n1, const_pointer s) const
{
    return substr(pos1, n1).compare(basic_string_view<C, T>(s));
}


template <typename C, typename T>
int basic_string_view<C, T>::compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const
{
    return substr(pos1, n1).compare(basic_string_view<C, T>(s, n2));
}


template <typename C, typename T>
bool basic_string_view<C, T>::starts_with(basic_string_view<C, T> x) const noexcept
{
    return size() >= x.size() && compare(0, x.size(), x) == 0;
}


template <typename C, typename T>
bool basic_string_view<C, T>::starts_with(value_type x) const noexcept
{
    return starts_with(basic_string_view<C, T>(&x, 1));
}


template <typename C, typename T>
bool basic_string_view<C, T>::starts_with(const_pointer x) const
{
    return starts_with(basic_string_view<C, T>(x));
}


template <typename C, typename T>
bool basic_string_view<C, T>::ends_width(basic_string_view<C, T> x) const noexcept
{
    return size() >= x.size() && compare(size() - x.size(), npos, x) == 0;
}


template <typename C, typename T>
bool basic_string_view<C, T>::ends_width(value_type x) const noexcept
{
    return ends_width(basic_string_view<C, T>(&x, 1));
}


template <typename C, typename T>
bool basic_string_view<C, T>::ends_width(const_pointer x) const
{
    return ends_width(basic_string_view<C, T>(x));
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (pos > size()) {
        return npos;
    }
    if (s.empty()) {
      return pos;
    }
    const_iterator iter = search(cbegin() + pos, cend(), s.cbegin(), s.cend(), traits_type::eq);
    return iter == cend() ? npos : distance(cbegin(), iter);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(value_type c, size_type pos) const noexcept -> size_type
{
    return find(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find(const_pointer s, size_type pos) const -> size_type
{
    return find(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (size() < s.size()) {
        return npos;
    }
    if (pos > size() - s.size()) {
        pos = size() - s.size();
    }
    if (s.size() == 0) {
        return pos;
    }

    for (const_pointer it = data() + pos; ; --it) {
        if (traits_type::compare(it, s.data(), s.size()) == 0) {
            return it - data();
        } else if (it == data()) {
            return npos;
        }
    }
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(value_type c, size_type pos) const noexcept -> size_type
{
    return rfind(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return rfind(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::rfind(const_pointer s, size_type pos) const -> size_type
{
    return rfind(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (pos >= size() || s.size() == 0) {
        return npos;
    }

    const_iterator it = PYCPP_NAMESPACE::find_first_of(cbegin() + pos, cend(), s.cbegin(), s.cend(), traits_type::eq);
    return it == cend() ? npos : distance(cbegin(), it);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_first_of(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_first_of(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_of(const_pointer s, size_type pos) const -> size_type
{
    return find_first_of(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (s.size() == 0) {
        return npos;
    }
    if (pos >= size()) {
        pos = 0;
    } else {
        pos = size() - (pos+1);
    }

    const_reverse_iterator iter = PYCPP_NAMESPACE::find_first_of(crbegin() + pos, crend(), s.cbegin(), s.cend(), traits_type::eq);
    if (iter == crend()) {
        return npos;
    }
    return size() - 1 - distance(crbegin(), iter);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_last_of(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_last_of(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_of(const_pointer s, size_type pos) const -> size_type
{
    return find_last_of(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (pos >= size()) {
        return npos;
    }
    if (s.size() == 0) {
        return pos;
    }
    const_iterator iter = find_not_of(cbegin() + pos, cend(), s);
    return iter == cend() ? npos : distance(cbegin(), iter);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_first_not_of(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_first_not_of(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_first_not_of(const_pointer s, size_type pos) const -> size_type
{
    return find_first_not_of(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(basic_string_view<C, T> s, size_type pos) const noexcept -> size_type
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    if (pos >= size()) {
        pos = size() - 1;
    }
    if (s.size() == 0) {
        return pos;
    }
    pos = size() - (pos+1);
    const_reverse_iterator iter = find_not_of(crbegin() + pos, crend(), s);

    if (iter == crend()) {
        return npos;
    }
    return size() - 1 - distance(crbegin(), iter);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(value_type c, size_type pos) const noexcept -> size_type
{
    return find_last_not_of(basic_string_view<C, T>(&c, 1), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const_pointer s, size_type pos, size_type n) const -> size_type
{
    return find_last_not_of(basic_string_view<C, T>(s, n), pos);
}


template <typename C, typename T>
auto basic_string_view<C, T>::find_last_not_of(const_pointer s, size_type pos) const -> size_type
{
    return find_last_not_of(basic_string_view<C, T>(s), pos);
}


template <typename C, typename T>
template <typename A>
basic_string_view<C, T>::operator basic_string<C, T, A>() const
{
    return basic_string<C, T, A>(data(), size());
}

// OPERATORS

template <typename C, typename T>
bool operator==(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && T::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}

template <typename C, typename T>
bool operator!=(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename C, typename T>
bool operator<(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename C, typename T>
bool operator<=(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename C, typename T>
bool operator>(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    return rhs < lhs;
}


template <typename C, typename T>
bool operator>=(basic_string_view<C, T> lhs, basic_string_view<C, T> rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename C, typename T>
basic_ostream<C, T>& operator<<(basic_ostream<C, T>& os, basic_string_view<C, T> v)
{
    return os.write(v.data(), v.length());
}

#endif                              // HAVE_CPP17

// NON-STANDARD
// ------------
// Non-standard extensions specific for PyCPP.

// Check if a string is null-terminated.
// If the string is **not** null-terminated, it will attempt to
// read beyond the array illegally. This is a logic assertion
// to ensure the `basic_string_view` is instantiated from
// a string-literal, null-terminated C-string, or `std::string`.
// Should not be in release builds.
template <typename C, typename T>
inline bool is_null_terminated(basic_string_view<C, T> s) noexcept
{
    return s.data() == nullptr || *s.end() == C();
}


PYCPP_END_NAMESPACE

namespace std
{
// SPECIALIZATION
// --------------

#if !defined(HAVE_CPP17)

PYCPP_SPECIALIZE_HASH_STRING(hash, PYCPP_NAMESPACE::string_view);
PYCPP_SPECIALIZE_HASH_STRING(hash, PYCPP_NAMESPACE::wstring_view);
PYCPP_SPECIALIZE_HASH_STRING(hash, PYCPP_NAMESPACE::u16string_view);
PYCPP_SPECIALIZE_HASH_STRING(hash, PYCPP_NAMESPACE::u32string_view);

#endif          // HAVE_CPP17

}   /* std */


PYCPP_BEGIN_NAMESPACE

// SPECIALIZATION
// --------------

#if defined(USE_XXHASH)

PYCPP_SPECIALIZE_HASH_REFERENCE(hash, string_view);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, wstring_view);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, u16string_view);
PYCPP_SPECIALIZE_HASH_REFERENCE(hash, u32string_view);

#endif          // USE_XXHASH

PYCPP_END_NAMESPACE
