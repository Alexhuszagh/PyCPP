- All the functions that take void* need to take void*&
    - Need to return void, and let the /src/dst report the progress.
        -PunyCode, then CaseMap, then UniCode
            - Independent -> Dependent
    - punycode.h
    - unicode.h

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

