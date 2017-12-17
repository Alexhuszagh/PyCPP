//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <execution> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <execution>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

namespace execution
{

using std::execution::is_execution_policy;
using std::execution::sequenced_policy;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::par;
using std::execution::par_unseq;

}   /* execution */

PYCPP_END_NAMESPACE
