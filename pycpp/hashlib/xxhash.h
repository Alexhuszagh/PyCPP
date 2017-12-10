//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper of the fast xxHash algorithm as an STL hash function.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/hashlib/xxhash_c.h>
#include <pycpp/preprocessor/architecture.h>
#include <pycpp/preprocessor/compiler.h>
#include <functional>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// These numbers were generated using NumPy with an initial seed
// of 0.
// np.random.seed(0)
// np.random.randint(np.iinfo(np.int32).min, np.iinfo(np.int32).max)
// np.random.seed(0)
// np.random.randint(np.iinfo(np.int64).min, np.iinfo(np.int64).max)
#if SYSTEM_ARCHITECTURE <= 32
#   define PYCPP_USE_HASH32
#   define XXH_NO_LONG_LONG
    using hash_result_t = XXH32_hash_t;
    static constexpr hash_result_t HASH_SEED = 118409032;
#elif SYSTEM_ARCHITECTURE == 64
#   define PYCPP_USE_HASH64
    using hash_result_t = XXH64_hash_t;
    static constexpr hash_result_t HASH_SEED = 900450186894289455;
#else
#   error "Unsupported system architecture."
#endif

/**
 *  Specialize primitive types by value using std::hash by default.
 */
#define PYCPP_SPECIALIZE_PRIMITIVE(type)                                \
    template <>                                                         \
    struct xxhash<type>                                                 \
    {                                                                   \
        using argument_type = type;                                     \
        using result_type = size_t;                                     \
                                                                        \
        inline size_t operator()(type x) const noexcept                 \
        {                                                               \
            return std::hash<type>()(x);                                \
        }                                                               \
    }

/**
 *  Specialize classes by const reference using std::hash by default.
 */
#define PYCPP_SPECIALIZE_CLASS(type)                                    \
    template <>                                                         \
    struct xxhash<type>                                                 \
    {                                                                   \
        using argument_type = type;                                     \
        using result_type = size_t;                                     \
                                                                        \
        inline size_t operator()(const type& x) const                   \
        {                                                               \
            return std::hash<type>()(x);                                \
        }                                                               \
    }

// FUNCTIONS
// ---------

inline hash_result_t xxhash_string(const void* buffer, size_t size)
{
#if defined(PYCPP_USE_HASH32)               // 32-bit
    return XXH32(buffer, size, HASH_SEED);
#elif defined(PYCPP_USE_HASH64)             // 64-bit
    return XXH64(buffer, size, HASH_SEED);
#else                                       // Unsupported
#   error "Unsupported system architecture."
#endif                                      // Hash size
}

// FORWARD
// -------

template <typename T>
struct xxhash;

// OBJECTS
// -------

PYCPP_SPECIALIZE_PRIMITIVE(bool);
PYCPP_SPECIALIZE_PRIMITIVE(char);
PYCPP_SPECIALIZE_PRIMITIVE(signed char);
PYCPP_SPECIALIZE_PRIMITIVE(unsigned char);
PYCPP_SPECIALIZE_PRIMITIVE(char16_t);
PYCPP_SPECIALIZE_PRIMITIVE(char32_t);
PYCPP_SPECIALIZE_PRIMITIVE(wchar_t);
PYCPP_SPECIALIZE_PRIMITIVE(short);
PYCPP_SPECIALIZE_PRIMITIVE(unsigned short);
PYCPP_SPECIALIZE_PRIMITIVE(int);
PYCPP_SPECIALIZE_PRIMITIVE(unsigned int);
PYCPP_SPECIALIZE_PRIMITIVE(long);
PYCPP_SPECIALIZE_PRIMITIVE(long long);
PYCPP_SPECIALIZE_PRIMITIVE(unsigned long);
PYCPP_SPECIALIZE_PRIMITIVE(unsigned long long);
PYCPP_SPECIALIZE_PRIMITIVE(float);
PYCPP_SPECIALIZE_PRIMITIVE(double);
PYCPP_SPECIALIZE_PRIMITIVE(long double);

#if defined(HAVE_CPP17)
PYCPP_SPECIALIZE_PRIMITIVE(std::nullptr_t);
#endif          // HAVE_CPP17

// Pointer
template <typename T>
struct xxhash<T*>
{
    using argument_type = T*;
    using result_type = size_t;

    inline size_t operator()(T* x) const noexcept
    {
        return std::hash<T*>()(x);
    }
};

// Enum
template <typename T, bool = std::is_enum<T>::value>
struct enum_xxhash
{
    using argument_type = T;
    using result_type = size_t;

    inline size_t operator()(T x) const noexcept
    {
        using type = typename std::underlying_type<T>::type;
        return std::hash<type>()(static_cast<type>(x));
    }
};

template <typename T>
struct enum_xxhash<T, false>
{
    enum_xxhash() = delete;
    enum_xxhash(const enum_xxhash&) = delete;
    enum_xxhash& operator=(const enum_xxhash&) = delete;
};

template <typename T>
struct xxhash: public enum_xxhash<T>
{};

// CLEANUP
// -------

#undef PYCPP_SPECIALIZE_PRIMITIVE
#undef PYCPP_SPECIALIZE_CLASS
#undef PYCPP_USE_HASH32
#undef PYCPP_USE_HASH64

PYCPP_END_NAMESPACE
