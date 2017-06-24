crosscxx
========

Utilities for cross-platform C++ development.

[![Build Status](https://travis-ci.org/Alexhuszagh/crosscxx.svg?branch=master)](https://travis-ci.org/Alexhuszagh/crosscxx)
[![Build Status](https://tea-ci.org/api/badges/Alexhuszagh/crosscxx/status.svg)](https://tea-ci.org/Alexhuszagh/crosscxx)
[![Build status](https://ci.appveyor.com/api/projects/status/jx4mmgo25myx9u9i?svg=true)](https://ci.appveyor.com/project/Alexhuszagh/crosscxx)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Alexhuszagh/crosscxx/blob/master/LICENSE.md)

**Table of Contents**

- [Features](#features)
- [Building](#building)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Features

- Compiler, OS, processor, system architecture, and endian detection macros.
- Thread-local storage
- Unicode code point conversions (UTF8, UTF16, and UTF32).
- Punycode encoding and decoding.
- Url encoding and decoding
- `getline` that recognizes '\r', '\n', and '\r\n'.
- Cross-platform `gettimeofday`.
- Cross-compiler wide string stream overloads for Windows.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/crosscxx.git
git submodule update --init --recursive
cmake .                         # `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild crosscxx.sln" for MSVC
```

## Platforms

crosscxx is continually built with the following compiler and compiler versions:

- MinGW 5.3.0 (MXE, MinGW, and MSYS2) 
- Visual Studio 14 2015
- Visual Studio 15 2017

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
