- All the functions that take void* need to take void*&
    - Unittests
        - Low-level interface
            - Casemap
                - Need to implement UTF-8 and UTF-16

            - PunyCode
                - Need to implement UTF-8 and UTF-16

            - Unicode
                - Need to implement UTF-32, UTF-16, UTF-8

    - Need to return void, and let the /src/dst report the progress.
        -PunyCode, then CaseMap, then UniCode
            - Independent -> Dependent
    - punycode.h
    - unicode.h

- I need to test the low-level interfaces
    - Merely having the high-level interfaces use them isn't sufficient... DUH

- Need to finish the compression modules
    - GZIP
        - Needs to be implemented in terms of ZLIB...
            - inflateInit2(&stream, 16+MAX_WBITS);
            - deflateInit2 (I believe...)
    - LZMA
    - BLOSC

- Mathlib
    - Need array/axis unittests

- Atomic Operations
    - Check boltsons

- Binary serialization
    - Add cereal...

- Math
    - Need a NumPy-like container
    - Needs to be an N-D array

