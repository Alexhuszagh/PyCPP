# Building

Building is mostly straightforward, with a few quirks on MSVC. The following describes the various configurations and options PyCPP supports. Please read the following build information before reporting any compiler/linker errors or bugs.

**Table of Contents**

- [Supported Compilers](#supported-compilers)
- [Optional Modules](#optional-modules)
- [Optional Targets](#optional-targets)
- [Generators](#generators)
- [Build Type](#build-type)
- [Shared Libraries](#shared-libraries)
- [System Libraries](#system-libraries)
- [LZMA](#lzma)

## Supported Compilers

PyCPP is built and tested with the following compilers and compiler versions:
    - GCC 5.4.0
    - GCC 6.4.0
    - GCC 7.2.1
    - (Add Clang)
    - (Add MSVC)
    - (Add flags)....

Testing GCC and Clang versions is easy on Linux, using Docker. To test a custom Docker version, run `docker run -it gcc:$VERSION bash`, where `VERSION` is the desired [GCC version](https://hub.docker.com/r/library/gcc/tags/).


## Optional Modules

PyCPP is modularized, and by default builds all modules. PyCPP defines the following modules:

| Module        | Description               | Flag              |
|:-------------:|:-------------------------:|:-----------------:|
| Bloom         | Bloom filters             | BLOOM             |
| Cipher        | Cryptographic ciphers     | CIPHER            |
| Collections   | STL container extensions  | COLLECTIONS       |
| Compression   | Compression filters       | COMPRESSION       |
| CSV           | CSV readers and writers   | CSV               |
| Cuckoo        | Fast, lossy hash algorithm| CUCKOO            |
| Datetime      | Date and time utilities   | DATETIME          |
| Filesystem    | Filesystem utilities      | FILESYSTEM        |
| Hashlib       | Cryptographic hashes      | HASHLIB           |
| JSON          | JSON readers and writers  | JSON              |
| Key-Value     | Build Key-Value Database  | KEYVALUE          |
| Lattice       | Build HTTP requests       | LATTICE           |
| Math          | Build math routines       | MATH              |
| MySQL         | Build MySQL wrapper       | MYSQL             |
| Postgres      | Build PostgreSQL wrapper  | POSTGRES          |
| Regex         | Build regular expressions | RE                |
| SQLite        | Build SQLite wrapper      | SQLITE            |
| I/O Streams   | Custom I/O streams        | STREAM            |
| XML           | XML readers and writers   | XML               |

Modules can be enabled or disabled in two ways. You may disable individual modules by setting `-DBUILD_$MODULE=OFF`, which will build all modules except those explicitly disabled. PyCPP also allows you to disable building all modules except those explicitly enabled, using the `-DWITH_$MODULE=ON`. If any `-DWITH_$MODULE=ON` is defined for any module, all modules except those explicitly enabled using the `WITH` syntax will be disabled. To disable all modules, pass `-DWITH_CORE=ON`.

For example, `-DWITH_BLOOM=ON` will build on the bloom filters, while `-DBUILD_BLOOM=OFF` will build all modules **but** the bloom filters.

## Optional Targets

PyCPP includes a unittesting suite and benchmark suite, which may be optionally enabled. To enable unittesting, pass `-DBUILD_TESTS=ON` during CMake configuration; to enable benchmakring, pass `-DBUILD_BENCHMARKS=ON` during CMake configuration.

## Generators

CMake currently supports Makefile and Visual Studio generators. It likely supports other generators out-of-the-box, however, only the following have been tested:

Valid generators include:
    - "Unix Makefiles"
    - "MinGW Makefiles"
    - "MSYS Makefiles"
    - "Visual Studio 14 2015"
    - "Visual Studio 14 2015 Win64"
    - "Visual Studio 15 2017"
    - "Visual Studio 15 2017 Win64"

To use the generator, simple add `-G "Unix Makefiles"`, or another custom generator during CMake configuration.

## Build Types

PyCPP supports either `Debug` or `Release` build types, but not both simultaneously. Although `CMAKE_BUILD_TYPE` is only relevant to a handful of generators by default, for PyCPP requires it for all generators since it uses external targets for configurations. A sample build is:

```bash
cd /path/to/pycpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Shared Libraries

PyCPP supports either shared or static libraries, as well as the optional use of [system libraries](#system-libraries). Configuration of static or shared libraries is done during CMake configuration, using the `BUILD_SHARED_LIBS` CMake variable. To build shared libraries, add `-DBUILD_SHARED_LIBS=ON` during CMake configuration; to build static libraries, add `-DBUILD_SHARED_LIBS=OFF` during CMake configuration. Not setting this variable will use the system default.

## System Libraries

CMake supports the optional use of system libraries, which may be installed previously or using a package manager. Each external dependency may be installed using a system library:

| Library | Flag                |
|:-------:|:-------------------:|
| Blosc   | USE_SYSTEM_BLOSC    |
| Bzip2   | USE_SYSTEM_BZIP2    |
| LevelDB | USE_SYSTEM_LEVELDB  |
| Libxml2 | USE_SYSTEM_LIBXML2  |
| XZ      | USE_SYSTEM_LZMA     |
| MySQL   | USE_SYSTEM_MYSQL    |
| Postgres| USE_SYSTEM_POSTGRES |
| RE2     | USE_SYSTEM_RE2      |
| SQLite  | USE_SYSTEM_SQLITE   |
| Zlib    | USE_SYSTEM_ZLIB     |

To use a system library, simple set it to `ON` during CMake configuration, for example, to use the system Zlib installation, add `-DUSE_SYSTEM_ZLIB=ON` to the configuration flags.

## LZMA

Linking with libc++ (LLVM) causes issues with the LZMA compressors and decompressors. Any patches would be wonderful, in the meantime, please use GCC or link with libstdc++.
