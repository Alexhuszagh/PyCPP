# Roadmap

**Short-Term**

    - Write a threshold counter and collections counter.
    - counter
        - Fix most_common
        - Write most_common unittests

    - threshold_counter
        - Fix most_common
        - Write most_common unittests

    - Implement an ordered_multi_map
    - Check if C++ has a priority_queue
    - iterutils.chunked?
    - iterutils.windowed?
    - iterutils.remap?
    - iterutils.backoff?
    - fileutils?
    - ioutils?

**Mid-Term**

- Need an API around a key/value store.

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
