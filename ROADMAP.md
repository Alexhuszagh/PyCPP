# Roadmap

**Short-Term**

    - Ordered map and ordered set need new constructors and move operators
        - Need to revert `std::allocator` to `allocator` after.
        
    - Change the default hash...

    - Look at use woever rope.

    - Make allocators non-optional, use a polymorphic allocator by default...
    - Allow a CMake flag to use polymorphic or the standard allocator by default 
            -- DONE
        - Implement the polymorphic allocator
            -- DONE (msotly)
            - Need to finish pool options, etc...

        - Implement all other allocators as a resource
            - DONE

        - Implement top-level types as a typedef of the STL containers using that as the default allocator...   
            -- DONE

        - Use all default types as aliases of the STL types...
            - Need to alias basically all public headers....
            - Lols shit...

        - Need a custom stringstream that takes a view or some shit, since that STL stringstream constructor is fucking dumb as shit.

    - The robin map needs to use std::pair<const Key, Value> rather than it's bullshit.

    - Implement more reference (intrusive) containers.
        - vector -- DONE
        - deque -- DONE

        - Rest of the STL?

    - Implement fixed containers
        - Add to CMakeLists
        - Vector -- DONE
        - Deque -- DONE
            - Bug, the allocators need to support a generic type in the constructor....
                - Need a generic T


        - ForwardList
        - List
        - Map
        - MultiMap
        - UnorderedMap
        - UnorderedMultiMap
        - Set
        - MultiSet
        - UnorderedSet
        - UnorderedMultiSet

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
