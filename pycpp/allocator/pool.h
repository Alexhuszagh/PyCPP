// TODO: implement
// Pool allocators take advantage of fixed-size allocations
// Array allocations make no sense, since they break this.
// It will therefore work with std::list, and not std::vector,
// since items must be requested 1x1.

// Boost has a fast pool allocator, which works like this.
// It all has a pool allocator, which can handle custom shapes.

// I might need a custom mutex for the implementation...
// This allocator is not thread-safe, which...

#pragma once

#include <pycpp/config.h>
