//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/timeout.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


timeout_t::timeout_t(const std::chrono::milliseconds &timeout):
    timeout(timeout)
{}


timeout_t::timeout_t(long ms):
    timeout(std::chrono::milliseconds(ms))
{}


long timeout_t::seconds() const
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    check_limits(seconds);

    return static_cast<long>(seconds.count());
}


long timeout_t::milliseconds() const
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    check_limits(ms);

    return static_cast<long>(ms.count());
}


long timeout_t::microseconds() const
{
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(timeout);
    check_limits(us);

    return static_cast<long>(us.count());
}


timeout_t::operator bool() const
{
    return bool(timeout.count());;
}

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
