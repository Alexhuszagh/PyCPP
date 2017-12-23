//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type-safe enum with bitwise operators.
 */

#pragma once

#include <pycpp/stl/type_traits.h>
#include <stdint.h>

// HELPERS
// -------

/**
 *  \brief Get enum underlying type.
 */
template <typename T>
inline PYCPP_NAMESPACE::underlying_type_t<T> int_t(T t)
{
    return static_cast<PYCPP_NAMESPACE::underlying_type_t<T>>(t);
}

// MACROS
// ------

/**
 *  \brief Macro to define enum operators between enumerations.
 *
 *  Supports `&`, `&=`, `|`, `|=`, `^`, `^=`, `~`, and bool conversion.
 */
#define PYCPP_ENUM_FLAG2(lhs_t, ths_t)                                  \
    /*  \brief Bitwise or operator. */                                  \
    inline lhs_t operator|(lhs_t lhs, ths_t rhs) noexcept               \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) | int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise or assignment operator. */                       \
    inline lhs_t & operator|=(lhs_t &lhs, ths_t rhs) noexcept           \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) | int_t(rhs));              \
        return lhs;                                                     \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and operator. */                                 \
    inline lhs_t operator&(lhs_t lhs, ths_t rhs) noexcept               \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) & int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and assignment operator. */                      \
    inline lhs_t & operator&=(lhs_t &lhs, ths_t rhs) noexcept           \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) & int_t(rhs));              \
        return lhs;                                                     \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor operator. */                                 \
    inline lhs_t operator^(lhs_t lhs, ths_t rhs) noexcept               \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) ^ int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor assignment operator. */                      \
    inline lhs_t & operator^=(lhs_t &lhs, ths_t rhs) noexcept           \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) ^ int_t(rhs));              \
        return lhs;                                                     \
    }


/**
 *  \brief Set enumeration flags within the same enum.
 */
#define PYCPP_ENUM_FLAG1(enum_t)                                        \
    PYCPP_ENUM_FLAG2(enum_t, enum_t)                                    \
                                                                        \
    /*  \brief Bitwise negation operator. */                            \
    inline enum_t operator~(enum_t value) noexcept                      \
    {                                                                   \
        return static_cast<enum_t>(~int_t(value));                      \
    }                                                                   \
                                                                        \
    /*  \brief Negation operator. */                                    \
    inline bool operator!(enum_t value) noexcept                        \
    {                                                                   \
        return !bool(int_t(value));                                     \
    }

/**
 *  \brief Macros to grab the proper bit-wise flag setter.
 */
#define PYCPP_ENUM_ID(x) x
#define PYCPP_GET_ENUM_FLAG(_1,_2,NAME,...) NAME
#define PYCPP_ENUM_FLAG(...) PYCPP_ENUM_ID(PYCPP_GET_ENUM_FLAG(__VA_ARGS__, PYCPP_ENUM_FLAG2, PYCPP_ENUM_FLAG1)(__VA_ARGS__))
