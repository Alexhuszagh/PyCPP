//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Secure memory utilities.
 *
 *  Low-level routines for secure memory utilities for cryptographic
 *  applications. These routines are ported from libsodium, and allow
 *  secure memory allocation and deallocation, page protection, memory
 *  locking, and zeroing.
 *
 *  1. https://github.com/jedisct1/libsodium
 *
 *  \synopsis
 *      void secure_zero(void* dst, size_t bytes) noexcept;
 *      void* secure_memset(void* dst, int c, size_t bytes) noexcept;
 *      void* secure_memcpy(void* dst, const void* src, size_t bytes) noexcept;
 *      void* secure_memmove(void* dst, const void* src, size_t bytes) noexcept;
 *      int secure_memcmp(const void* lhs, const void* rhs, size_t bytes) noexcept;
 *      void* secure_malloc(size_t size) noexcept;
 *      void* secure_calloc(size_t num, size_t size) noexcept;
 *      void secure_free(void* ptr) noexcept;
 *      int secure_mprotect_noaccess(void *ptr) noexcept;
 *      int secure_mprotect_readonly(void *ptr) noexcept;
 *      int secure_mprotect_readwrite(void *ptr) noexcept;
 *      int secure_mlock(void* ptr, size_t len) noexcept;
 *      int secure_munlock(void* ptr, size_t len) noexcept;
 */

#pragma once

#include <pycpp/config.h>
#include <stdlib.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Securely clear buffer, preventing compiler optimizations.
 *
 *  Use this rather than bzero for cryptographic applications.
 *
 *  \param dst              Buffer to zero.
 *  \param bytes            Number of bytes to zero.
 */
void secure_zero(void* dst, size_t bytes) noexcept;

/**
 *  \brief Securely set values into buffer, preventing compiler optimizations.
 *
 *  Use this rather than memset for cryptographic applications.
 *
 *  \param dst              Buffer to set to a specific value.
 *  \param c                Value to assign.
 *  \param bytes            Number of bytes to assign.
 */
void* secure_memset(void* dst, int c, size_t bytes) noexcept;

/**
 *  \brief Securely copy from src to dst, preventing compiler optimizations.
 *
 *  Use this rather than memcpy for cryptographic applications.
 *
 *  \param dst              Destination buffer.
 *  \param src              Source buffer.
 *  \param bytes            Number of bytes to copy.
 */
void* secure_memcpy(void* dst, const void* src, size_t bytes) noexcept;

/**
 *  \brief Securely move from src to dst, preventing compiler optimizations.
 *
 *  Use this rather than memmove for cryptographic applications.
 *
 *  \param dst              Destination buffer.
 *  \param src              Source buffer.
 *  \param bytes            Number of bytes to move.
 */
void* secure_memmove(void* dst, const void* src, size_t bytes) noexcept;

/**
 *  \brief Securely check if src == dst, preventing compiler optimizations.
 *
 *  Use this rather than memcmp for cryptographic applications.
 *
 *  \param lhs              First buffer.
 *  \param rhs              Second buffer.
 *  \param bytes            Number of bytes to compare between buffers.
 */
int secure_memcmp(const void* lhs, const void* rhs, size_t bytes) noexcept;

/**
 *  \brief Securely allocate memory, similar to libsodium.
 *
 *  \param size             Number of bytes to allocate.
 */
void* secure_malloc(size_t size) noexcept;

/**
 *  \brief Securely allocate and clear memory, similar to libsodium.
  *
 *  \param num              Number of elements to allocate.
 *  \param size             Bytes per element.
 */
void* secure_calloc(size_t num, size_t size) noexcept;

/**
 *  \brief Free securely allocated data.
 *
 *  \param ptr              Pointer to allocated memory.
 */
void secure_free(void* ptr) noexcept;

/**
 *  \brief Block access to memory at ptr.
 *
 *  \param ptr              Pointer to allocated memory.
 */
int secure_mprotect_noaccess(void *ptr) noexcept;

/**
 *  \brief Set read-only access to memory at ptr.
 *
 *  \param ptr              Pointer to allocated memory.
 */
int secure_mprotect_readonly(void *ptr) noexcept;

/**
 *  \brief Restore read-write access to memory at ptr.
 *
 *  \param ptr              Pointer to allocated memory.
 */
int secure_mprotect_readwrite(void *ptr) noexcept;

/**
 *  \brief Lock access to memory.
 *
 *  \param ptr              Pointer to allocated memory.
 *  \param len              Number of bytes to lock.
 */
int secure_mlock(void* ptr, size_t len) noexcept;

/**
 *  \brief Unlock access to memory.
 *
 *  \param ptr              Pointer to allocated memory.
 *  \param len              Number of bytes to unlock.
 */
int secure_munlock(void* ptr, size_t len) noexcept;

PYCPP_END_NAMESPACE
