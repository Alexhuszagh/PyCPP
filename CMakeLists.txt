#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

set(CMAKE_LEGACY_CYGWIN_WIN32 1)
cmake_minimum_required(VERSION 2.8)
project(pycpp CXX)
enable_testing()

if(MSVC AND BUILD_SHARED_LIBS)
    cmake_minimum_required(VERSION 3.4)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# FLAGS
# -----

# MODULES
option(BUILD_CIPHER "Build cipher library" ON)
option(BUILD_COLLECTIONS "Build collections library" ON)
option(BUILD_COMPRESSION "Build compression library" ON)
option(BUILD_CSV "Build CSV library" ON)
option(BUILD_FILESYSTEM "Build filesystem library" ON)
option(BUILD_HASHLIB "Build hash library" ON)
option(BUILD_JSON "Build JSON library" ON)
option(BUILD_KEYVALUE "Build key-value database library" ON)
option(BUILD_LATTICE "Build lattice (networking) library" ON)
option(BUILD_MATH "Build math library" ON)
option(BUILD_RE "Build regular expression library" ON)
option(BUILD_SQLITE "Build SQLite database library" ON)
option(BUILD_STREAM "Build stream library" ON)
option(BUILD_XML "Build XML library" ON)

# DEPENDENCIES
option(USE_SYSTEM_BLOSC "Use system BLOSC installation" OFF)
option(USE_SYSTEM_BZIP2 "Use system BZIP2 installation" OFF)
option(USE_SYSTEM_LEVELDB "Use system LEVELDB installation" OFF)
option(USE_SYSTEM_LIBXML2 "Use system LIBXML2 installation" OFF)
option(USE_SYSTEM_LZMA "Use system LZMA2/XZZ installation" OFF)
option(USE_SYSTEM_RE2 "Use system RE2 installation" OFF)
option(USE_SYSTEM_ZLIB "Use system ZLIB installation" OFF)

# WITH MODULE
option(WITH_CORE "Build (only) core library" OFF)
option(WITH_CIPHER "Build (only) cipher library" OFF)
option(WITH_COLLECTIONS "Build (only) collections library" OFF)
option(WITH_COMPRESSION "Build (only) compression library" OFF)
option(WITH_CSV "Build (only) CSV library" OFF)
option(WITH_FILESYSTEM "Build (only) filesystem library" OFF)
option(WITH_HASHLIB "Build (only) hash library" OFF)
option(WITH_JSON "Build (only) JSON library" OFF)
option(WITH_KEYVALUE "Build (only) key-value database library" OFF)
option(WITH_LATTICE "Build (only) lattice library" OFF)
option(WITH_MATH "Build (only) math library" OFF)
option(WITH_RE "Build (only) regular expression library" OFF)
option(WITH_SQLITE "Build (only) SQLite database library" OFF)
option(WITH_STREAM "Build (only) stream library" OFF)
option(WITH_XML "Build (only) XML library" OFF)

# OPTIONS
option(BUILD_TESTS "Build pycpp library tests" OFF)
SET(CXX_STANDARD "11" CACHE STRING "Minimum C++ standard to support.")
SET(PYCPP_NAMESPACE "" CACHE STRING "Name for PyCPP namespace (empty for no namespace).")

if(WITH_CORE OR WITH_CIPHER OR WITH_COLLECTIONS OR WITH_COMPRESSION OR WITH_CSV OR WITH_FILESYSTEM OR WITH_HASHLIB OR WITH_JSON OR WITH_KEYVALUE OR WITH_LATTICE OR WITH_MATH OR WITH_RE OR WITH_SQLITE OR WITH_STREAM OR WITH_XML)
    set(BUILD_CIPHER ${WITH_CIPHER})
    set(BUILD_COLLECTIONS ${WITH_COLLECTIONS})
    set(BUILD_COMPRESSION ${WITH_COMPRESSION})
    set(BUILD_CSV ${WITH_CSV})
    set(BUILD_FILESYSTEM ${WITH_FILESYSTEM})
    set(BUILD_HASHLIB ${WITH_HASHLIB})
    set(BUILD_JSON ${WITH_JSON})
    set(BUILD_KEYVALUE ${WITH_KEYVALUE})
    set(BUILD_LATTICE ${WITH_LATTICE})
    set(BUILD_MATH ${WITH_MATH})
    set(BUILD_RE ${WITH_RE})
    set(BUILD_SQLITE ${WITH_SQLITE})
    set(BUILD_STREAM ${WITH_STREAM})
    set(BUILD_XML ${WITH_XML})
endif()

set(BUILD_CACHE ${BUILD_KEYVALUE})

if (BUILD_MATH OR MSVC)
    if (CXX_STANDARD LESS "14")
        # libdynd needs C++14 support
        set(CXX_STANDARD 14)
    endif()
endif()
set(CMAKE_CXX_STANDARD ${CXX_STANDARD})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++${CXX_STANDARD}")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++${CXX_STANDARD}")
elseif(MSVC)
    list(APPEND PYCPP_COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX -wd4018 /std:c++${CXX_STANDARD}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
endif()

if(NOT BUILD_SHARED_LIBS)
    if(MINGW OR MSYS OR CYGWIN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
endif()

if(WIN32)
    list(APPEND PYCPP_COMPILE_DEFINITIONS UNICODE _UNICODE)
endif()

# INCLUDES
# --------

include(CheckIncludeFile)
enable_language(C)

CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAN_H)

# FUNCTIONS
# ---------

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(explicit_bzero HAVE_EXPLICIT_BZERO)
CHECK_FUNCTION_EXISTS(memset_s HAVE_MEMSET_S)
CHECK_FUNCTION_EXISTS(memcpy_s HAVE_MEMCPY_S)
CHECK_FUNCTION_EXISTS(memmove_s HAVE_MEMMOVE_S)
CHECK_FUNCTION_EXISTS(posix_memalign HAVE_POSIX_MEMALIGN)
CHECK_FUNCTION_EXISTS(posix_fallocate HAVE_POSIX_FALLOCATE)
CHECK_FUNCTION_EXISTS(posix_fadvise HAVE_POSIX_FADVISE)
CHECK_FUNCTION_EXISTS(madvise HAVE_MADVISE)
CHECK_FUNCTION_EXISTS(mlock HAVE_MLOCK)
CHECK_FUNCTION_EXISTS(mmap HAVE_MMAP)
CHECK_FUNCTION_EXISTS(mmap HAVE_MPROTECT)
CHECK_FUNCTION_EXISTS(_wfopen HAVE_WFOPEN)

# CONFIGURATION
# -------------

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/pycpp/config.h"
)

# EXTERNAL
# --------

include(ExternalProject)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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
                   ${extra_args}
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
elseif(MINGW OR MSYS OR CYGWIN)
    list(APPEND PYCPP_LIBRARIES -lws2_32 -lcrypt32 -lkernel32)
endif()

# LIBRARY
# -------

set(PYCPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/include")
set(WARNINGS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/warnings/src")
set(MULTI_INDEX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/multi_index/include")
set(BRIGAND_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/multi_index/third_party/brigand/include")

list(APPEND PYCPP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${WARNINGS_INCLUDE_DIRS}
    ${MULTI_INDEX_INCLUDE_DIRS}
    ${BRIGAND_INCLUDE_DIRS}
)

if(BUILD_CACHE)
    if(BUILD_KEYVALUE)
        if(USE_SYSTEM_LEVELDB)
            find_package(LEVELDB "1.0")
        else()
            find_package(LEVELDB "1.0")     # TODO: remove
# TODO: restore
#            add_external_project(zlib_external third_party/zlib)
#            add_external_target(z third_party/zlib third_party/zlib STATIC zlib_external)
#            list(APPEND PYCPP_DEPENDENCIES z)
#            set(Z_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib")
#            set(LEVELDB_INCLUDE_DIRS ${Z_INCLUDE_DIR})
#            set(LEVELDB_LIBRARIES z)
#            set(LEVELDB_FOUND 1)
        endif()

        if(LEVELDB_FOUND)
            list(APPEND PYCPP_LIBRARIES ${LEVELDB_LIBRARIES})
            list(APPEND PYCPP_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIRS})
        else()
            message(WARNING "Cannot find back-end for key-value database. Disabling...")
            set(BUILD_KEYVALUE OFF)
        endif()
    endif()
endif()

if(BUILD_COMPRESSION)
    # ZLIB
    if(USE_SYSTEM_ZLIB)
        find_package(ZLIB "1.0")
    else()
        add_external_project(zlib_external third_party/zlib)
        add_external_target(z third_party/zlib third_party/zlib STATIC zlib_external)
        list(APPEND PYCPP_DEPENDENCIES z)
        set(Z_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib")
        set(ZLIB_INCLUDE_DIRS ${Z_INCLUDE_DIR})
        set(ZLIB_LIBRARIES z)
        set(ZLIB_FOUND 1)
    endif()

    if(ZLIB_FOUND)
        list(APPEND PYCPP_COMPILE_DEFINITIONS HAVE_ZLIB=1)
        list(APPEND PYCPP_LIBRARIES ${ZLIB_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    endif()

    # BZIP2
    if(USE_SYSTEM_BZIP2)
        find_package(BZIP2 "1.0")
    else()
        add_external_project(bzip2_external third_party/bzip2)
        add_external_target(bz2 third_party/bzip2 third_party/bzip2 STATIC bzip2_external)
        list(APPEND PYCPP_DEPENDENCIES bz2)
        set(BZIP2_INCLUDE_DIRS ${bz2_INCLUDE_DIR})
        set(BZIP2_LIBRARIES bz2)
        set(BZIP2_FOUND 1)
    endif()

    if(BZIP2_FOUND)
        list(APPEND PYCPP_COMPILE_DEFINITIONS HAVE_BZIP2=1)
        list(APPEND PYCPP_LIBRARIES ${BZIP2_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${BZIP2_INCLUDE_DIRS})
    endif()

    # XZ
    if(USE_SYSTEM_LZMA)
        find_package(LZMA "5.0")
    else()
        add_external_project(xz_external third_party/xz)
        add_external_target(lzma third_party/xz/src/liblzma/api third_party/xz/src/liblzma/ STATIC xz_external)
        list(APPEND PYCPP_DEPENDENCIES lzma)
        set(LZMA_INCLUDE_DIRS ${lzma_INCLUDE_DIR})
        set(LZMA_LIBRARIES lzma)
        set(LZMA_FOUND 1)
    endif()

    if(LZMA_FOUND)
        list(APPEND PYCPP_COMPILE_DEFINITIONS HAVE_LZMA=1 LZMA_API_STATIC=1)
        list(APPEND PYCPP_LIBRARIES ${LZMA_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${LZMA_INCLUDE_DIRS})
    endif()

    # BLOSC
    list(APPEND BLOSC_FLAGS
        "-DBUILD_SHARED=OFF"
        "-DBUILD_TESTS=OFF"
        "-DBUILD_BENCHMARKS=OFF"
        "-DBLOSC_INSTALL=OFF"
    )
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        if (WIN32)
            list(APPEND BLOSC_FLAGS "-DCMAKE_C_FLAGS=-Wno-implicit-function-declaration -Wno-attributes")
        else()
            list(APPEND BLOSC_FLAGS "-DCMAKE_C_FLAGS=-Wno-implicit-function-declaration")
        endif()
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        list(APPEND BLOSC_FLAGS "-DCMAKE_C_FLAGS=-Wno-implicit-function-declaration -Wno-shift-negative-value")
    elseif(MSVC)
        list(APPEND BLOSC_FLAGS "-DCMAKE_CXX_FLAGS=-wd4101 -wd4146 -wd4244 -wd4530 -wd4577 -wd4996")
    endif()

    if(USE_SYSTEM_BLOSC)
        find_package(BLOSC "1.0")
    else()
        add_external_project(blosc_external third_party/c-blosc ${BLOSC_FLAGS})
        add_external_target(blosc third_party/c-blosc/blosc third_party/c-blosc/blosc STATIC blosc_external)
        list(APPEND PYCPP_DEPENDENCIES blosc)
        set(BLOSC_INCLUDE_DIRS ${blosc_INCLUDE_DIR})
        set(BLOSC_LIBRARIES blosc)
        set(BLOSC_FOUND 1)
    endif()

    if(BLOSC_FOUND)
        list(APPEND PYCPP_COMPILE_DEFINITIONS HAVE_BLOSC=1)
        list(APPEND PYCPP_LIBRARIES ${BLOSC_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${BLOSC_INCLUDE_DIRS})
    endif()
endif()

if(BUILD_JSON)
    set(RAPIDJSON_INCLUDE_DIRS third_party/rapidjson/include)
    list(APPEND PYCPP_INCLUDE_DIRS ${RAPIDJSON_INCLUDE_DIRS})
endif()

if(BUILD_MATH)
# TODO: temporarily disabled libdynd, restore
#    add_external_project(dynd_external third_party/libdynd
#        "-DDYND_SHARED_LIB=OFF"
#        "-DDYND_LLVM=OFF"
#        "-DDYND_INSTALL_LIB=OFF"
#        "-DDYND_BUILD_TESTS=OFF"
#        "-DDYND_BUILD_BENCHMARKS=OFF"
#        "-DDYND_BUILD_PLUGINS=OFF"
#        "-DDYND_BUILD_DOCS=OFF"
#        "-DDYND_COVERAGE=OFF"
#    )
#    # Force lib prefix due to libdynd behavior
#    set(OLD_CMAKE_STATIC_LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
#    set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
#    add_external_target(dynd third_party/libdynd/include third_party/libdynd STATIC dynd_external)
#    add_external_target(dyndt third_party/libdynd/include third_party/libdynd STATIC dynd_external)
#    set(CMAKE_STATIC_LIBRARY_PREFIX ${OLD_CMAKE_STATIC_LIBRARY_PREFIX})
#
#    # Include and set files
#    list(APPEND ${dynd_INCLUDE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/third_party/libdynd/include")
#    list(APPEND PYCPP_COMPILE_DEFINITIONS HAVE_DYND=1)
#    list(APPEND PYCPP_DEPENDENCIES dynd dyndt)
#    list(APPEND PYCPP_LIBRARIES dynd dyndt)
#    list(APPEND PYCPP_INCLUDE_DIRS ${dynd_INCLUDE_DIR})
endif()

if(BUILD_RE)
    if(USE_SYSTEM_RE2)
        find_package(RE2)
    else()
        add_external_project(re2_external third_party/re2)
        add_external_target(re2 third_party/re2 third_party/re2 STATIC re2_external)
        list(APPEND PYCPP_DEPENDENCIES re2)
        set(RE2_INCLUDE_DIRS ${re2_INCLUDE_DIR})
        set(RE2_LIBRARIES re2)
        set(RE2_FOUND 1)
    endif()

    if(RE2_FOUND)
        list(APPEND PYCPP_LIBRARIES ${RE2_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${RE2_INCLUDE_DIRS})
    endif()
endif()

if(BUILD_XML)
    if(USE_SYSTEM_LIBXML2)
        find_package(LIBXML2)
    else()
        add_external_project(xml2_external third_party/libxml2)
        add_external_target(xml2 third_party/libxml2/include third_party/libxml2 STATIC xml2_external)
        list(APPEND PYCPP_DEPENDENCIES xml2)
        set(LIBXML2_INCLUDE_DIRS ${xml2_INCLUDE_DIR})
        set(LIBXML2_LIBRARIES xml2)
        set(LIBXML2_FOUND 1)
    endif()

    if(LIBXML2_FOUND)
        list(APPEND PYCPP_COMPILE_DEFINITIONS LIBXML_STATIC=1)
        list(APPEND PYCPP_LIBRARIES ${LIBXML2_LIBRARIES})
        list(APPEND PYCPP_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIRS})
    endif()
endif()

set(HEADER_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/include/pycpp/config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/iterator.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/iterator/category.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/iterator/input_iterator_facade.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/iterator/range.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/iterator/transform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/itertools.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/itertools/product.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/itertools/sampling.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/atof.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/atoi.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/bool.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/char.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/enum.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/float.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/ftoa.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/int.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/itoa.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/null.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/misc/enum.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/misc/ordering.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/misc/pimpl.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/misc/xrange.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/composite_key.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/container.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/global_fun.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/hashed_index_fwd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/hashed_index.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/identity.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/indexed_by.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/mem_fun.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/member.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/ordered_index_fwd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/ordered_index.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/random_access_index_fwd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/random_access_index.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/ranked_index_fwd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/ranked_index.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/sequenced_index_fwd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/sequenced_index.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/multi_index/tag.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/architecture.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/byteorder.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/compiler.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/errno.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/os.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/parallel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/processor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/sysstat.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/tls.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/random.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/reference/core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/reference/deque.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/reference/vector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/runtime/os.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/safe.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/safe/stdlib.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure/allocator.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure/char_traits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure/stdlib.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure/string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base16.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base32.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base64.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/casemap.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/codec.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/getline.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/hex.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/punycode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/unicode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/url.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/whitespace.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/view/string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/view/vector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/windows/error.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/windows/mman.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/windows/winapi.h"
)

set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/atof.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/atoi.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/bool.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/char.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/enum.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/float.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/ftoa.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/int.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/itoa.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lexical/null.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/preprocessor/byteorder.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/random/pseudorandom.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/random/sysrandom.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/runtime/os.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/safe/stdlib.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/secure/stdlib.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base16.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base32.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/base64.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/casemap.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/codec.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/getline.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/hex.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/punycode.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/string.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/unicode.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/string/url.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/windows/error.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/windows/mman.cc"
)

if(BUILD_CACHE)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cache/lru.h"
    )
    if(BUILD_KEYVALUE)
        list(APPEND HEADER_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cache/kv_backend.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cache/kv.h"
        )
        list(APPEND SOURCE_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cache/kv_backend.cc"
        )
    endif()
    if(BUILD_SQLITE)
        list(APPEND HEADER_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cache/sqlite.h"
        )
    endif()
endif()

if(BUILD_CIPHER)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cipher.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cipher/aes.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/cipher/aes.cc"
    )
endif()

if(BUILD_COLLECTIONS)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/any.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/defaultdict.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/optional.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/ordereddict.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/utility.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/variant.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/any.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/collections/variant.cc"
    )
endif()

if(BUILD_COMPRESSION)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/blosc.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/bzip2.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/core.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/detect.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/exception.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/gzip.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/lzma.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/zlib.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/blosc.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/bzip2.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/detect.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/exception.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/gzip.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/lzma.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/zlib.cc"
    )

    if(BUILD_STREAM)
        list(APPEND HEADER_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/stream.h"
        )
        list(APPEND SOURCE_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/compression/stream.cc"
        )
    endif()
endif()

if (BUILD_CSV)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/dict.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/punct.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/reader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/writer.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/dict.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/punct.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/reader.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/csv/writer.cc"
    )
endif()

if (BUILD_FILESYSTEM)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/access.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/exception.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/fd.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/home.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/iterator.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/path.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/stat.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/tmp.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/exception.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/filesystem.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/home.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/iterator.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/mac.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/nt.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/path.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/posix.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/stat.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/filesystem/tmp.cc"
    )
endif()

if (BUILD_HASHLIB)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/hash.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/md2.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/md4.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/md5.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/sha1.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/sha3.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/sha256.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/sha512.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/hashlib/whirlpool.cc"
    )
endif()

if(BUILD_JSON)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/core.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/dom.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/sax.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/writer.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/core.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/dom.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/sax.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/json/writer.cc"
    )
endif()

if(BUILD_LATTICE)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/nossl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/openssl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/async.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/auth.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/connection.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/cookie.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/crypto.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/digest.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/dns.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/header.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/method.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/multipart.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/parameter.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/proxy.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/redirect.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/request.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/response.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/ssl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/timeout.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/transfer.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/url.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/util.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/async.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/auth.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/cookie.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/digest.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/dns.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/header.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/multipart.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/parameter.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/proxy.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/redirect.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/request.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/response.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/ssl.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/timeout.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/url.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/util.cc"
    )

    if(UNIX)
        list(APPEND HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/posix.h")
        list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/posix.cc")
    elseif(WIN32)
        list(APPEND HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/windows.h")
        list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/lattice/adaptor/windows.cc")
    endif()
endif()

if(BUILD_MATH)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/array.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/average.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/binomial.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/distribution.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/dot.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/factorial.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/std.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/trapz.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/math/distribution.cc"
    )
endif()

if(BUILD_RE)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re/match.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re/re.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re/regex.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re/match.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/re/re.cc"
        # Don't include regex.cc, it's included by re.cc
    )
endif()

if(BUILD_STREAM)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/filter.h"
    )
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/filter.cc")
    if(BUILD_FILESYSTEM)
        list(APPEND HEADER_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/fd.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/fstream.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/mmap.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/random_access.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/sequential.h"
        )
        list(APPEND SOURCE_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/fd.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/fstream.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/mmap.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/random_access.cc"
            "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/stream/sequential.cc"
        )
    endif()
endif()

if(BUILD_XML)
    list(APPEND HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/core.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/dom.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/sax.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/text.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/writer.h"
    )
    list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/core.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/dom.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/sax.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/text.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/pycpp/xml/writer.cc"
    )
endif()

add_library(pycpp "")
target_sources(pycpp
    PRIVATE ${SOURCE_FILES}
    PUBLIC ${HEADER_FILES}
)

# INTERFACE
# ---------

if(PYCPP_COMPILE_DEFINITIONS)
    target_compile_definitions(pycpp PUBLIC ${PYCPP_COMPILE_DEFINITIONS})
endif()

if(PYCPP_COMPILE_OPTIONS)
    target_compile_options(pycpp PUBLIC ${PYCPP_COMPILE_OPTIONS})
endif()

if(PYCPP_DEPENDENCIES)
    add_dependencies(pycpp ${PYCPP_DEPENDENCIES})
endif()

if(PYCPP_INCLUDE_DIRS)
    target_include_directories(pycpp PUBLIC ${PYCPP_INCLUDE_DIRS})
endif()

if(PYCPP_LIBRARIES)
    target_link_libraries(pycpp PUBLIC ${PYCPP_LIBRARIES})
endif()

list(APPEND PYCPP_LIBRARIES pycpp)

# TESTS
# -----

set(TEST_FILES
    test/main.cc
    test/cache/lri.cc
    test/cache/lru.cc
#    test/lexical/atof.cc
    test/lexical/atoi.cc
#    test/lexical/bool.cc
#    test/lexical/char.cc
#    test/lexical/enum.cc
#    test/lexical/float.cc
#    test/lexical/ftoa.cc
#    test/lexical/int.cc
#    test/lexical/itoa.cc
#    test/lexical/null.cc
    test/misc/enum.cc
    test/misc/ordering.cc
    test/misc/pimpl.cc
    test/misc/xrange.cc
    test/preprocessor/architecture.cc
    test/preprocessor/byteorder.cc
    test/preprocessor/compiler.cc
    test/preprocessor/parallel.cc
    test/preprocessor/processor.cc
    test/preprocessor/os.cc
    test/preprocessor/tls.cc
    test/reference/deque.cc
    test/reference/vector.cc
    test/runtime/os.cc
    test/safe/stdlib.cc
    test/secure/allocator.cc
    test/secure/string.cc
    test/secure/stdlib.cc
    test/sfinae/has_bit_and.cc
    test/sfinae/has_bit_and_assign.cc
    test/sfinae/has_bit_or.cc
    test/sfinae/has_bit_or_assign.cc
    test/sfinae/has_bit_xor.cc
    test/sfinae/has_bit_xor_assign.cc
    test/sfinae/has_divides.cc
    test/sfinae/has_divides_assign.cc
    test/sfinae/has_equal_to.cc
    test/sfinae/has_greater.cc
    test/sfinae/has_greater_equal.cc
    test/sfinae/has_left_shift.cc
    test/sfinae/has_left_shift_assign.cc
    test/sfinae/has_less.cc
    test/sfinae/has_less_equal.cc
    test/sfinae/has_logical_and.cc
    test/sfinae/has_logical_or.cc
    test/sfinae/has_member_function.cc
    test/sfinae/has_member_type.cc
    test/sfinae/has_member_variable.cc
    test/sfinae/has_minus.cc
    test/sfinae/has_minus_assign.cc
    test/sfinae/has_modulus.cc
    test/sfinae/has_modulus_assign.cc
    test/sfinae/has_multiplies.cc
    test/sfinae/has_multiplies_assign.cc
    test/sfinae/has_not_equal_to.cc
    test/sfinae/has_plus.cc
    test/sfinae/has_plus_assign.cc
    test/sfinae/has_right_shift.cc
    test/sfinae/has_right_shift_assign.cc
    test/sfinae/has_subscript.cc
    test/sfinae/is_complex.cc
    test/sfinae/is_specialization.cc
    test/sfinae/is_pair.cc
    test/sfinae/is_reference_wrapper.cc
    test/sfinae/is_string.cc
    test/string/base16.cc
    test/string/base32.cc
    test/string/base64.cc
    test/string/casemap.cc
    test/string/codec.cc
    test/string/getline.cc
    test/string/hex.cc
    test/string/punycode.cc
    test/string/string.cc
    test/string/unicode.cc
    test/string/url.cc
    test/view/string.cc
    test/view/vector.cc
    test/iterator.cc
    test/itertools.cc
    test/random.cc
)

if (BUILD_CACHE)
    if(BUILD_KEYVALUE)
        list(APPEND TEST_FILES
            test/cache/kv.cc
        )
    endif()
endif()

if (BUILD_CIPHER)
    list(APPEND TEST_FILES test/cipher.cc)
endif()

if(BUILD_COLLECTIONS)
    list(APPEND TEST_FILES
        test/collections/any.cc
        test/collections/counter.cc
        test/collections/defaultdict.cc
        test/collections/optional.cc
        test/collections/ordereddict.cc
        test/collections/threshold_counter.cc
        test/collections/variant.cc
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

if (BUILD_CSV)
    list(APPEND TEST_FILES test/csv.cc)
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
        test/math/average.cc
        test/math/binomial.cc
        test/math/distribution.cc
        test/math/dot.cc
        test/math/factorial.cc
        test/math/std.cc
        test/math/trapz.cc
    )
endif()

if(BUILD_RE)
    list(APPEND TEST_FILES
        test/re/match.cc
        test/re/re.cc
        test/re/regex.cc
    )
endif()

if(BUILD_SQLITE)
    list(APPEND TEST_FILES
        test/cache/sqlite.cc
    )
endif()

if (BUILD_STREAM)
    list(APPEND TEST_FILES test/stream/filter.cc)
    if(BUILD_FILESYSTEM)
        list(APPEND TEST_FILES
            test/stream/fd.cc
            test/stream/fstream.cc
            test/stream/mmap.cc
            test/stream/random_access.cc
            test/stream/sequential.cc
        )
    endif()
endif()

if (BUILD_XML)
    list(APPEND TEST_FILES
        test/xml/dom.cc
        test/xml/sax.cc
        test/xml/writer.cc
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

# INSTALL
# -------

if(MSVC)
    if(CMAKE_CL_64)
        set(MSVC_FOLDER_PREFIX x64)
    else()
        set(MSVC_FOLDER_PREFIX Win32)
    endif()

    install(TARGETS pycpp
        LIBRARY DESTINATION "lib/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
        ARCHIVE DESTINATION "lib/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
        RUNTIME DESTINATION "bin/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
    )
else()
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )
endif()

install(DIRECTORY PYCPP_NAMESPACE
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)