# STL

**Table of Contents**

- [Introduction](#introduction)
- [Changes](#changes)
  - [Allocator](#allocator)
  - [Hash](#hash)
  - [File Streams](#file-streams)
  - [Backports](#backports)
- [Aliases](#aliases)

## Introduction

This module defines custom aliases for Standard Template Library (STL) types, including backports and aliases for PyCPP's custom allocator and hash model. To use a type, for example, a vector, you would traditionally use the following:

```cpp
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    // do stuff with vector

    return 0;
}
```

## Changes

### Allocator

The largest changes by far have been to the allocator model, which uses C++17's implementation of `polymorphic_allocator` as the default allocator, based on [Technical Report P0336R1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0336r1.pdf).

In PyCPP, to allow custom allocators without breaking template type compatibility, by default all types use a [polymorphic allocator](/pycpp/allocator/polymorphic.h). We therefore have created custom aliases of STL template types to allow the default allocator. To use types compatible with PyCPP's high-level API, simple use the aliases type in [pycpp/stl](/pycpp/stl):

```cpp
#include <pycpp/stl/vector.h>

PYCPP_USING_NAMESPACE

int main()
{
    vector<int> v;
    v.emplace_back(1);
    // add code here...

    return 0;
}
```

To use custom allocators, see the [allocator documentation](/pycpp/allocator/README.md). To disable defaulting to `polymorphic_allocator`, simply configure the project by passing `-DUSE_POLYMORPHIC_ALLOCATORS=OFF` to CMake.

### Hash

C++ depends on fast hash functions for unordered container performance, and the hash function performance varies wildly in quality by compiler and compiler version. While GCC provides a good hash function for both [performance and hash quality]((https://gcc.gnu.org/git/?p=gcc.git;a=blob_plain;f=libstdc%2b%2b-v3/libsupc%2b%2b/hash_bytes.cc;hb=HEAD) (MurmurHash 2, as of version 7.2), MSVC uses a relatively [poor](https://docs.microsoft.com/en-us/cpp/porting/fix-your-dependencies-on-library-internals) hash function (FNV 1a). Furthermore, both hash functions are dramatically slower than [xxHash](https://aras-p.info/blog/2016/08/02/Hash-Functions-all-the-way-down/). Replacing the default hash function with xxHash provides increased performance, hash quality, and consistency between different platforms.

For small hashes, such as primitive types and wrappers around primitive types (like `std::error_condition`), our custom hasher reuses the `std::hash` implementation. For larger hashes, like `std::string`, our custom hasher uses xxHash internally.

 To disable defaulting to `xxhash`, simply configure the project by passing `-DUSE_XXHASH=OFF` to CMake.

### File Streams

PyCPP assumes data is [UTF-8 encoded](http://utf8everywhere.org/), since is compact, universal, and backwards compatible with ASCII. However, certain operating systems, most notably Windows, use UTF-16 for their filesystem API. To reliably open files with Unicode names, one must use the UTF-16 filesystem API, requiring the conversion of data from multi- to single-byte character sets for subsequent processing.

PyCPP therefore separates the path and stream character sets: all streams read and write narrow (char-based) data, while the file streams accept both UTF-8 and native paths (UTF-16 on Windows). Internally, the file streams convert paths to the native encoding, create a C `FILE` stream from the path, and wrap the low-level stream for I/O operations. This implementation is similar to both GCC and Clang, and therefore provides native performance for the underlying file streams.

PyCPP's separation of the path and stream character set differs significantly from a similar strategy found in [Boost.Filesystem](https://github.com/boostorg/filesystem/blob/master/include/boost/filesystem/fstream.hpp#L27). For certain compilers without wide character overloads for narrow streams, Boost internally converts UTF-16 wide paths to ANSI paths, which does not work for the majority of Unicode code points. PyCPP, on the other hand, uses Unicode filesystem APIs on all platforms, ensuring international filename support.

## Backports

C++17 includes novel STL containers which provide much-needed functionality to C++. However, all these types are fully standards-compliant with a C++11 compiler, so we provide backports for them. Please reference the STL documentation for each of the following types, as it will not be duplicated here:

 - [any](/pycpp/stl/any.h)
 - [optional](/pycpp/stl/optional.h)
 - [variant](/pycpp/stl/variant.h)

## Aliases

Nearly all C++11 and higher headers export the `std` namespace into the `pycpp` namespace for convenience. These aliases address various shortcomings in the STL versions, such as partial type_traits support in early C++11 compilers, and should be a drop-in replacement for the STL variants. The only substantial difference is in the aliases for the STL containers, which use `allocator` (generally an alias for `polymorphic_allocator`) by default rather than `std::allocator`. Therefore, the `pycpp` namespace is a near-analog of the `std::pmr` namespace.
