//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Secure char_traits implementation.
 *
 *  Provides wrappers assign(), copy(), and move() to call
 *  memory-safe routines.
 */

#pragma once

#include <string.h>
#include <string>

// DECLARATION
// -----------

/**
 *  \brief Secure character traits implementation.
 */
template <typename Char>
struct secure_char_traits
{
    // MEMBER TYPES
    // ------------
    typedef Char char_type;
    typedef typename std::char_traits<Char>::int_type int_type;
    typedef typename std::char_traits<Char>::off_type off_type;
    typedef typename std::char_traits<Char>::pos_type pos_type;
    typedef typename std::char_traits<Char>::state_type state_type;

    // MEMBER FUNCTIONS
    // ----------------
    static void assign(char_type&, const char_type&) noexcept;
    static char_type* assign(char_type*, size_t, char_type);
    static constexpr bool eq(char_type, char_type);
    static constexpr bool lt(char_type, char_type);
    static char_type* move(char_type*, const char_type*, size_t);
    static char_type* copy(char_type*, const char_type*, size_t);
    static int compare(const char_type*, const char_type*, size_t);
    static size_t length(const char_type*);
    static const char_type* find(const char_type*, size_t, const char_type&);
    static constexpr int_type not_eof(int_type) noexcept;
    static constexpr char_type to_char_type(int_type) noexcept;
    static constexpr int_type to_int_type(char_type) noexcept;
    static constexpr bool eq_int_type(int_type, int_type) noexcept;
    static constexpr int_type eof() noexcept;
};


// IMPLEMENTATION
// --------------


template <typename C>
void secure_char_traits<C>::assign(char_type& r, const char_type& a) noexcept
{
    r = a;
}


template <typename C>
auto secure_char_traits<C>::assign(char_type* p, size_t n, char_type a) -> char_type*
{
    char_type* r = p;
    for (; n; --n, ++p) {
        assign(*p, a);
    }
    return p;
}


template <typename C>
constexpr bool secure_char_traits<C>::eq(char_type a, char_type b)
{
    return a == b;
}


template <typename C>
constexpr bool secure_char_traits<C>::lt(char_type a, char_type b)
{
    return a < b;
}


template <typename C>
auto secure_char_traits<C>::move(char_type* dst, const char_type* src, size_t n) -> char_type*
{
    if (n != 0) {
        memmove(dst, src, n * sizeof(char_type));
    }
    return dst;
}


template <typename C>
auto secure_char_traits<C>::copy(char_type* dst, const char_type* src, size_t n) -> char_type*
{
    if (n != 0) {
        memcpy(dst, src, n * sizeof(char_type));
    }
    return dst;
}


template <typename C>
int secure_char_traits<C>::compare(const char_type* s1, const char_type* s2, size_t n)
{
    for (; n; --n, ++s1, ++s2)
    {
        if (lt(*s1, *s2)) {
            return -1;
        } else if (lt(*s2, *s1)) {
            return 1;
        }
    }
    return 0;
}


template <typename C>
auto secure_char_traits<C>::find(const char_type* s, size_t n, const char_type& a) -> const char_type*
{
    for (; n; --n) {
        if (eq(*s, a)) {
            return s;
        }
        ++s;
    }
    return 0;
}


template <typename C>
size_t secure_char_traits<C>::length(const char_type* str)
{
    size_t len = 0;
    for (; !eq(*str, char_type(0)); ++str) {
        ++len;
    }
    return len;
}


template <typename C>
constexpr auto secure_char_traits<C>::not_eof(int_type c) noexcept -> int_type
{
    return eq_int_type(c, eof()) ? ~eof() : c;
}


template <typename C>
constexpr auto secure_char_traits<C>::to_char_type(int_type c) noexcept -> char_type
{
    return char_type(c);
}


template <typename C>
constexpr auto secure_char_traits<C>::to_int_type(char_type c) noexcept -> int_type
{
    return int_type(c);
}


template <typename C>
constexpr bool secure_char_traits<C>::eq_int_type(int_type a, int_type b) noexcept
{
    return a == b;
}


template <typename C>
constexpr auto secure_char_traits<C>::eof() noexcept -> int_type
{
    return int_type(EOF);
}
