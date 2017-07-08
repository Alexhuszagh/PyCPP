#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

set(CMAKE_LEGACY_CYGWIN_WIN32 1)
cmake_minimum_required(VERSION 2.8)
project(pycpp CXX)
enable_testing()

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD TRUE)

# MODULES
option(BUILD_CIPHER "Build cipher library" ON)
option(BUILD_COLLECTIONS "Build collections library" ON)
option(BUILD_COMPRESSION "Build compression library" ON)
option(BUILD_FILESYSTEM "Build filesystem library" ON)
option(BUILD_HASHLIB "Build hash library" ON)
option(BUILD_JSON "Build JSON library" ON)
option(BUILD_MATH "Build math library" ON)
option(BUILD_STREAM "Build stream library" ON)
option(BUILD_XML "Build XML library" ON)

# WITH MODULE
option(WITH_CIPHER "Build (only) cipher library" OFF)
option(WITH_COLLECTIONS "Build (only) collections library" OFF)
option(WITH_COMPRESSION "Build (only) compression library" OFF)
option(WITH_FILESYSTEM "Build (only) filesystem library" OFF)
option(WITH_HASHLIB "Build (only) hash library" OFF)
option(WITH_JSON "Build (only) JSON library" OFF)
option(WITH_MATH "Build (only) math library" OFF)
option(WITH_STREAM "Build (only) stream library" OFF)
option(WITH_XML "Build (only) XML library" OFF)

# OPTIONS
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build pycpp library tests" OFF)

if(WITH_CIPHER OR WITH_COLLECTIONS OR WITH_COMPRESSION OR WITH_FILESYSTEM OR WITH_HASHLIB OR WITH_JSON OR WITH_MATH OR WITH_STREAM OR WITH_XML)
    set(BUILD_CIPHER ${WITH_CIPHER})
    set(BUILD_COLLECTIONS ${WITH_COLLECTIONS})
    set(BUILD_COMPRESSION ${WITH_COMPRESSION})
    set(BUILD_FILESYSTEM ${WITH_FILESYSTEM})
    set(BUILD_HASHLIB ${WITH_HASHLIB})
    set(BUILD_JSON ${WITH_JSON})
    set(BUILD_MATH ${WITH_MATH})
    set(BUILD_STREAM ${WITH_STREAM})
    set(BUILD_XML ${WITH_XML})
endif()

if(BUILD_STATIC)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        add_definitions(-fPIC)
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    endif()
endif()

# Need to manually set compiler flags for Travis CI & Appveyor
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# INCLUDES
# --------

include(CheckIncludeFile)
enable_language(C)

CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAN_H)
if(HAVE_SYS_MMAN_H)
    add_definitions(-DHAVE_SYS_MMAN_H=1)
endif()

# FUNCTIONS
# ---------

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(explicit_bzero HAVE_EXPLICIT_BZERO)
if(HAVE_EXPLICIT_BZERO)
    add_definitions(-DHAVE_EXPLICIT_BZERO=1)
endif()

CHECK_FUNCTION_EXISTS(memset_s HAVE_MEMSET_S)
if(HAVE_MEMSET_S)
    add_definitions(-DHAVE_MEMSET_S=1)
endif()

CHECK_FUNCTION_EXISTS(memcpy_s HAVE_MEMCPY_S)
if(HAVE_MEMCPY_S)
    add_definitions(-DHAVE_MEMCPY_S=1)
endif()

CHECK_FUNCTION_EXISTS(memmove_s HAVE_MEMMOVE_S)
if(HAVE_MEMMOVE_S)
    add_definitions(-DHAVE_MEMMOVE_S=1)
endif()

CHECK_FUNCTION_EXISTS(posix_memalign HAVE_POSIX_MEMALIGN)
if(HAVE_POSIX_MEMALIGN)
    add_definitions(-DHAVE_POSIX_MEMALIGN=1)
endif()

CHECK_FUNCTION_EXISTS(madvise HAVE_MADVISE)
if(HAVE_MADVISE)
    add_definitions(-DHAVE_MADVISE=1)
endif()

CHECK_FUNCTION_EXISTS(mlock HAVE_MLOCK)
if(HAVE_MLOCK)
    add_definitions(-DHAVE_MLOCK=1)
endif()

CHECK_FUNCTION_EXISTS(mmap HAVE_MMAP)
if(HAVE_MMAP)
    add_definitions(-DHAVE_MMAP=1)
endif()

if(HAVE_SYS_MMAN_H)
    CHECK_FUNCTION_EXISTS(mprotect HAVE_MPROTECT)
    if(HAVE_MPROTECT)
        add_definitions(-DHAVE_MPROTECT=1)
    endif()
endif()

# LIBRARY
# -------

include_directories(src warnings/src)

if(BUILD_JSON)
    include_directories(third_party/rapidjson/include)
endif()

if(BUILD_XML)
    include_directories(third_party/libxml2/include)
    include_directories(
        third_party/multi_index/include
        third_party/multi_index/third_party/brigand/include
    )
    add_subdirectory(third_party/libxml2)
endif()

set(SOURCE_FILES
    src/random/pseudorandom.cc
    src/random/sysrandom.cc
    src/safe/stdlib.cc
    src/secure/util.cc
    src/any.cc
    src/base16.cc
    src/base32.cc
    src/base64.cc
    src/byteorder.cc
    src/casemap.cc
    src/codec.cc
    src/getline.cc
    src/gettimeofday.cc
    src/hex.cc
    src/punycode.cc
    src/stringlib.cc
    src/unicode.cc
    src/url.cc
)

if(BUILD_CIPHER)
    list(APPEND SOURCE_FILES
        src/cipher/aes.cc
    )
endif()

if(BUILD_COLLECTIONS)
endif()

if(BUILD_COMPRESSION)
endif()

if (BUILD_FILESYSTEM)
    list(APPEND SOURCE_FILES
        src/filesystem/exception.cc
        src/filesystem/mac.cc
        src/filesystem/nt.cc
        src/filesystem/path.cc
        src/filesystem/posix.cc
        src/filesystem/stat.cc
    )
endif()

if (BUILD_HASHLIB)
    list(APPEND SOURCE_FILES
        src/hashlib/crc32.cc
        src/hashlib/hash.cc
        src/hashlib/md2.cc
        src/hashlib/md4.cc
        src/hashlib/md5.cc
        src/hashlib/sha1.cc
        src/hashlib/sha3.cc
        src/hashlib/sha256.cc
        src/hashlib/sha512.cc
        src/hashlib/whirlpool.cc
    )
endif()

if(BUILD_JSON)
    list(APPEND SOURCE_FILES
        src/json/core.cc
        src/json/dom.cc
        src/json/sax.cc
        src/json/writer.cc
    )
endif()

if(BUILD_MATH)
endif()

if(BUILD_STREAM)
    list(APPEND SOURCE_FILES
        src/stream/fstream.cc
        src/stream/filter.cc
    )
endif()

if(BUILD_XML)
    list(APPEND SOURCE_FILES
        src/xml/core.cc
        src/xml/dom.cc
        src/xml/sax.cc
        src/xml/writer.cc
    )
endif()

if(BUILD_STATIC)
    add_library(pycpp STATIC ${SOURCE_FILES})
else()
    add_library(pycpp SHARED ${SOURCE_FILES})
endif()
set(CROSSCXX_LIBRARIES pycpp)

# Need to specify proper exception handling for Windows.
if(MSVC)
    set_target_properties(pycpp PROPERTIES
        COMPILE_OPTIONS "/EHsc"
    )
endif()

if(MSVC)
    list(APPEND CROSSCXX_LIBRARIES ws2_32.lib crypt32.lib kernel32.lib)
elseif(MINGW OR MSYS)
    list(APPEND CROSSCXX_LIBRARIES -lws2_32 -lcrypt32 -lkernel32)
endif()

# TESTS
# -----

set(TEST_FILES
    test/main.cc
    test/safe/stdlib.cc
    test/secure/allocator.cc
    test/secure/string.cc
    test/secure/util.cc
    test/view/string.cc
    test/view/vector.cc
    test/any.cc
    test/architecture.cc
    test/base16.cc
    test/base32.cc
    test/base64.cc
    test/byteorder.cc
    test/casemap.cc
    test/codec.cc
    test/compiler.cc
    test/enum.cc
    test/getline.cc
    test/gettimeofday.cc
    test/hex.cc
    test/iterator.cc
    test/itertools.cc
    test/processor.cc
    test/optional.cc
    test/ordering.cc
    test/os.cc
    test/pimpl.cc
    test/punycode.cc
    test/random.cc
    test/stringlib.cc
    test/tls.cc
    test/unicode.cc
    test/url.cc
)

if (BUILD_CIPHER)
    list(APPEND TEST_FILES test/cipher.cc)
endif()

if(BUILD_COLLECTIONS)
    list(APPEND TEST_FILES
        test/collections/ordereddict.cc
    )
endif()

if(BUILD_COMPRESSION)
endif()

if (BUILD_FILESYSTEM)
    list(APPEND TEST_FILES test/filesystem.cc)
endif()

if (BUILD_HASHLIB)
    list(APPEND TEST_FILES test/hashlib.cc)
endif()

if (BUILD_JSON)
    list(APPEND TEST_FILES
        test/json/dom.cc
        test/json/sax.cc
        test/json/writer.cc
    )
endif()

if(BUILD_MATH)
    list(APPEND TEST_FILES
        test/mathlib/array.cc
        test/mathlib/axis.cc
    )
endif()

if (BUILD_STREAM)
    list(APPEND TEST_FILES
        test/stream/fstream.cc
        test/stream/filter.cc
    )
endif()

if (BUILD_XML)
    list(APPEND TEST_FILES
        test/xml/dom.cc
        test/xml/sax.cc
    )
endif()


if(BUILD_TESTS)
    if(NOT TARGET gtest)
        add_subdirectory(googletest)
    endif()
    include_directories(googletest/googletest/include)

    add_executable(pycpp_tests ${TEST_FILES})
    target_link_libraries(pycpp_tests
        gtest
        gtest_main
        ${CROSSCXX_LIBRARIES}
    )
    # Need to specify proper exception handling for Windows.
    if(MSVC)
        set_target_properties(pycpp_tests PROPERTIES
            COMPILE_OPTIONS "/EHsc"
        )
    endif()

    add_test(NAME pycpp_tests
        COMMAND pycpp_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
