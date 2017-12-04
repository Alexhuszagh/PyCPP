//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Check a type contains a specific member function.
 */

#pragma once

#include <pycpp/config.h>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

/**
 *  \brief Check if class has named method.
 *  \warning Does not handle overloaded functions.
 *
 *  \param func             The name of the function to detect.
 *  \param name             The name of the resulting SFINAE detection class.
 *
 *  \code
 *      PYCPP_HAS_MEMBER_FUNCTION(reserve, has_reserve);
 */
#define PYCPP_HAS_MEMBER_FUNCTION2(func, name)                               \
    template <typename T>                                                    \
    class name                                                               \
    {                                                                        \
    protected:                                                               \
        template <typename C> static char &test(decltype(&C::func));         \
        template <typename C> static long &test(...);                        \
                                                                             \
    public:                                                                  \
        enum {                                                               \
            value = sizeof(test<T>(0)) == sizeof(char)                       \
        };                                                                   \
    }


/**
 *  \brief Check if class has named method.
 *
 *  Uses custom signature to resolve overloaded function types. The
 *  signature can use dependent typenames using `C` as the class name, so,
 *  for example, `void (C::*)(typename C::const_reference)` is  valid
 *  signature.
 *
 *  \param func             The name of the function to detect.
 *  \param name             The name of the resulting SFINAE detection class.
 *  \param signature        The type signature, using `C` for the class name.
 *
 *  \code
 *      PYCPP_HAS_MEMBER_FUNCTION(push_back, has_push_back,
 *                                void (C::*)(typename C::const_reference));
 */
#define PYCPP_HAS_MEMBER_FUNCTION3(func, name, signature)                    \
    template <typename T>                                                    \
    class name                                                               \
    {                                                                        \
    protected:                                                               \
        template <typename C, signature = &C::func> static char &test(C*);   \
        template <typename C> static long &test(...);                        \
                                                                             \
    public:                                                                  \
        enum {                                                               \
            value = sizeof(test<T>(0)) == sizeof(char)                       \
        };                                                                   \
    }

// Macro magic to detect the proper macro overload.
#define PYCPP_HAS_MEMBER_FUNCTION_ID(x) x
#define GET_PYCPP_HAS_MEMBER_FUNCTION(_1,_2,_3,NAME,...) NAME
#define PYCPP_HAS_MEMBER_FUNCTION(...) PYCPP_HAS_MEMBER_FUNCTION_ID(GET_PYCPP_HAS_MEMBER_FUNCTION(__VA_ARGS__, PYCPP_HAS_MEMBER_FUNCTION3, PYCPP_HAS_MEMBER_FUNCTION2)(__VA_ARGS__))

PYCPP_END_NAMESPACE
