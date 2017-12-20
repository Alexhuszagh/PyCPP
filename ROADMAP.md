# Roadmap

**Short-Term**

    - Get rid of naked new/delete
        - Change to use the global allocator generally speaking...

    - Make moves always non-throwing
        - Check if copies can be non-throwing too
        - Add a lot of noexcepts in my code....

    - Make sure my streams properly handle moves but cannot be copied...
        - CSV
            - Reader    - Skeleton
            - Writer    - Skeleton
            - Dict      - Skeleton
        - JSON
            - SAX       - Skeleton
            - Writer    - Skeleton
        - XML
            - SAX       - Skeleton
            - Writer    - Skeleton

    - Interprocess
        - Some sort of interprocess library like Boost would be great

    - Remove all superflous std:: shit
        - (ALMOST, ALMOST DONE)
        - Just need std::string...
            - LOLS
            - Change everything involving std::string to a typedef alias of it...
                - string_tmp or something...

    - Need custom allocators for the JSON and XML interfaces
    - Look at use woever rope.

    - Implement various useful allocators
        - Need a dummy mutex and locked and unlocked variants of allocators
        - Free list
            - All the allocators from here: https://www.youtube.com/watch?time_continue=1&v=LIb3L4vKZ7U\
        - Linear -- DONE
        - Bitmapped block
        - Pool
        - Segregator

    - Make allocators non-optional, use a polymorphic allocator by default...
    - Allow a CMake flag to use polymorphic or the standard allocator by default
            -- DONE
        - Implement the polymorphic allocator
            -- DONE (mostly)
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

    - Implement more intrusive containers.
        - vector -- DONE
        - deque -- DONE
        - forward_list
        - list
        - set
        - multiset
        - unordered_set
        - unordered_multiset

    - Implement fixed containers
        - Have all the skeletons up
        - vector -- DONE
        - deque -- DONE
        - forward_list -- DONE
        - list -- DONE
        - set
        - multiset
        - map
        - multimap
        - unordered_set
        - unordered_multiset
        - unordered_map
        - unordered_multimap

    - Ordered set unittests
    - Add tests for robin-hood hash into the project

    - Add custom memory allocators
        - https://github.com/mtrebi/memory-allocators
        - Need an alias for the secure memory allocator
    - Add benchmarks compared to STL variants

    - Need to include SQL header files in CMakeLists
    - Need to implement the interpolation search
    - Implement a ranked set maybe?
    - Implement an indexed set maybe?
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
