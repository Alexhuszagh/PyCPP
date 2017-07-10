//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure memory utilities.
 */

#pragma once

#include <cstdlib>

// FUNCTIONS
// ---------

/**
 *  \brief Securely clear buffer, preventing compiler optimizations.
 *
 *  Use this rather bzero.
 */
void secure_zero(void* dst, size_t bytes);

/**
 *  \brief Securely set values into buffer, preventing compiler optimizations.
 *
 *  Use this rather memset.
 */
void* secure_memset(void* dst, int c, size_t bytes);

/**
 *  \brief Securely copy from src to dst, preventing compiler optimizations.
 *
 *  Use this rather memcpy.
 */
void* secure_memcpy(void* dst, const void* src, size_t bytes);

/**
 *  \brief Securely move from src to dst, preventing compiler optimizations.
 *
 *  Use this rather memmove.
 */
void* secure_memmove(void* dst, const void* src, size_t bytes);

/**
 *  \brief Securely check if src == dst, preventing compiler optimizations.
 *
 *  Use this rather memcmp.
 */
int secure_memcmp(const void* lhs, const void* rhs, size_t bytes);

/**
 *  \brief Securely allocate memory, similar to libsodium.
 */
void* secure_malloc(size_t size);

/**
 *  \brief Securely allocate and clear memory, similar to libsodium.
 */
void* secure_calloc(size_t num, size_t size);

/**
 *  \brief Free securely allocated data.
 */
void secure_free(void* ptr);

/**
 *  \brief Block access to memory at ptr.
 */
int secure_mprotect_noaccess(void *ptr);

/**
 *  \brief Set read-only access to memory at ptr.
 */
int secure_mprotect_readonly(void *ptr);

/**
 *  \brief Restore read-write access to memory at ptr.
 */
int secure_mprotect_readwrite(void *ptr);

/**
 *  \brief Lock access to memory
 */
int secure_mlock(void* ptr, const size_t len);

/**
 *  \brief Unlock access to memory
 */
int secure_munlock(void* ptr, const size_t len);