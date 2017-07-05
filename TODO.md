- Bug fixes
    - Clang on macOS with JSON DOM is producing an infinite loop
    - Why? Where...

- filesystem
    - copystat
    - copy_directory
    - move_directory
    - remove_directory
    - mkdir -- unitests
    - makedirs -- unittests

- Random
    - Need to clone Python's random API

- IDNA
    - Add IDNA support

- Stringlib
    - unittests

- Atomic Operations
    - Check boltsons

- JSON support
    - Add writer...
        - Clone the RapidJSON example
            - https://github.com/miloyip/rapidjson/blob/master/example/simplewriter/simplewriter.cpp

- Binary serialization
    - Add cereal...

- Math
    - Need a NumPy-like container
    - Needs to be an N-D array

- Compression support
    - ZLIB
    - LZMA
    - BZ2
