# Roadmap

**Mid-Term**

- Need an API around a key/value store.
- Need sequential, memory-mapped, random-access files.
    - Use fd_fstream APIs to wrap them.
    - Implementation
        - Memory-mapped
    - Unittests
        - sequential_fstream
        - sequential_ifstream
        - sequential_ofstream
        - random_access_fstream
        - random_access_ifstream
        - random_access_ofstream

- Update AutoCOM so it is using the modern library
    - Need install targets
    - Same for PyCPP
    - Track headers for install targets

**Long-Term**

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
