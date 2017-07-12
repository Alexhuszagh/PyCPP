//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Python-like stackful coroutines for C++.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/compiler.h>
#include <pycpp/ordering.h>
#include <iterator>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------


#define CORO_REENTER(c)                             \
  switch (coroutine_ref _coro_value = c)            \
    case -1: if (_coro_value)                       \
    {                                               \
      goto terminate_coroutine;                     \
      terminate_coroutine:                          \
      _coro_value = -1;                             \
      goto bail_out_of_coroutine;                   \
      bail_out_of_coroutine:                        \
      break;                                        \
    }                                               \
    else case 0:


#define CORO_YIELD_IMPL(n)                          \
  for (_coro_value = (n);;)                         \
    if (_coro_value == 0)                           \
    {                                               \
      case (n): ;                                   \
      break;                                        \
    }                                               \
    else                                            \
      switch (_coro_value ? 0 : 1)                  \
        for (;;)                                    \
          case -1: if (_coro_value)                 \
            goto terminate_coroutine;               \
          else for (;;)                             \
            case 1: if (_coro_value)                \
              goto bail_out_of_coroutine;           \
            else case 0:


#define CORO_FORK_IMPL(n)                           \
  for (_coro_value = -(n);; _coro_value = (n))      \
    if (_coro_value == (n))                         \
    {                                               \
      case -(n): ;                                  \
      break;                                        \
    }                                               \
    else


#if defined(HAVE_MSVC)              // MSVC
#   define CORO_YIELD CORO_YIELD_IMPL(__COUNTER__ + 1)
#   define CORO_FORK CORO_FORK_IMPL(__COUNTER__ + 1)
#else                               // NOT MSVC
#   define CORO_YIELD CORO_YIELD_IMPL(__LINE__)
#   define CORO_FORK CORO_FORK_IMPL(__LINE__)
#endif                              // MSVC

// FORWARD
// -------

struct coroutine;
struct coroutine_ref;
template <typename T> struct generator_iterator;
template <typename T> struct generator;

// DECLARATION
// -----------

/**
 *  \brief Core coroutine object.
 */
struct coroutine
{
public:
    bool is_child() const;
    bool is_parent() const;
    bool is_complete() const;

private:
    friend class coroutine_ref;
    int value_ = 0;
};


/**
 *  \brief Reference for current position of the coroutine.
 */
struct coroutine_ref
{
public:
    coroutine_ref(coroutine& c);
    coroutine_ref(coroutine* c);
    ~coroutine_ref();
    operator int() const;
    int& operator=(int v);

private:
//    coroutine_ref(const coroutine_ref&) = default;
//    coroutine_ref& operator=(const coroutine_ref&) = default;
    int& value_;
    bool modified_ = false;
};



/**
 *  \brief Iterator wrapper for the generator.
 */
template <typename T>
struct generator_iterator: std::iterator<std::input_iterator_tag, T>
{
public:
    // MEMBER TYPES
    // ------------
    typedef std::iterator<std::input_iterator_tag, T> base;
    typedef generator_iterator<T> self;
    using typename base::value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS
    generator_iterator();
    generator_iterator(const self&);
    self& operator=(const self&);
    generator_iterator(self&&);
    self& operator=(self&&);
    generator_iterator(generator<T>& generator);
    generator_iterator(generator<T>* generator);

    // OPERATORS
    self& operator++();
    self operator++(int);
    pointer operator->();
    const_pointer operator->() const;
    reference operator*();
    const_reference operator*() const;

    // OTHER
    void swap(self&);

    // RELATIONAL
    template <typename U>
    friend bool operator==(const generator_iterator<U>&, const generator_iterator<U>&);

    template <typename U>
    friend bool operator!=(const generator_iterator<U>&, const generator_iterator<U>&);

private:
    generator<T>* generator_ = nullptr;
};


/**
 *  \brief Coroutine generator, analogous to Python's push/pull.
 */
template <typename T>
struct generator
{
public:
    // MEMBER TYPES
    // ------------
    typedef generator<T> self;
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef generator_iterator<T> iterator;

    // MEMBER FUNCTIONS
    // ----------------

    // ITERATORS
    iterator begin();
    iterator end();

    // DATA
    virtual void operator()();
    void store(const value_type& value);
    void store(value_type&& value);
    reference get();
    const_reference get() const;
    bool initialized() const;
    explicit operator bool() const;

protected:
    using mem_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    bool initialized_ = false;
    mem_t data_;
    coroutine coro_;
};


// IMPLEMENTATION
// --------------


template <typename T>
generator_iterator<T>::generator_iterator()
{}


template <typename T>
generator_iterator<T>::generator_iterator(generator<T>& generator):
    generator_(&generator)
{}


template <typename T>
generator_iterator<T>::generator_iterator(generator<T>* generator):
    generator_(generator)
{}


template <typename T>
generator_iterator<T>::generator_iterator(const self& rhs):
    generator_(rhs.generator_)
{}


template <typename T>
auto generator_iterator<T>::operator=(const self& rhs) -> self&
{
    generator_ = rhs.generator_;
    return *this;
}


template <typename T>
auto generator_iterator<T>::operator++() -> self&
{
    if (!generator_) {
    } else if (!*generator_) {
        generator_ = nullptr;
    } else {
        if (!generator_->initialized()) {
            (*generator_)();
        }

        (*generator_)();
        if (!*generator_) {
            generator_ = nullptr;
        }
    }

    return *this;
}


template <typename T>
auto generator_iterator<T>::operator++(int) -> self
{
    self copy(*this);
    operator++();
    return copy;
}


template <typename T>
auto generator_iterator<T>::operator->() -> pointer
{
    return &generator_->get();
}


template <typename T>
auto generator_iterator<T>::operator->() const -> const_pointer
{
    return &generator_->get();
}


template <typename T>
auto generator_iterator<T>::operator*() -> reference
{
    return generator_->get();
}


template <typename T>
auto generator_iterator<T>::operator*() const -> const_reference
{
    return generator_->get();
}


template <typename T>
generator_iterator<T>::generator_iterator(self&& rhs)
{
    swap(rhs);
}


template <typename T>
auto generator_iterator<T>::operator=(self&& rhs) -> self&
{
    swap(rhs);
    return *this;
}


template <typename T>
void generator_iterator<T>::swap(self& rhs)
{
    std::swap(generator_, rhs.generator_);
}


template <typename U>
bool operator==(const generator_iterator<U>& lhs, const generator_iterator<U>& rhs)
{
    return lhs.generator_ == rhs.generator_;
}


template <typename U>
bool operator!=(const generator_iterator<U>& lhs, const generator_iterator<U>& rhs)
{
    return not_equal_to(lhs, rhs);
}


template <typename T>
auto generator<T>::begin() -> iterator
{
    return iterator(this);
}


template <typename T>
auto generator<T>::end() -> iterator
{
    return iterator();
}


template <typename T>
void generator<T>::operator()()
{}


template <typename T>
void generator<T>::store(const value_type& value)
{
    new(&data_) value_type(value);
    initialized_ = true;
}


template <typename T>
void generator<T>::store(value_type&& value)
{
    new(&data_) value_type(std::forward<value_type>(value));
    initialized_ = true;
}


template <typename T>
auto generator<T>::get() -> reference
{
    return reinterpret_cast<reference>(data_);
}


template <typename T>
auto generator<T>::get() const -> const_reference
{
    return reinterpret_cast<const_reference>(data_);
}


template <typename T>
bool generator<T>::initialized() const
{
    return initialized_;
}


template <typename T>
generator<T>::operator bool() const
{
    return !coro_.is_complete();
}

//
//// FORWARD
//// -------
//
//template <typename T>
//struct coro_iterator_t;
//
//template <typename T>
//struct coro_t;
//
//// DECLARATION
//// -----------
//
//
///**
// *  \brief Iterator facade for a coroutine.
// */
//template <typename T>
//struct coro_iterator_t
//{
//    // TODO: here...
//};
//
//
///**
// *  \brief Type-aware coroutine definition.
// */
//template <typename T>
//struct coro_t
//{
//public:
//    // MEMBER TYPES
//    // ------------
//    typedef T value_type;
//
//    // MEMBER FUNCTIONS
//    // ----------------
//
//    void yield(T&&);
//    T get();
//
//    explicit operator bool() const;
//
//protected:
//    using mem_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
//    mem_t data_;
//
//    // TODO: need an iterator facade and all that...
//    // TODO: need to switch stacks, etc...
//};
//
//// IMPLEMENTATION
//// --------------
//
//
//template <typename T>
//void coro_t<T>::yield(T&& t)
//{
//    new(&data_) value_type(std::forward<T>(t));
////    suspend();
//    // TODO: implement...
//}
//
//
//template <typename T>
//T coro_t<T>::get()
//{
//    return T();
//    // TODO: implement...
//}
//
//
//template <typename T>
//coro_t<T>::operator bool() const
//{
//    // TODO: implement...
//    return false;
//}
//
//// MACROS
//// ------
//
///**
// *  \brief Declare a stackful coroutine.
// *
// *  Use macro-magic to make it look like a function declaration,
// *  when in reality it's a wrapped class.
// */
//#define COROUTINE_DECL(type, name, ...)                 \
//    struct name##_class_wrapper                         \
//    {                                                   \
//        coro_t<type> _coroutine;                        \
//        void body(__VA_ARGS__);                         \
//    };                                                  \
//                                                        \
//    template <typename... Ts>                           \
//    coro_t<type> name(Ts&&... ts)                       \
//    {                                                   \
//        name##_class_wrapper inst;                      \
//        inst.body(std::forward<Ts>(ts)...);             \
//        return std::move(inst._coroutine);              \
//    }                                                   \
//                                                        \
//    void name##_class_wrapper::body(__VA_ARGS__)
//
//
///**
// *  \brief Yield value from function into coroutine.
// */
//#define COROUTINE_YIELD(value) this->_coroutine.yield(value)
//
///**
// *  \brief Get value from coroutine.
// */
//#define COROUTINE_GET(coroutine) coroutine.get()
//
//// TODO: need a callback for the coroutine use...
//
//// Coroutines need to be iterable.
//
//// libcoroutine should simplify things:
////      http://www.dekorte.com/projects/opensource/libcoroutine/
//// TODO: consider implementing via
////      http://www.1024cores.net/home/lock-free-algorithms/tricks/fibers
//// Also consider looking at:
////      https://github.com/rethinkdb/rethinkdb/blob/next/src/arch/runtime/coroutines.hpp

PYCPP_END_NAMESPACE
