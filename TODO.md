- Need a C++11 variant structure
- Need to add typedefs to any and make it mirror std::any
    - emplace
    - make_any
    - any_cast
    - unittests

- Need to use variant/optional/any only when std version isn't available
    - any -- DONE
    - optional -- DONE
    -- variant

- Filesystem unittests
    - This is a major point of failure: don't slack

- Defaultdict
- namedtuple

- Update AutoCOM so it is using the modern library
    - Need install targets
    - Same for PyCPP
    - Track headers for install targets

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
