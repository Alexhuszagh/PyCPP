- Regular expressions
    - RE2 backbone
    - Add RE2 to project.

- TODO: there's an issue in my RE2 implementation...
    ~bz2_compressor_impl is getting called **almost** immediately...

- BZIP2 buffer needs to grow exponentially (2-fold).
    - Don't use a constant buffer size... seriously.
    - https://stackoverflow.com/a/19849403/4131059

- Need to make the streambuf's buffer increase maybe???
    - buffer_size

- Mathlib
    - Need array/axis unittests

- filesystem
    - move_* implements need to be **durable** across filesystems.

- Atomic Operations
    - Check boltsons

- Binary serialization
    - Add cereal...

- Math
    - Need a NumPy-like container
    - Needs to be an N-D array

- Compression support
    - Detection
    - Compressors/decompressors
    - ZLIB
    - LZMA
    - BZ2

