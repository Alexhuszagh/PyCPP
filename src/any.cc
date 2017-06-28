//  :copyright: (c) Kevlin Henney 2001.
//  :copyright: (c) Pablo Aguilar 2005.
//  :copyright: (c) Christopher Diggins 2005-2011.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include "any.h"
#include "ordering.h"

#include <utility>

// OBJECTS
// -------


bool empty_any::operator==(const empty_any&) const
{
    return true;
}


any::any():
    policy(get_policy<empty_any>()),
    object(nullptr)
{}


any::any(const any& other):
    policy(get_policy<empty_any>()),
    object(nullptr)
{
    assign(other);
}


any& any::operator=(const any& other)
{
    return assign(other);
}


any::~any()
{
    policy->static_delete(&object);
}


any::any(const char* x):
    policy(get_policy<empty_any>()),
    object(nullptr)
{
    assign(x);
}


any& any::assign(const any& other)
{
    reset();
    policy = other.policy;
    policy->clone(&other.object, &object);
    return *this;
}


void any::swap(any& other)
{
    std::swap(policy, other.policy);
    std::swap(object, other.object);
}


void any::reset()
{
    policy->static_delete(&object);
    policy = get_policy<empty_any>();
}


bool any::empty() const
{
    return policy == get_policy<empty_any>();
}


bool any::compatible(const any& other) const
{
    return policy == other.policy;
}


bool any::equals(const any& other) const
{
    void **l = const_cast<void**>(&object);
    void **r = const_cast<void**>(&other.object);
    return policy->equals(l, r);
}


bool any::operator==(const any& other) const
{
    int sum = empty() + other.empty();
    switch (sum) {
        case 0:
            return compatible(other) ? equals(other) : false;
        case 1:
            return false;
        case 2:
            return true;
    }
}


bool any::operator!=(const any& other) const
{
    return not_equal_to(*this, other);
}

