//  :copyright: (c) 2003-2013 Christopher M. Kohlhoff.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Bolost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Python-like stackful coroutines for C++.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/compiler.h>
#include <pycpp/ordering.h>
#include <functional>
#include <iterator>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------


#define COROUTINE_REENTER(c)                        \
  switch (coroutine_ref_t _coro_value = c)          \
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


#define COROUTINE_YIELD_IMPL(n)                     \
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


#define COROUTINE_FORK_IMPL(n)                      \
  for (_coro_value = -(n);; _coro_value = (n))      \
    if (_coro_value == (n))                         \
    {                                               \
      case -(n): ;                                  \
      break;                                        \
    }                                               \
    else


#if defined(HAVE_MSVC)              // MSVC
#   define COROUTINE_YIELD COROUTINE_YIELD_IMPL(__COUNTER__ + 1)
#   define COROUTINE_FORK COROUTINE_FORK_IMPL(__COUNTER__ + 1)
#else                               // NOT MSVC
#   define COROUTINE_YIELD COROUTINE_YIELD_IMPL(__LINE__)
#   define COROUTINE_FORK COROUTINE_FORK_IMPL(__LINE__)
#endif                              // MSVC

// FORWARD
// -------

struct coroutine_t;
struct coroutine_ref_t;
template <typename T> struct generator_iterator;
template <typename T> struct generator;

// DECLARATION
// -----------

/**
 *  \brief Core coroutine object.
 */
struct coroutine_t
{
public:
    bool is_child() const;
    bool is_parent() const;
    bool is_complete() const;

private:
    friend class coroutine_ref_t;
    int value_ = 0;
};


/**
 *  \brief Reference for current position of the coroutine.
 */
struct coroutine_ref_t
{
public:
    coroutine_ref_t(coroutine_t& c);
    coroutine_ref_t(coroutine_t* c);
    ~coroutine_ref_t();
    operator int() const;
    int& operator=(int v);

private:
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
    generator(std::function<void(self&)>&& function);

    // ITERATORS
    iterator begin();
    iterator end();

    // PROPERTIES
    void operator()();
    coroutine_t& coroutine();

    // DATA
    void store(const value_type& value);
    void store(value_type&& value);
    reference get();
    const_reference get() const;
    explicit operator bool() const;

private:
    using mem_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    std::function<void(self&)> function_;
    mem_t data_;
    coroutine_t coroutine_;
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
generator<T>::generator(std::function<void(self&)>&& function):
    function_(std::move(function))
{
    operator()();
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
coroutine_t& generator<T>::coroutine()
{
    return coroutine_;
}


template <typename T>
void generator<T>::operator()()
{
    function_(*this);
}


template <typename T>
void generator<T>::store(const value_type& value)
{
    new(&data_) value_type(value);
}


template <typename T>
void generator<T>::store(value_type&& value)
{
    new(&data_) value_type(std::forward<value_type>(value));
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
generator<T>::operator bool() const
{
    return !coroutine_.is_complete();
}


// MACROS
// ------

PYCPP_END_NAMESPACE
