//  :copyright: (c) 2001 Kevlin Henney.
//  :copyright: (c) 2005 Pablo Aguilar.
//  :copyright: (c) 2005-2011 Christopher Diggins.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup funxx
 *  \brief High-performance generic type implementation.
 */

#pragma once

#include <cstdlib>
#include <memory>

// FORWARD
// -------

struct bad_any_cast;
struct empty_any;
struct base_any_policy;
struct any;

// OBJECTS
// -------

struct bad_any_cast {};


/**
 *  \brief Placeholder value.
 */
struct empty_any
{
    bool operator==(const empty_any&) const;
};


/**
 *  \brief Shared policies for any.
 */
struct base_any_policy
{
    virtual void static_delete(void **x) = 0;
    virtual void copy_from_value(void const *src, void **dst) = 0;
    virtual void clone(void * const *src, void **dst) = 0;
    virtual void move(void * const *src, void **dst) = 0;
    virtual void * get_value(void **src) const = 0;
    virtual size_t get_size() = 0;
    virtual bool equals(void **src, void **dst) = 0;
};


/**
 *  \brief Shared policies for any.
 */
template <typename T>
struct typed_base_any_policy: base_any_policy
{
    virtual size_t get_size() override
    {
        return sizeof(T);
    }
};


/** \brief Primite type policies.
 */
template <typename T>
struct small_any_policy: typed_base_any_policy<T>
{
    virtual void static_delete(void **x) override
    {}

    virtual void copy_from_value(void const *src, void **dst) override
    {
        new(dst) T(*reinterpret_cast<T const*>(src));
    }

    virtual void clone(void * const *src, void **dst) override
    {
        *dst = *src;
    }

    virtual void move(void * const *src, void **dst) override
    {
        *dst = *src;
    }

    virtual void * get_value(void **src) const override
    {
        return reinterpret_cast<void*>(src);
    }

    virtual bool equals(void **src, void **dst) override
    {
        T *s = reinterpret_cast<T*>(get_value(src));
        T *d = reinterpret_cast<T*>(get_value(dst));

        return *s == *d;
    }
};


/**
 *  \brief Custom type policies.
 */
template <typename T>
struct big_any_policy: typed_base_any_policy<T>
{
    virtual void static_delete(void **x) override
    {
        if (*x) {
            delete(*reinterpret_cast<T**>(x));
            *x = NULL;
        }
    }

    virtual void copy_from_value(void const *src, void **dst) override
    {
        *dst = new T(*reinterpret_cast<T const*>(src));
    }

    virtual void clone(void * const *src, void **dst) override
    {
        *dst = new T(**reinterpret_cast<T* const*>(src));
    }

    virtual void move(void * const *src, void **dst) override
    {
        (*reinterpret_cast<T**>(dst))->~T();
        **reinterpret_cast<T**>(dst) = **reinterpret_cast<T* const*>(src);
    }

    virtual void * get_value(void **src) const override
    {
        return *src;
    }

    virtual bool equals(void **src, void **dst) override
    {
        T *s = reinterpret_cast<T*>(get_value(src));
        T *d = reinterpret_cast<T*>(get_value(dst));

        return *s == *d;
    }
};


template <typename T>
struct choose_policy
{
    typedef big_any_policy<T> type;
};


template <typename T>
struct choose_policy<T*>
{
    typedef small_any_policy<T*> type;
};


/** \brief Choose policy for any type bestides `any`.
 *
 *  \warning Throws compiler error if `any` is chosen.
 */
template <>
struct choose_policy<any>
{
    typedef void type;
};


/** \brief Specializations for primitive types.
 */
#define PRIMITIVE_TYPE(TYPE) template <> struct             \
    choose_policy<TYPE>                                     \
    {                                                       \
        typedef small_any_policy<TYPE> type;                \
    };


PRIMITIVE_TYPE(signed char);
PRIMITIVE_TYPE(unsigned char);
PRIMITIVE_TYPE(signed short);
PRIMITIVE_TYPE(unsigned short);
PRIMITIVE_TYPE(signed int);
PRIMITIVE_TYPE(unsigned int);
PRIMITIVE_TYPE(signed long);
PRIMITIVE_TYPE(unsigned long);
PRIMITIVE_TYPE(float);
PRIMITIVE_TYPE(bool);

#undef PRIMITIVE_TYPE


/**
 *  \brief Create unique policy for each type.
 */
template <typename T>
base_any_policy *get_policy()
{
    static typename choose_policy<T>::type policy;
    return &policy;
};


/**
 *  \brief Generic object that can store values of any data type.
 *
 *  Uses pointer indirection to store an opaque value which can
 *  be safely retrieved using SFINAE.
 */
struct any
{
public:
    any();
    any(const any&);
    any& operator=(const any&);
    ~any();

    any(const char*);
    template <typename T> any(const T&);
    template <typename T> any& operator=(const T&);

    // MODIFIERS
    any& assign(const any& other);
    template <typename T> any& assign(const T&);
    void swap(any&);
    void reset();

    // OBSERVERS
    bool empty() const;
    bool compatible(const any&) const;
    bool equals(const any&) const;
    bool operator==(const any&) const;
    bool operator!=(const any&) const;
    template <typename T> T& cast();
    template <typename T> const T& cast() const;

private:
    base_any_policy* policy;
    void* object;
};


// IMPLEMENTATION
// --------------


template <typename T>
any::any(const T& t):
    policy(get_policy<empty_any>()),
    object(nullptr)
{
    assign(t);
}


template <typename T>
any& any::operator=(const T& t)
{
    return assign(t);
}


template <typename T>
any& any::assign(const T& t)
{
    reset();
    policy = get_policy<T>();
    policy->copy_from_value(&t, &object);
    return *this;
}


template <typename T>
T& any::cast()
{
    if (policy != get_policy<T>()) {
        throw bad_any_cast();
    }
    T *r = reinterpret_cast<T*>(policy->get_value(&object));

    return *r;
}


template <typename T>
const T& any::cast() const
{
    if (policy != get_policy<T>()) {
        throw bad_any_cast();
    }
    T *r = reinterpret_cast<T*>(policy->get_value(const_cast<void**>(&object)));

    return *r;
}
