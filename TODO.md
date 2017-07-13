- Mathlib
    - Need array/axis unittests

- filesystem
    - copystat --unittests
    - copy_directory
        - CreateDirectoryExW
        - 
    - move_directory
    - remove_directory
    - mkdir -- unitests
    - makedirs -- unittests
    - iterator
        - On MinGW, we don't seem to be entering the loop...
        - Need to skip ".", ".." probably
        - Need to have a directory "deque" for the recursive directory iterator


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
