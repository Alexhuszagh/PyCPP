//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/coroutine.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


bool coroutine_t::is_child() const
{
    return value_ < 0;
}


bool coroutine_t::is_parent() const
{
    return !is_child();
}


bool coroutine_t::is_complete() const
{
    return value_ == -1;
}


coroutine_ref_t::coroutine_ref_t(coroutine_t& c):
    value_(c.value_),
    modified_(false)
{}


coroutine_ref_t::coroutine_ref_t(coroutine_t* c):
    value_(c->value_),
    modified_(false)
{}


coroutine_ref_t::~coroutine_ref_t()
{
    if (!modified_) {
        value_ = -1;
    }
}


coroutine_ref_t::operator int() const
{
    return value_;
}


int& coroutine_ref_t::operator=(int v)
{
    modified_ = true;
    value_ = v;
    return value_;
}


PYCPP_END_NAMESPACE
