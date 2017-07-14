//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief High-level regular expression methods.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// TODO: need to implement a cache here..
// Abstract away the actual pattern/regex compilation.

// re.search(pattern, string, flags=0)
// re.match(pattern, string, flags=0)
// re.split(pattern, string, maxsplit=0, flags=0)
// re.findall(pattern, string, flags=0)
// re.finditer(pattern, string, flags=0)
// re.sub(pattern, repl, string, count=0, flags=0)
// re.subn(pattern, repl, string, count=0, flags=0)
// re.escape(pattern)
// re.purge()

// I can use an ordered_map for the queue...
// Purge items after N

PYCPP_END_NAMESPACE
