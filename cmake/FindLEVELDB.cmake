#  :copyright: (c) 2017 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

# FindLEVELDB
# -----------
#
# Find LEVELDB include dirs and libraries
#
# Use this module by invoking find_package with the form:
#
#   find_package(LEVELDB
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if LEVELDB is not found
#     )
#
# You may also set `LEVELDB_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `LEVELDB_FOUND` will be set to true, and `LEVELDB_LIBRARIES`
# and `LEVELDB_INCLUDE_DIRS` will both be set.
#
# You may optionally set `LEVELDB_ROOT` to specify a custom root directory
# for the LEVELDB installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(LEVELDB_SEARCH_PATHS)

if(LEVELDB_ROOT)
    list(APPEND LEVELDB_SEARCH_PATHS ${LEVELDB_ROOT})
endif()

if(WIN32)
    list(APPEND LEVELDB_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/leveldb"
    )
endif()

unset(LEVELDB_SYSTEM_ROOT)
unset(LEVELDB_CUSTOM_ROOT)
unset(LEVELDB_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(LEVELDB)
foreach(search ${LEVELDB_SEARCH_PATHS})
    FIND_PATH(LEVELDB_INCLUDE_DIR
        NAMES leveldb/db.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT LEVELDB_INCLUDE_DIR)
    FIND_PATH(LEVELDB_INCLUDE_DIR leveldb/db.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(LEVELDB_LIBRARY_NAMES leveldb)

foreach(search ${LEVELDB_SEARCH_PATHS})
    FIND_LIBRARY(LEVELDB_LIBRARY
        NAMES ${LEVELDB_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT LEVELDB_LIBRARY)
    FIND_LIBRARY(LEVELDB_LIBRARY NAMES ${LEVELDB_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(LEVELDB_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIR})
set(LEVELDB_LIBRARIES ${LEVELDB_LIBRARY})

CheckFound(LEVELDB)
FindStaticLibs(LEVELDB)

# VERSION
# -------

if(LEVELDB_FOUND)
    file(STRINGS "${LEVELDB_INCLUDE_DIRS}/leveldb/db.h" LEVELDB_VERSION_CONTENTS REGEX "static const int k[A-Za-z]+Version = [0-9]+;")
    string(REGEX REPLACE ".*kMajorVersion = ([0-9]+).*" "\\1" LEVELDB_VERSION_MAJOR "${LEVELDB_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*kMinorVersion = ([0-9]+).*" "\\1" LEVELDB_VERSION_MINOR "${LEVELDB_VERSION_CONTENTS}")
    set(LEVELDB_VERSION_PATCH 0)
    set(LEVELDB_VERSION_STRING "${LEVELDB_VERSION_MAJOR}.${LEVELDB_VERSION_MINOR}.${LEVELDB_VERSION_PATCH}")
    set(LEVELDB_VERSION ${LEVELDB_VERSION_STRING})

    MatchVersion(LEVELDB)
endif()

# COMPILATION
# -----------

set(LEVELDB_CODE "
#include <leveldb/db.h>
int main(int argc, char *argv[])
{
    int major_version = leveldb::kMajorVersion;
    return 0;
}
"
)

if(LEVELDB_FOUND)
    CheckCompiles(LEVELDB)
endif()
RequiredPackageFound(LEVELDB)