//  :copyright: (c) 2016 Mariano Trebino.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Helpers to wrap the key-value backend.
 */

// TODO: remove

#pragma once

#include <pycpp/config.h>
#include <cstddef>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Virtual base for a memory allocator.
 */
struct allocator_base
{
public:
//    allocator_base(const size_t total_size);
//    virtual ~allocator_base();
//    virtual void* allocate(size_t size, const size_t alignment = 0) = 0;
//    virtual void deallocate(void* ptr, size_t) = 0;
//    virtual void Init() = 0;

protected:
//    size_t total_size_;
//    size_t used_;
//    size_t peak_;
};


PYCPP_END_NAMESPACE
