//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type-safe enum with bitwise operators.
 */

#pragma once

#include <stdint.h>

// MACROS
// ------

/**
 *  \brief Get enum underlying type.
 */
#define int_t(x) static_cast<int64_t>(x)


/**
 *  \brief Macro to define enum operators between enumerations.
 *
 *  Supports `&`, `&=`, `|`, `|=`, `^`, `^=`, `~`, and bool conversion.
 */
#define enum_flag2(lhs_t, ths_t)                                        \
    /*  \brief Bitwise or operator. */                                  \
    inline lhs_t operator|(lhs_t lhs, ths_t rhs)                        \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) | int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise or assignment operator. */                       \
    inline lhs_t & operator|=(lhs_t &lhs, ths_t rhs)                    \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) | int_t(rhs));              \
        return lhs;                                                     \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and operator. */                                 \
    inline lhs_t operator&(lhs_t lhs, ths_t rhs)                        \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) & int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and assignment operator. */                      \
    inline lhs_t & operator&=(lhs_t &lhs, ths_t rhs)                    \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) & int_t(rhs));              \
        return lhs;                                                     \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor operator. */                                 \
    inline lhs_t operator^(lhs_t lhs, ths_t rhs)                        \
    {                                                                   \
        return static_cast<lhs_t>(int_t(lhs) ^ int_t(rhs));             \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor assignment operator. */                      \
    inline lhs_t & operator^=(lhs_t &lhs, ths_t rhs)                    \
    {                                                                   \
        lhs = static_cast<lhs_t>(int_t(lhs) ^ int_t(rhs));              \
        return lhs;                                                     \
    }


/**
 *  \brief Set enumeration flags within the same enum.
 */
#define enum_flag1(enum_t)                                              \
    enum_flag2(enum_t, enum_t)                                          \
                                                                        \
    /*  \brief Bitwise negation operator. */                            \
    inline enum_t operator~(enum_t value)                               \
    {                                                                   \
        return static_cast<enum_t>(~int_t(value));                      \
    }                                                                   \
                                                                        \
    /*  \brief Negation operator. */                                    \
    inline bool operator!(enum_t value)                                 \
    {                                                                   \
        return !bool(int_t(value));                                     \
    }

/**
 *  \brief Macros to grab the proper bit-wise flag setter.
 */
#define enum_id(x) x
#define get_enum_flag(_1,_2,NAME,...) NAME
#define enum_flag(...) enum_id(get_enum_flag(__VA_ARGS__, enum_flag2, enum_flag1)(__VA_ARGS__))
