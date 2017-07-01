//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Secure buffer and allocator implementation.
 *
 *  Secure storage for sensitve information, using libsodium wrappers
 *  to try to ensure data confidentiality.
 */

#pragma once

#include <secure/allocator.h>
#include <secure/string.h>
