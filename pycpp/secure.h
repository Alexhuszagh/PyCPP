//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure buffer and allocator implementation.
 *
 *  Secure storage for sensitve information, using libsodium wrappers
 *  to try to ensure data confidentiality.
 */

#pragma once

#include <pycpp/secure/allocator.h>
#include <pycpp/secure/string.h>
