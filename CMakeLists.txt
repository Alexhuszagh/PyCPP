#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

set(CMAKE_LEGACY_CYGWIN_WIN32 1)
cmake_minimum_required(VERSION 2.8)
project(pycpp CXX)
enable_testing()

# FLAGS
# -----

# MODULES
option(BUILD_CIPHER "Build cipher library" ON)
option(BUILD_COLLECTIONS "Build collections library" ON)
option(BUILD_COMPRESSION "Build compression library" ON)
option(BUILD_FILESYSTEM "Build filesystem library" ON)
option(BUILD_HASHLIB "Build hash library" ON)
option(BUILD_JSON "Build JSON library" ON)
option(BUILD_LATTICE "Build lattice (networking) library" ON)
option(BUILD_MATH "Build math library" ON)
option(BUILD_RE "Build regular expression library" ON)
option(BUILD_STREAM "Build stream library" ON)
option(BUILD_XML "Build XML library" ON)

# WITH MODULE
option(WITH_CIPHER "Build (only) cipher library" OFF)
option(WITH_COLLECTIONS "Build (only) collections library" OFF)
option(WITH_COMPRESSION "Build (only) compression library" OFF)
option(WITH_FILESYSTEM "Build (only) filesystem library" OFF)
option(WITH_HASHLIB "Build (only) hash library" OFF)
option(WITH_JSON "Build (only) JSON library" OFF)
option(WITH_LATTICE "Build (only) lattice library" OFF)
option(WITH_MATH "Build (only) math library" OFF)
option(WITH_RE "Build (only) regular expression library" OFF)
option(WITH_STREAM "Build (only) stream library" OFF)
option(WITH_XML "Build (only) XML library" OFF)

# OPTIONS
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build pycpp library tests" OFF)
SET(PYCPP_NAMESPACE "" CACHE STRING "Name for PyCPP namespace (empty for no namespace).")

if(WITH_CIPHER OR WITH_COLLECTIONS OR WITH_COMPRESSION OR WITH_FILESYSTEM OR WITH_HASHLIB OR WITH_JSON OR WITH_LATTICE OR WITH_MATH OR WITH_RE OR WITH_STREAM OR WITH_XML)
    set(BUILD_CIPHER ${WITH_CIPHER})
    set(BUILD_COLLECTIONS ${WITH_COLLECTIONS})
    set(BUILD_COMPRESSION ${WITH_COMPRESSION})
    set(BUILD_FILESYSTEM ${WITH_FILESYSTEM})
    set(BUILD_HASHLIB ${WITH_HASHLIB})
    set(BUILD_JSON ${WITH_JSON})
    set(BUILD_LATTICE ${WITH_LATTICE})
    set(BUILD_MATH ${WITH_MATH})
    set(BUILD_RE ${WITH_RE})
    set(BUILD_STREAM ${WITH_STREAM})
    set(BUILD_XML ${WITH_XML})
endif()

if (BUILD_CIPHER)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_CIPHER=1")
endif()

if (BUILD_COLLECTIONS)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_COLLECTIONS=1")
endif()

if (BUILD_COMPRESSION)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_COMPRESSION=1")
endif()

if (BUILD_FILESYSTEM)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_FILESYSTEM=1")
endif()

if (BUILD_HASHLIB)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_HASHLIB=1")
endif()

if (BUILD_COMPRESSION)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_COMPRESSION=1")
endif()

if (BUILD_JSON)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_JSON=1")
endif()

if (BUILD_LATTICE)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_LATTICE=1")
endif()

if (BUILD_MATH)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_MATH=1")
endif()

if (BUILD_RE)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_RE=1")
endif()

if (BUILD_STREAM)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_STREAM=1")
endif()

if (BUILD_XML)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DBUILD_XML=1")
endif()

if(BUILD_STATIC)
    if(MINGW OR MSYS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        add_definitions(-fPIC)
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    endif()
endif()

if (BUILD_MATH)
    # libdynd needs C++14 support
    set(CMAKE_CXX_STANDARD 14)
else()
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CXX_STANDARD TRUE)

# Need to manually set compiler flags for Travis CI & Appveyor
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}")
endif()

# INCLUDES
# --------

include(CheckIncludeFile)
enable_language(C)

CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAN_H)
if(HAVE_SYS_MMAN_H)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_SYS_MMAN_H=1")
endif()

# FUNCTIONS
# ---------

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(explicit_bzero HAVE_EXPLICIT_BZERO)
if(HAVE_EXPLICIT_BZERO)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_EXPLICIT_BZERO=1")
endif()

CHECK_FUNCTION_EXISTS(memset_s HAVE_MEMSET_S)
if(HAVE_MEMSET_S)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MEMSET_S=1")
endif()

CHECK_FUNCTION_EXISTS(memcpy_s HAVE_MEMCPY_S)
if(HAVE_MEMCPY_S)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MEMCPY_S=1")
endif()

CHECK_FUNCTION_EXISTS(memmove_s HAVE_MEMMOVE_S)
if(HAVE_MEMMOVE_S)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MEMMOVE_S=1")
endif()

CHECK_FUNCTION_EXISTS(posix_memalign HAVE_POSIX_MEMALIGN)
if(HAVE_POSIX_MEMALIGN)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_POSIX_MEMALIGN=1")
endif()

CHECK_FUNCTION_EXISTS(madvise HAVE_MADVISE)
if(HAVE_MADVISE)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MADVISE=1")
endif()

CHECK_FUNCTION_EXISTS(mlock HAVE_MLOCK)
if(HAVE_MLOCK)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MLOCK=1")
endif()

CHECK_FUNCTION_EXISTS(mmap HAVE_MMAP)
if(HAVE_MMAP)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MMAP=1")
endif()

if(HAVE_SYS_MMAN_H)
    CHECK_FUNCTION_EXISTS(mprotect HAVE_MPROTECT)
    if(HAVE_MPROTECT)
        list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_MPROTECT=1")
    endif()
endif()

# EXTERNAL
# --------

include(ExternalProject)

#
#   Add external project.
#
#   Takes optional N arguments after the name and path
#   to specify additional flags for the build sequence.
#
#   \param name             Name of external project
#   \param path             Path to source directory
#
macro(add_external_project name path)
    set(extra_args ${ARGN})
    # Create external project
    set(${name}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${path})
    set(${name}_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${path})
    ExternalProject_Add(${name}
        SOURCE_DIR "${${name}_SOURCE_DIR}"
        BINARY_DIR "${${name}_BINARY_DIR}"
        CMAKE_ARGS "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
        CMAKE_ARGS "-DCMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}"
        CMAKE_ARGS "-DCMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}"
                   "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
                   "-DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}"
                   "-DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}"
                   "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                   "-DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}"
                   "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
                   "-DCMAKE_AR=${CMAKE_AR}"
                   "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                   "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                   "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}"
                   "-DCMAKE_COMPILER_PREFIX=${CMAKE_COMPILER_PREFIX}"
                   "-DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}"
                   "${extra_args}"
       INSTALL_COMMAND ""
    )

endmacro(add_external_project)

#
#   Add external target to external project.
#
#   \param name             Name of external project
#   \param includedir       Path to include directory
#   \param libdir           Path to library directory
#   \param build_type       Build type {STATIC, SHARED}
#   \param external         Name of the external target
#
macro(add_external_target name includedir libdir build_type external)
    # Configurations
    set(${name}_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${libdir})

    # Create external library
    add_library(${name} ${build_type} IMPORTED)
    set(${name}_LIBRARY "${${name}_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_${build_type}_LIBRARY_PREFIX}${name}${CMAKE_${build_type}_LIBRARY_SUFFIX}")

    # Find paths and set dependencies
    add_dependencies(${name} ${external})
    set(${name}_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${includedir}")

    # Set interface properties
    set_target_properties(${name} PROPERTIES IMPORTED_LOCATION ${${name}_LIBRARY})
    set_target_properties(${name} PROPERTIES INCLUDE_DIRECTORIES ${${name}_INCLUDE_DIR})
endmacro(add_external_target)

# WINDOWS
# -------

# Need to specify proper exception handling for Windows.
if(MSVC)
    list(APPEND PYCPP_COMPILE_OPTIONS "/EHsc")
endif()

if(MSVC)
    list(APPEND PYCPP_LIBRARIES ws2_32.lib crypt32.lib kernel32.lib)
elseif(MINGW OR MSYS)
    list(APPEND PYCPP_LIBRARIES -lws2_32 -lcrypt32 -lkernel32)
endif()


# LIBRARY
# -------

set(PYCPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
set(WARNINGS_INCLUDE_DIRS third_party/warnings/src)
set(MULTI_INDEX_INCLUDE_DIRS third_party/multi_index/include)
set(BRIGAND_INCLUDE_DIRS third_party/multi_index/third_party/brigand/include)

list(APPEND PYCPP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${WARNINGS_INCLUDE_DIRS}
    ${MULTI_INDEX_INCLUDE_DIRS}
    ${BRIGAND_INCLUDE_DIRS}
)

list(APPEND PYCPP_COMPILE_DEFINITIONS "-DPYCPP_NAMESPACE=${PYCPP_NAMESPACE}")

if(BUILD_COMPRESSION)
    # ZLIB
    add_external_project(zlib_external third_party/zlib)
    add_external_target(z third_party/zlib third_party/zlib STATIC zlib_external)
    list(APPEND z_INCLUDE_DIR "${CMAKE_BINARY_DIR}/third_party/zlib")
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_ZLIB=1")
    list(APPEND PYCPP_DEPENDENCIES z)
    list(APPEND PYCPP_LIBRARIES z)
    list(APPEND PYCPP_INCLUDE_DIRS ${z_INCLUDE_DIR})

    # BZIP2
    add_external_project(bzip2_external third_party/bzip2)
    add_external_target(bz2 third_party/bzip2 third_party/bzip2 STATIC bzip2_external)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_BZIP2=1")
    list(APPEND PYCPP_DEPENDENCIES bz2)
    list(APPEND PYCPP_LIBRARIES bz2)
    list(APPEND PYCPP_INCLUDE_DIRS ${bz2_INCLUDE_DIR})

    # XZ
    add_external_project(xz_external third_party/xz)
    add_external_target(lzma third_party/xz/src/liblzma/api third_party/xz/src/liblzma/ STATIC xz_external)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_LZMA=1" "-DLZMA_API_STATIC=1")
    list(APPEND PYCPP_DEPENDENCIES lzma)
    list(APPEND PYCPP_LIBRARIES lzma)
    list(APPEND PYCPP_INCLUDE_DIRS ${lzma_INCLUDE_DIR})

    # BLOSC
    add_external_project(blosc_external third_party/c-blosc
        "-DBUILD_SHARED=OFF"
        "-DBUILD_TESTS=OFF"
        "-DBUILD_BENCHMARKS=OFF"
        "-DBLOSC_INSTALL=OFF"
    )
    add_external_target(blosc third_party/c-blosc/blosc third_party/c-blosc/blosc STATIC blosc_external)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_BLOSC=1")
    list(APPEND PYCPP_DEPENDENCIES blosc)
    list(APPEND PYCPP_LIBRARIES blosc)
    list(APPEND PYCPP_INCLUDE_DIRS ${blosc_INCLUDE_DIR})
endif()

if(BUILD_JSON)
    set(RAPIDJSON_INCLUDE_DIRS third_party/rapidjson/include)
    list(APPEND PYCPP_INCLUDE_DIRS ${RAPIDJSON_INCLUDE_DIRS})
endif()

if(BUILD_MATH)
    add_external_project(dynd_external third_party/libdynd
        "-DDYND_SHARED_LIB=OFF"
        "-DDYND_LLVM=OFF"
        "-DDYND_INSTALL_LIB=OFF"
        "-DDYND_BUILD_TESTS=OFF"
        "-DDYND_BUILD_BENCHMARKS=OFF"
        "-DDYND_BUILD_PLUGINS=OFF"
        "-DDYND_BUILD_DOCS=OFF"
        "-DDYND_COVERAGE=OFF"
    )
    add_external_target(dynd third_party/libdynd/include third_party/libdynd STATIC dynd_external)
    list(APPEND ${dynd_INCLUDE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/third_party/libdynd/include")
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DHAVE_DYND=1")
    list(APPEND PYCPP_DEPENDENCIES dynd)
    list(APPEND PYCPP_LIBRARIES dynd)
    list(APPEND PYCPP_INCLUDE_DIRS ${dynd_INCLUDE_DIR})
endif()

if(BUILD_RE)
    add_external_project(re2_external third_party/re2)
    add_external_target(re2 third_party/re2 third_party/re2 STATIC re2_external)
    list(APPEND PYCPP_DEPENDENCIES re2)
    list(APPEND PYCPP_LIBRARIES re2)
    list(APPEND PYCPP_INCLUDE_DIRS ${re2_INCLUDE_DIR})
endif()

if(BUILD_XML)
    add_external_project(xml2_external third_party/libxml2)
    add_external_target(xml2 third_party/libxml2/include third_party/libxml2 STATIC xml2_external)
    list(APPEND PYCPP_COMPILE_DEFINITIONS "-DLIBXML_STATIC=1")
    list(APPEND PYCPP_DEPENDENCIES xml2)
    list(APPEND PYCPP_LIBRARIES xml2)
    list(APPEND PYCPP_INCLUDE_DIRS ${xml2_INCLUDE_DIR})
endif()

set(SOURCE_FILES
    pycpp/random/pseudorandom.cc
    pycpp/random/sysrandom.cc
    pycpp/safe/stdlib.cc
    pycpp/secure/util.cc
    pycpp/any.cc
    pycpp/base16.cc
    pycpp/base32.cc
    pycpp/base64.cc
    pycpp/byteorder.cc
    pycpp/casemap.cc
    pycpp/codec.cc
    pycpp/getline.cc
    pycpp/gettimeofday.cc
    pycpp/hex.cc
    pycpp/punycode.cc
    pycpp/runtime.cc
    pycpp/stringlib.cc
    pycpp/unicode.cc
    pycpp/url.cc
)

if(BUILD_CIPHER)
    list(APPEND SOURCE_FILES
        pycpp/cipher/aes.cc
    )
endif()

if(BUILD_COLLECTIONS)
endif()

if(BUILD_COMPRESSION)
    list(APPEND SOURCE_FILES
        pycpp/compression/blosc.cc
        pycpp/compression/bzip2.cc
        pycpp/compression/detect.cc
        pycpp/compression/exception.cc
        pycpp/compression/gzip.cc
        pycpp/compression/lzma.cc
        pycpp/compression/stream.cc
    )
endif()

if (BUILD_FILESYSTEM)
    list(APPEND SOURCE_FILES
        pycpp/filesystem/exception.cc
        pycpp/filesystem/filesystem.cc
        pycpp/filesystem/iterator.cc
        pycpp/filesystem/mac.cc
        pycpp/filesystem/nt.cc
        pycpp/filesystem/path.cc
        pycpp/filesystem/posix.cc
        pycpp/filesystem/stat.cc
    )
endif()

if (BUILD_HASHLIB)
    list(APPEND SOURCE_FILES
        pycpp/hashlib/hash.cc
        pycpp/hashlib/md2.cc
        pycpp/hashlib/md4.cc
        pycpp/hashlib/md5.cc
        pycpp/hashlib/sha1.cc
        pycpp/hashlib/sha3.cc
        pycpp/hashlib/sha256.cc
        pycpp/hashlib/sha512.cc
        pycpp/hashlib/whirlpool.cc
    )
endif()

if(BUILD_JSON)
    list(APPEND SOURCE_FILES
        pycpp/json/core.cc
        pycpp/json/dom.cc
        pycpp/json/sax.cc
        pycpp/json/writer.cc
    )
endif()

if(BUILD_LATTICE)
    list(APPEND SOURCE_FILES
        pycpp/lattice/async.cc
        pycpp/lattice/auth.cc
        pycpp/lattice/cookie.cc
        pycpp/lattice/digest.cc
        pycpp/lattice/dns.cc
        pycpp/lattice/header.cc
        pycpp/lattice/multipart.cc
        pycpp/lattice/parameter.cc
        pycpp/lattice/proxy.cc
        pycpp/lattice/redirect.cc
        pycpp/lattice/request.cc
        pycpp/lattice/response.cc
        pycpp/lattice/ssl.cc
        pycpp/lattice/timeout.cc
        pycpp/lattice/url.cc
        pycpp/lattice/util.cc
    )

    if(UNIX)
        list(APPEND SOURCE_FILES pycpp/lattice/adaptor/posix.cc)
    elseif(WIN32)
        list(APPEND SOURCE_FILES pycpp/lattice/adaptor/windows.cc)
    endif()
endif()

if(BUILD_MATH)
    list(APPEND SOURCE_FILES
        pycpp/mathlib/distribution.cc
    )
endif()

if(BUILD_RE)
    list(APPEND SOURCE_FILES
        pycpp/re/match.cc
        pycpp/re/re.cc
    )
endif()

if(BUILD_STREAM)
    list(APPEND SOURCE_FILES
        pycpp/stream/fstream.cc
        pycpp/stream/filter.cc
    )
endif()

if(BUILD_XML)
    list(APPEND SOURCE_FILES
        pycpp/xml/core.cc
        pycpp/xml/dom.cc
        pycpp/xml/sax.cc
        pycpp/xml/text.cc
        pycpp/xml/writer.cc
    )
endif()

#include_directories(${PYCPP_INCLUDE_DIRS})

if(BUILD_STATIC)
    add_library(pycpp STATIC ${SOURCE_FILES})
else()
    add_library(pycpp SHARED ${SOURCE_FILES})
endif()


# INTERFACE
# ---------

target_compile_definitions(pycpp PUBLIC ${PYCPP_COMPILE_DEFINITIONS})
target_compile_options(pycpp PUBLIC ${PYCPP_COMPILE_OPTIONS})
add_dependencies(pycpp ${PYCPP_DEPENDENCIES})
target_include_directories(pycpp PUBLIC ${PYCPP_INCLUDE_DIRS})
target_link_libraries(pycpp PUBLIC ${PYCPP_LIBRARIES})

list(APPEND PYCPP_LIBRARIES pycpp)

# TESTS
# -----

set(TEST_FILES
    test/main.cc
    test/cache/lru.cc
    test/reference/deque.cc
    test/reference/vector.cc
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
    test/xrange.cc
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
    list(APPEND TEST_FILES
        test/compression/blosc.cc
        test/compression/bzip2.cc
        test/compression/detect.cc
        test/compression/gzip.cc
        test/compression/lzma.cc
        test/compression/stream.cc
        test/compression/zlib.cc
    )
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

if(BUILD_LATTICE)
    list(APPEND TEST_FILES
        test/lattice/auth.cc
        test/lattice/cookie.cc
        test/lattice/digest.cc
        test/lattice/parameter.cc
        test/lattice/timeout.cc
        test/lattice/url.cc
    )
endif()

if(BUILD_MATH)
    list(APPEND TEST_FILES
        test/mathlib/average.cc
        test/mathlib/binomial.cc
        test/mathlib/distribution.cc
        test/mathlib/dot.cc
        test/mathlib/factorial.cc
        test/mathlib/std.cc
        test/mathlib/trapz.cc
    )
endif()

if(BUILD_RE)
    list(APPEND TEST_FILES
        test/re/match.cc
        test/re/re.cc
        test/re/regex.cc
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
    if (NOT MINGW)
        find_package(Threads)
    endif()

    if(NOT TARGET gtest)
        add_external_project(googletest_external third_party/googletest)
        add_external_target(gtest third_party/googletest/googletest/include third_party/googletest/googlemock/gtest STATIC googletest_external)
        add_external_target(gtest_main third_party/googletest/googletest/include third_party/googletest/googlemock/gtest STATIC googletest_external)
    endif()
    include_directories(${gtest_INCLUDE_DIR})

    add_executable(pycpp_tests ${TEST_FILES})
    target_link_libraries(pycpp_tests
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
        ${PYCPP_LIBRARIES}
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

    add_dependencies(pycpp_tests ${PYCPP_DEPENDENCIES} gtest gtest_main)
endif()
