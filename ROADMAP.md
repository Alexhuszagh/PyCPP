# Roadmap

**Short-Term**

    - Bug on Windows with the compressed streams....
        - CARRIAGE RETURN....

    - Specialized `is_relocatable` for a lot of types.
        - Basically everything can be specialized....
        - csv
        - filesystem
        - lattice
        - random
        - stream
        - string
        - windows
        - xml

    - Get rid of naked new/delete
        - Change to use the global allocator generally speaking...
        - `allocate_and_construct` for custom allocators...
        - Don't store the allocator in JSON and XML values (or any values)
            - **HOW** (Pass the allocator always??)

    - Add noexcepts everywhere
        - CSV
        - Filesystem
        - Lattice
        - Random
        - Stream
        - String
        - XML

    - Make moves always non-throwing
        - Check if copies can be non-throwing too
        - Add a lot of noexcepts in my code....

    - Interprocess
        - Some sort of interprocess library like Boost would be great

    - Look at Howard Hinnant's Date library:
        - https://github.com/HowardHinnant/date/blob/master/include/date/date.h
        - ~7k lines of code
        - See if I can adapt to a datetime interface for Python

    - Look at Bloom filters
        - https://github.com/mavam/libbf

    - Look at Cuckoo filters
        - https://github.com/efficient/libcuckoo

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
        - Heap allocator
            - Linear and preallocated allocators on the heap (which can grow)
            - This is great when requesting large quantities of small data...

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
