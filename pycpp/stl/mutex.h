//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <mutex> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <mutex>

PYCPP_BEGIN_NAMESPACE

// ALIASES
// -------

using std::try_lock;
using std::lock;
using std::call_once;
using std::swap;

using mutex = std::mutex;
using recursive_mutex = std::recursive_mutex;
using timed_mutex = std::timed_mutex;
using recursive_timed_mutex = std::recursive_timed_mutex;
using once_flag = std::once_flag;
using adopt_lock_t = std::adopt_lock_t;
using defer_lock_t = std::defer_lock_t;
using try_to_lock_t = std::try_to_lock_t;

template <typename Mutex>
using lock_guard = std::lock_guard<Mutex>;

template <typename Mutex>
using unique_lock = std::unique_lock<Mutex>;

// EXTENSIONS
// ----------

/**
 *  Mutex implementation that does nothing.
 */
struct dummy_mutex
{
    // MEMBER TYPES
    // ------------
    using native_handle_type = void;

    // MEMBER FUNCTIONS
    // ----------------
    constexpr dummy_mutex() noexcept = default;
    dummy_mutex(const dummy_mutex&) = delete;

    // LOCKING
    inline void lock()
    {}

    inline bool try_lock()
    {
        return true;
    }

    inline void unlock()
    {}

    native_handle_type native_handle()
    {}
};


PYCPP_END_NAMESPACE
