# Roadmap

**Short-Term**

    - Need path views.
        
    - The robin map needs to use std::pair<const Key, Value> rather than it's bullshit.

    - Need to ensure all the streams have a low-level API and that is used for... well, everything.
        - We want to minimize the number of things an allocator can't be used for.
        - algorithm -- DONE
        - allocator -- DONE
        - cache -- DONE
        - cipher -- DONE
        - collections -- DONE
        - compression -- DONE
        - csv -- DONE
        - filesystem
        - fixed -- DONE
        - hashlib -- DONE
        - iterator -- DONE
        - itertools -- DONE
        - json -- DONE
        - lattice
            - Up to crypto....
            - digest
            - dns -- done
            - header -- done
            - method -- done
            - multipart -- done
            - parameter -- done
            - proxy -- done
            - redirect -- done
            - request
            - response
            - ssl
            - timeout
            - url
            - util -- DONE
        - lexical -- DONE
        - math -- DONE
        - memmap -- DONE
        - misc -- DONE
        - multi_index -- DONE
        - preprocessor -- DONE
        - random -- DONE
        - re -- DONE
        - reference -- DONE
        - runtime -- DONE
        - safe -- DONE
        - secure -- DONE
        - sfinae -- DONE
        - sql -- DONE
        - stream -- DONE
        - string -- DONE
        - view -- DONE
        - windows -- DONE
        - xml -- DONE
    - Need to implement splititer
    - Implement more reference (intrusive) containers.

    - Implement a sorted vector
        - unittests
    - Ordered set unittests
    - Add tests for robin-hood hash into the project
    - Add tests from btree-cpp into the project
        - Need to add support for C++11 stuff...

    - Add custom memory allocators
        - https://github.com/mtrebi/memory-allocators
        - Need an alias for the secure memory allocator
    - Add benchmarks compared to STL variants

    - Implement fixed containers
        - Add to CMakeLists

    - Need to include SQL header files in CMakeLists
    - Need to implement the interpolation search
    - Implement a ranked set maybe?
    - Implement an indexed set maybe?
    - Implement EASTL fixed containers as an extension of the container + stack allocator
        - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#intrusive_containers
    - fileutils?
    - ioutils?

**Mid-Term**

- Need an API around a key/value store.

- Update AutoCOM so it is using the modern library
    - Need install targets
    - Same for PyCPP
    - Track headers for install targets

**Long-Term**

- Work on the SQL bindings
    - It should provide an interface opaque to the actual SQL database...
    - Need to add SQL logic for the backends to the CMakeLists -- done
    - Need to find system installations

- Use DyND for the NumPy interface
    - Wrap it nicely
    - http://libdynd.org/introduction/

- Mathlib
    - Need array/axis unittests

- Atomic Operations
    - Check boltsons

- Binary serialization
    - Add cereal...

- Math
    - Need a NumPy-like container
    - Needs to be an N-D array
