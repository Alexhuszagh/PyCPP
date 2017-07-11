//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Python-like stackful coroutines for C++.
 */

#pragma once

#include <pycpp/config.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Type-aware coroutine definition.
 */
template <typename T>
struct coro_t
{
public:
    void yield(const T&);
    void yield(T&&);
    T get();

protected:
    // TODO: need an iterator facade and all that...
    // TODO
};

// IMPLEMENTATION
// --------------

template <typename T>
void coro_t<T>::yield(const T& t)
{
    // TODO: implement...
}


template <typename T>
void coro_t<T>::yield(T&& t)
{
    // TODO: implement...
}


template <typename T>
T coro_t<T>::get()
{
    return T();
    // TODO: implement...
}

// MACROS
// ------

/**
 *  \brief Declare a stackful coroutine.
 *
 *  Use macro-magic to make it look like a function declaration,
 *  when in reality it's a wrapped class.
 */
#define COROUTINE_DECL(type, name)              \
    struct name##_Class                         \
    {                                           \
        coro_t<type> _coroutine;                \
        void body();                            \
    };                                          \
                                                \
    coro_t<type> name()                         \
    {                                           \
        name##_Class inst;                      \
        inst.body();                            \
        return std::move(inst._coroutine);      \
    }                                           \
                                                \
    void name##_Class::body()


/**
 *  \brief Yield value from function into coroutine.
 */
#define COROUTINE_YIELD(value) this->_coroutine.yield(value)

/**
 *  \brief Get value from coroutine.
 */
#define COROUTINE_GET(coroutine) coroutine.get()

// Coroutines need to be iterable.

// libcoroutine should simplify things:
//      http://www.dekorte.com/projects/opensource/libcoroutine/
// TODO: consider implementing via
//      http://www.1024cores.net/home/lock-free-algorithms/tricks/fibers
// Also consider looking at:
//      https://github.com/rethinkdb/rethinkdb/blob/next/src/arch/runtime/coroutines.hpp

PYCPP_END_NAMESPACE
