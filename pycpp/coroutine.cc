//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/coroutine.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


bool coroutine::is_child() const
{
    return value_ < 0;
}


bool coroutine::is_parent() const
{
    return !is_child();
}


bool coroutine::is_complete() const
{
    return value_ == -1;
}


coroutine_ref::coroutine_ref(coroutine& c):
    value_(c.value_),
    modified_(false)
{}


coroutine_ref::coroutine_ref(coroutine* c):
    value_(c->value_),
    modified_(false)
{}


coroutine_ref::~coroutine_ref()
{
    if (!modified_) {
        value_ = -1;
    }
}


coroutine_ref::operator int() const
{
    return value_;
}


int& coroutine_ref::operator=(int v)
{
    modified_ = true;
    value_ = v;
    return value_;
}


PYCPP_END_NAMESPACE
