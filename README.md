fun++
=====
Independent, lightweight utilities for cross-platform C++ development.

[![Build Status](https://travis-ci.org/Alexhuszagh/funxx.svg?branch=master)](https://trAlexhuszagh/csv_parseravis-ci.org/Alexhuszagh/funxx)
[![Build Status](https://tea-ci.org/api/badges/Alexhuszagh/funxx/status.svg)](https://tea-ci.org/Alexhuszagh/funxx)
[![Build status](https://ci.appveyor.com/api/projects/status/b8fh8431rcu3wc3q?svg=true)](https://ci.appveyor.com/project/Alexhuszagh/funxx)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Alexhuszagh/funxx/blob/master/LICENSE.md)

**Table of Contents**

- [Introduction](#introduction)
- [Design](#design)
- [Building](#building)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Introduction

Modern C++ development depends on a mix of archaic C-libraries with poorly documented interfaces, and template-driven, header-only libraries that produce significant binary and compile time bloat. fun++, for fundamentals++, aims to be a collection loosely interconnected features missing from the C++ standard library, boasting lean headers, fast compile times, and permissive licenses, to facilitate app development with minimal overhead.

fun++ is, in many ways, a spiritual port of Python back to C++. There are idiomatic ways to do certain tasks, and there is a right and a wrong way. "Zero-cost abstractions" don't exist: enabling infinite abstraction leads to unmaintainable complexity.

## Design

fun++ is broken down into multiple core parts:

1. A macro-driven abstraction library to detect compiler features.
2. Encoding and decoding routines
3. Filesystem utilities.
4. High-level features common in other standard libraries.

Combined, the library has less than 1500 (is this still accurate??) total lines of C++ headers, with binary sizes of < 1MB, and each module has only a few dependencies, making it easy to extract individual modules for use in another project.

**Abstraction Library**

The files [os.h](/src/os.h), [compiler.h](/src/compiler.h), [architecture.h](/src/architecture.h), [processor.h](/src/processor.h), and [byteorder.h](/src/byteorder.h) provide an abstraction platform to detect the current compiler, compiler version, operating system, system endianness, and processor. These are the largest dependencies, with ~1000 lines of code.

**Encoding and Decoding**

Any modern application should be Unicode aware, and fun++ includes routines for [code point conversions](/src/unicode.h) and [codec conversions](codec.h), as well as [Unicode-aware case conversions](/src/casemap.h). They provide two APIs: a modern wrapper for STL string conversions, and a C-API taking a src and dst buffer. 

Support features include

- UTF-8, UTF-16, and UTF-32 conversions
- std::string, std::u16string, and std::u32string conversions.
- Lowercase, uppercase, and titlecase Unicode conversions.

**Fileystem**

**High-Level Features**

- [Url](/src/url.h) encoding and decoding.
- [Thread-local storage](/src/tls.h).
- [Bit-flag aware scoped enums](/src/enum.h).
- [Punycode](/src/punycode.h) encoding and decoding.
- [Base16](/src/base16.h), [base32](/src/base32.h), and [Base64](/src/base64.h) encoding and decoding.
- [Hex](/src/hex.h) encoding and decoding.
- [Wide API streams](/src/fstream.h) for Windows.
- [string_view](/str/string_view.h) and [vector_view](/src/vector_view.h)
- [Hash](/src/hashlib.h) functions.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/funxx.git
git submodule update --init --recursive
cmake .                         # `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild funxx.sln" for MSVC
```

## Platforms

fun++ is continually built with the following compiler and compiler versions:

- Clang 3.8+
- GCC 5.3+
- MinGW 5.3.0 (MXE, MinGW, and MSYS2) 
- Visual Studio 14 2015
- Visual Studio 15 2017

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
