//  :copyright: (c) 2016 Denilson das Mercês Amorim.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Any implementation for C++11.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>

#if defined(HAVE_CPP17)             // HAVE_CPP17
#   include <any>
#else                               // !HAVE_CPP17
#   include <pycpp/collections/utility.h>
#   include <typeinfo>
#   include <type_traits>
#   include <stdexcept>
#endif                              // HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_CPP17)             // HAVE_CPP17

using any = std::any;
using bad_any_cast = std::bad_any_cast;


template <typename T, typename... Ts>
auto any_cast(Ts&&... ts) -> decltype(std::any_cast<T>(std::forward<Ts>(ts)...))
{
  return std::any_cast<T>(std::forward<Ts>(ts)...);
}


template <typename T, typename... Ts>
auto make_any(Ts&&... ts) -> decltype(std::make_any<T>(std::forward<Ts>(ts)...))
{
  return std::make_any<T>(std::forward<Ts>(ts)...);
}

#else                               // !HAVE_CPP17

// DECLARATIONS
// ------------

struct bad_any_cast: public std::bad_cast
{
    virtual const char* what() const noexcept override;
};


struct any
{
private:
//    storage_union storage;
//    vtable_type*  vtable;
};

#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE
