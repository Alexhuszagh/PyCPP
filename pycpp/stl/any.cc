//  :copyright: (c) 2016 Denilson das Mercês Amorim.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/stl/any.h>

#if !defined(HAVE_CPP17)             // !HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

const char* bad_any_cast::what() const noexcept
{
    return "bad any cast";
}


any::any() noexcept:
    vtable(nullptr)
{}


any::any(const any& rhs):
    vtable(rhs.vtable)
{
    if(rhs.has_value()) {
        rhs.vtable->copy(rhs.storage, storage);
    }
}


any::any(any&& rhs) noexcept:
    vtable(rhs.vtable)
{
    if(rhs.has_value()) {
        rhs.vtable->move(rhs.storage, storage);
        rhs.vtable = nullptr;
    }
}

any::~any()
{
    reset();
}


any& any::operator=(const any& rhs)
{
    any(rhs).swap(*this);
    return *this;
}


any& any::operator=(any&& rhs) noexcept
{
    any(std::move(rhs)).swap(*this);
    return *this;
}


void any::reset() noexcept
{
    if(has_value()) {
        vtable->destroy(storage);
        vtable = nullptr;
    }
}


bool any::has_value() const noexcept
{
    return vtable != nullptr;
}


const std::type_info& any::type() const noexcept
{
    return has_value() ? vtable->type() : typeid(void);
}


void any::swap(any& rhs) noexcept
{
    if(vtable != rhs.vtable) {
        any tmp(std::move(rhs));
        rhs.vtable = vtable;
        if(vtable != nullptr) {
            vtable->move(storage, rhs.storage);
        }

        vtable = tmp.vtable;
        if(tmp.vtable != nullptr) {
            tmp.vtable->move(tmp.storage, storage);
            tmp.vtable = nullptr;
        }
    } else {
        if(vtable != nullptr) {
            vtable->swap(storage, rhs.storage);
        }
    }
}

bool any::is_typed(const std::type_info& t) const
{
    return is_same(type(), t);
}


bool any::is_same(const std::type_info& a, const std::type_info& b)
{
    return a == b;
}

PYCPP_END_NAMESPACE

#endif                              // !HAVE_CPP17
