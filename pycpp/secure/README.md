# Secure

**Table of Contents**

- [Introduction](#introduction)
- [Allocator](#allocator)
- [Char Traits](#char-traits)
- [stdlib](#stdlib)
- [String](#string)

## Introduction

Side-channel attacks are a significant threat to cryptosystems, by allowing attackers to exploit the system rather than the cryptographic implementation. [Libsodium](https://github.com/jedisct1/libsodium) is a portable, easy-to-use cryptographic library with utilities to securely store data in memory. We therefore ported various libsodium utilities to PyCPP to design a secure memory buffer and allocator, with an STL string-like interface.

> **WARNING** Libsodium is not affilitated with this project in any way, and bugs during porting may have introduced security vulnerabilities.

## Allocator

The `secure_allocator` is a general-purpose allocator with page protection that securely wipes data prior to deallocation. Memory can be locked to the current process, and read or write access can be limited at will. For more information, see [allocators](/pycpp/allocator/README.md#secure).

## Char Traits

Character traits implementation analogous to `std::char_traits`. Currently, `secure_char_traits` is an alias for `std::char_traits`, however, in the future, it might be useful to implement it in terms of `secure_memcpy` and `secure_memmove` to avoid compiler optimizations.

## stdlib

Low-level C-like routines that handle secure memory allocation, protection, wiping, and locking. No STL containers are used internally, to avoid `std::allocator` or various optimizations at all costs.

## String

Secure memory buffer implemented as an STL-like string. `secure_string` is implemented as a naive string, without short-string optimization, to ensure memory is securely wiped upon deallocation. By default, it uses `secure_allocator`, and provides high-level routines to restrict read/write access to the buffer, and lock/unlock the memory.

