# Roadmap

**Short-Term**

    - Implement a sorted vector
        - unittests
    - Add tests for robin-hood hash into the project
    
    - Add the btree-cpp from Google to the project
        - https://github.com/algorithm-ninja/cpp-btree
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
