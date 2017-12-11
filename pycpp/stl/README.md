# STL

// TODO: Right now this only includes things from the STL I'm using.
// Later I need to export everything.

**Table of Contents**

- [Introduction](#introduction)
- [Changes](#changes)
  - [Allocator](#allocator)
  - [Hash](#hash)
  - [Backports](#backports)

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

## Backports

C++17 includes novel STL containers which provide much-needed functionality to C++. However, all these types are fully standards-compliant with a C++11 compiler, so we provide backports for them. Please reference the STL documentation for each of the following types, as it will not be duplicated here:

 - [any](/pycpp/stl/any.h)
 - [optional](/pycpp/stl/optional.h)
 - [variant](/pycpp/stl/variant.h)
