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
using std::mutex;
using std::recursive_mutex;
using std::timed_mutex;
using std::recursive_timed_mutex;
using std::once_flag;
using std::adopt_lock_t;
using std::defer_lock_t;
using std::try_to_lock_t;
using std::lock_guard;
using std::unique_lock;

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
