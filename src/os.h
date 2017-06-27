//  :copyright: (c) 2001-2002 John Maddock.
//  :copyright: (c) 2001 Jens Maurer.
//  :copyright: (c) 2008-2013 Rene Rivera.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Detect operating system for build.
 */

#pragma once

// ANDROID
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__ANDROID__) || defined(ANDROID)
#       define OS_ANDROID
#       define OS_LINUX
#       define OS_DETECTED
#   endif
#endif

// LINUX
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__linux__) || defined(linux) || defined(__linux)
#       define OS_LINUX
#       define OS_DETECTED
#   endif
#endif

// MACOS
// -----

#if !defined(OS_DETECTED)
// VERSION
#   if defined(__APPLE__) && defined(__MACH__)
#       define OS_VERSION_MAJOR 10
#       define OS_VERSION_MINOR 0
#       define OS_VERSION_PATCH 0
#   elif defined(__APPLE__) || defined(macintosh)
#       define OS_VERSION_MAJOR 9
#       define OS_VERSION_MINOR 0
#       define OS_VERSION_PATCH 0
#   endif
// DETECTION
#   if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#       define OS_MACOS
#       define OS_DETECTED
#       if defined(__APPLE__) && defined(__MACH__)
#           define OS_DARWIN
#           define OS_BSD
#           define OS_BSD4_4
#       endif
// SUBPLATFORMS
#       include "TargetConditionals.h"
#       if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#           define OS_IOS
#       elif TARGET_OS_MAC
#           define OS_MACOSX
#       endif
#   endif
#endif

// BSD
// ---

#if !defined(OS_DETECTED)
// VERSION
#   if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__bsdi__) || defined(__INTERIX)
#       include <sys/param.h>
#       define OS_BSD
#       define OS_DETECTED
#       if defined(BSD4_4)
#           define OS_BSD4_4
#          define OS_VERSION_MAJOR 4
#          define OS_VERSION_MINOR 4
#          define OS_VERSION_PATCH 0
#       elif defined(BSD4_3)
#           define OS_BSD4_3
#          define OS_VERSION_MAJOR 4
#          define OS_VERSION_MINOR 3
#          define OS_VERSION_PATCH 0
#       elif defined(BSD4_2)
#           define OS_BSD4_2
#          define OS_VERSION_MAJOR 4
#          define OS_VERSION_MINOR 2
#          define OS_VERSION_PATCH 0
#       endif
#   endif
// DETECTION
#   if defined(__FreeBSD__) || defined(__DragonFly__)
#       define OS_FREEBSD
#   elif defined(__NetBSD__)
#       define OS_NETBSD
#   elif defined(__OpenBSD__)
#       define OS_OPENBSD
#   elif defined(__bsdi__)
#       define OS_BSDI
#   elif defined(__INTERIX)
#       define OS_INTERIX
#   endif
#endif

// SOLARIS
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__sun) || defined(sun)
#       define OS_SOLARIS
#       define OS_DETECTED
#   endif
#endif

// IRIX
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(sgi) || defined(__sgi)
#       define OS_IRIX
#       define OS_DETECTED
#   endif
#endif

// HPUX
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__hpux) || defined(hpux) || defined(_hpux)
#       define OS_HPUX
#       define OS_DETECTED
#   endif
#endif

// CYGWIN
// ------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__CYGWIN__)
#       define OS_CYGWIN
#       define OS_DETECTED
#   endif
#endif

// WIN32
// -----

#if !defined(OS_DETECTED)
#   if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(__NT__)
#      include <sdkddkver.h>
#       if _WIN32_WINNT == _WIN32_WINNT_NT4
#       elif _WIN32_WINNT == _WIN32_WINNT_WIN2K             // NT 4.0
#           define OS_VERSION_MAJOR 4
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WINXP             // 2000
#           define OS_VERSION_MAJOR 6
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WS03              // XP
#           define OS_VERSION_MAJOR 6
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WIN6              // Server 2003
#           define OS_VERSION_MAJOR 6
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_VISTA             // VISTA
#           define OS_VERSION_MAJOR 7
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WS08              // VISTA
#           define OS_VERSION_MAJOR 7
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WS08              // VISTA
#           define OS_VERSION_MAJOR 7
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_LONGHORN          // VISTA
#           define OS_VERSION_MAJOR 7
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WIN7              // WINDOWS 7
#           define OS_VERSION_MAJOR 8
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WIN8              // WINDOWS 8
#           define OS_VERSION_MAJOR 9
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WINBLUE           // WINDOWS 8
#           define OS_VERSION_MAJOR 9
#           define OS_VERSION_MINOR 1
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WINTHRESHOLD      // WINDOWS 10
#           define OS_VERSION_MAJOR 10
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       elif _WIN32_WINNT == _WIN32_WINNT_WIN10             // WINDOWS 10
#           define OS_VERSION_MAJOR 10
#           define OS_VERSION_MINOR 0
#           define OS_VERSION_PATCH 0
#       endif
#   endif
// DETECTION
#   if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(__NT__)
#       define OS_WINDOWS
#       define OS_DETECTED
#   endif
#endif

// WINDOWS CE
// ----------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_WIN32_WCE)
#       define OS_WINDOWS_CE
#       define OS_DETECTED
#   endif
#endif

// HAIKU
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__HAIKU__)
#       define OS_HAIKU
#       define OS_DETECTED
#   endif
#endif

// BEOS
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__BEOS__)
#       define OS_BEOS
#       define OS_DETECTED
#   endif
#endif

// AIX
// ---

#if !defined(OS_DETECTED)
// VERSION
#   if defined(_AIX43)
#       define OS_VERSION_MAJOR 4
#       define OS_VERSION_MINOR 3
#       define OS_VERSION_PATCH 0
#   elif defined(_AIX41)
#       define OS_VERSION_MAJOR 4
#       define OS_VERSION_MINOR 1
#       define OS_VERSION_PATCH 0
#   elif defined(_AIX32)
#       define OS_VERSION_MAJOR 3
#       define OS_VERSION_MINOR 2
#       define OS_VERSION_PATCH 0
#   elif defined(_AIX3)
#       define OS_VERSION_MAJOR 3
#       define OS_VERSION_MINOR 0
#       define OS_VERSION_PATCH 0
#   endif
// DETECTION
#   if defined(_AIX) || defined(__TOS_AIX__) || defined(__IBMCPP__)
#       define OS_AIX
#       define OS_DETECTED
#   endif
#endif

// AMIGAOS
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(AMIGA) || defined(__amigaos__)
#       define OS_AMIGAOS
#       define OS_DETECTED
#   endif
#endif

// QNX
// ---

#if !defined(OS_DETECTED)
// VERSION
#   if defined(__QNX__)
#       define OS_VERSION_MAJOR 4
#       define OS_VERSION_MINOR 0
#       define OS_VERSION_PATCH 0
#   endif
// DETECTION
#   if defined(__QNX__) || defined(__QNXNTO__)
#       define OS_QNX
#       define OS_DETECTED
#   endif
#endif

// VXWORKS
// -------

#if !defined(OS_DETECTED)
// DETECTION && VERSION
#   include <version.h>
#   if defined(__VXWORKS__) || defined(__vxworks) || defined(__RTP__) || defined(_WRS_KERNEL)
#       define OS_VXWORKS
#       define OS_DETECTED
#       define OS_VERSION_MAJOR _WRS_VXWORKS_MAJOR
#       define OS_VERSION_MINOR _WRS_VXWORKS_MINOR
#       define OS_VERSION_PATCH _WRS_VXWORKS_MAINT
#   endif
#endif

// SYMBIAN
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__SYMBIAN32__)
#       define OS_SYMBIAN
#       define OS_DETECTED
#   endif
#endif

// CRAY
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_CRAYC)
#       define OS_CRAY
#       define OS_DETECTED
#   endif
#endif

// VMS
// ---

#if !defined(OS_DETECTED)
// VERSION
#   if defined(__VMS_VER)
#       define OS_VERSION_MAJOR __VMS_VER / 10000000
#       define OS_VERSION_MINOR (__VMS_VER - OS_VERSION_MAJOR * 10000000) / 100000
#       define OS_VERSION_PATCH 0
#   endif
// DETECTION
#   if defined(__VMS)
#       define OS_VMS
#       define OS_DETECTED
#   endif
#endif

// ULTRIX
// ------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__ultrix) || defined(ultrix)
#       define OS_ULTRIX
#       define OS_DETECTED
#   endif
#endif

// RELIANT
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(sinix)
#       define OS_RELIANT
#       define OS_DETECTED
#   endif
#endif

// NATIVE CLIENT
// -------------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__native_client__)
#       define OS_NACL
#       define OS_DETECTED
#   endif
#endif

// DYNIX
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_SEQUENT_) || defined(sequent)
#       define OS_DYNIX
#       define OS_DETECTED
#   endif
#endif

// ECOS
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__ECOS)
#       define OS_ECOS
#       define OS_DETECTED
#   endif
#endif

// EMX
// ---

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__EMX__)
#       define OS_EMX
#       define OS_DETECTED
#   endif
#endif

// OSF
// ---

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__osf__)
#       define OS_OSF
#       define OS_DETECTED
#   endif
#endif

// DGUX
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(DGUX) || defined(__DGUX__) || defined(__dgux__)
#       define OS_DGUX
#       define OS_DETECTED
#   endif
#endif

// INTEGRITY
// ---------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__INTEGRITY)
#       define OS_INTEGRITY
#       define OS_DETECTED
#   endif
#endif

// LYNX
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__Lynx__)
#       define OS_LYNX
#       define OS_DETECTED
#   endif
#endif

// SCO
// ---

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_SCO_DS) || defined(M_XENIX) || defined(M_I386)
#       define OS_SCO
#       define OS_DETECTED
#   endif
#endif

// HURD
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__GNU__) || defined(__gnu_hurd__)
#       define OS_HURD
#       define OS_DETECTED
#   endif
#endif

// UNIXWARE
// --------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__USLC__) || defined(_UNIXWARE7) || defined(sco)
#       define OS_UNIXWARE
#       define OS_DETECTED
#   endif
#endif

// AMDAHL
// ------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(UTS)
#       define OS_AMDAHL
#       define OS_DETECTED
#   endif
#endif

// APOLLO AEGIS
// ------------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(aegis)
#       define OS_AEGIS
#       define OS_DETECTED
#   endif
#endif

// APOLLO DOMAIN
// -------------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(apollo)
#       define OS_APOLLO
#       define OS_DETECTED
#   endif
#endif

// BLUE GENE
// ---------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__bg__) || defined(__bgq__) || defined(__THW_BLUEGENE__) || defined(__TOS_BGQ__)
#       define OS_BLUEGENE
#       define OS_DETECTED
#   endif
#endif

// MINIX
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__convex__)
#       define OS_CONVEX
#       define OS_DETECTED
#   endif
#endif

// MINIX
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__minix)
#       define OS_MINIX
#       define OS_DETECTED
#   endif
#endif

// MORPHOS
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__MORPHOS__)
#       define OS_MORPHOS
#       define OS_DETECTED
#   endif
#endif

// MPE/IX
// ------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(mpeix) || defined(__mpexl)
#       define OS_MPEIX
#       define OS_DETECTED
#   endif
#endif

// MICROWARE
// ---------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__OS9000) || defined(_OSK)
#       define OS_MICROWARE
#       define OS_DETECTED
#   endif
#endif

// MSDOS
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#       define OS_MSDOS
#       define OS_DETECTED
#   endif
#endif

// NONSTOP
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__TANDEM)
#       define OS_NONSTOP
#       define OS_DETECTED
#   endif
#endif

// NUCLEUS
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__nucleus__)
#       define OS_NUCLEUS
#       define OS_DETECTED
#   endif
#endif

// OS2
// ---

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(OS2) || defined(_OS2) || defined(__OS2__) || defined(__TOS_OS2__)
#       define OS_OS2
#       define OS_DETECTED
#   endif
#endif

// OS400
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__OS400__)
#       define OS_OS400
#       define OS_DETECTED
#   endif
#endif

// PALM OS
// -------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__palmos__)
#       define OS_PALM
#       define OS_DETECTED
#   endif
#endif

// PLAN9
// -----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(EPLAN9)
#       define OS_PLAN9
#       define OS_DETECTED
#   endif
#endif

// PYRAMID DC/OSx
// --------------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(pyr)
#       define OS_PYRAMID
#       define OS_DETECTED
#   endif
#endif

// STARTUS VOS
// -----------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__VOS__)
#       define OS_VOS
#       define OS_DETECTED
#   endif
#endif

// SVR4
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__sysv__) || defined(__SVR4) || defined(__svr4__) || defined(_SYSTYPE_SVR4)
#       define OS_SVR4
#       define OS_DETECTED
#   endif
#endif

// SYLLABLE
// --------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__SYLLABLE__)
#       define OS_SYLLABLE
#       define OS_DETECTED
#   endif
#endif

// UNICOS
// ------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_UNICOS)
#       define OS_UNICOS
#       define OS_DETECTED
#   endif
#endif

// UNICOS/mp
// ---------

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(_CRAY) || defined(__crayx1)
#       define OS_UNICOSMP
#       define OS_DETECTED
#   endif
#endif

// WIND/U
// ------

#if !defined(OS_DETECTED)
// DETECTION && VERSION
#   if defined(_WINDU_SOURCE)
#       define OS_WINDU
#       define OS_DETECTED
#       define OS_VERSION_MAJOR (_WINDU_SOURCE & 0xff0000) >> 16
#       define OS_VERSION_MINOR (_WINDU_SOURCE & 0xff00) >> 8
#       define OS_VERSION_PATCH (_WINDU_SOURCE & 0xff)
#   endif
#endif

// Z/OS
// ----

#if !defined(OS_DETECTED)
// DETECTION && VERSION
#   if defined(__MVS__) || defined(__HOS_MVS__) || defined(__TOS_MVS__)
#       define OS_ZOS
#       define OS_DETECTED
#   endif
#endif

// UNIX
// ----

#if !defined(OS_DETECTED)
// DETECTION
#   if defined(__unix__) || defined(__unix)
#       define OS_UNIX
#       define OS_DETECTED
#   endif
#endif

// POSIX
// -----

// Check if we have a POSIX-like system.
#if defined(OS_DETECTED)
#   if defined(OS_LINUX) || defined(OS_MACOS) || defined(OS_BSD) || \
        defined(OS_SOLARIS) || defined(OS_IRIX) || defined(OS_HPUX) || \
        defined(OS_CYGWIN) || defined(OS_AIX) || defined(OS_QNX) || \
        defined(OS_VMS) || defined(OS_ULTRIX) || defined(OS_RELIANT) || \
        defined(OS_DYNIX) || defined(OS_EMX) || defined(OS_OSF) || \
        defined(OS_DGUX) || defined(OS_LYNX) || defined(OS_SCO) || \
        defined(OS_HURD) || defined(OS_UNIXWARE) || defined(AMDAHL) || \
        defined(OS_AEGIS) || defined(OS_APOLLO) || defined(OS_MINIX) || \
        defined(OS_MPEIX) || defined(OS_VOS) || defined(OS_SVR4) || \
        defined(OS_UNICOS) || defined(OS_UNICOSMP) || defined(OS_ZOS) || \
        defined(OS_UNIX)
#       define OS_POSIX
#   endif
#endif
