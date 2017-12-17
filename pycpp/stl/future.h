//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <future> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <future>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::promise;
using std::packaged_task;
using std::future;
using std::shared_future;
using std::launch;
using std::future_status;
using std::future_error;
using std::future_errc;
using std::async;
using std::future_category;
using std::swap;

PYCPP_END_NAMESPACE
