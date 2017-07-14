- libxml2 to third_party
    - Change libxml2 to a 3rd party module...

- Mathlib
    - Need array/axis unittests

- filesystem
    - copystat --unittests
    - copy_directory -- unittests
    - move_directory
    - remove_directory
        - Implement in terms of unlink for symlinks
        - Descend into directories first...
    - mkdir -- unitests
    - makedirs -- unittests
    - copylink (for copy_directory, etc.)

- Regular expressions
    - RE2 backbone

- Atomic Operations
    - Check boltsons

- Binary serialization
    - Add cereal...

- Math
    - Need a NumPy-like container
    - Needs to be an N-D array

- Compression support
    - ZLIB
    - LZMA
    - BZ2
