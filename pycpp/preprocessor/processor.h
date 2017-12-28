//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Detect processor for build.
 *
 *  Detect the processor architecture, such as an ARM, SPARC, or Itanium
 *  processor, using preprocessor macros.
 *
 *  Most of these macros can be found from:
 *      https://sourceforge.net/p/predef/wiki/Architectures/
 *      https://people.csail.mit.edu/jaffer/scm/Automatic-C-Preprocessor-Definitions.html
 *
 *  \synopsis
 *      #define PROCESSOR_DETECTED      implementation-defined
 *      #define PROCESSOR_ITANIUM       implementation-defined
 *      #define PROCESSOR_ITANIUM64     implementation-defined
 *      #define PROCESSOR_POWERPC       implementation-defined
 *      #define PROCESSOR_POWERPC32     implementation-defined
 *      #define PROCESSOR_POWERPC64     implementation-defined
 *      #define PROCESSOR_SPARC         implementation-defined
 *      #define PROCESSOR_SPARC64       implementation-defined
 *      #define PROCESSOR_SPARC32       implementation-defined
 *      #define PROCESSOR_X86           implementation-defined
 *      #define PROCESSOR_X8632         implementation-defined
 *      #define PROCESSOR_X8664         implementation-defined
 *      #define PROCESSOR_ALPHA         implementation-defined
 *      #define PROCESSOR_ALPHA64       implementation-defined
 *      #define PROCESSOR_ARM           implementation-defined
 *      #define PROCESSOR_ARM32         implementation-defined
 *      #define PROCESSOR_ARM64         implementation-defined
 *      #define PROCESSOR_BLACKFIN      implementation-defined
 *      #define PROCESSOR_BLACKFIN32    implementation-defined
 *      #define PROCESSOR_CONVEX        implementation-defined
 *      #define PROCESSOR_CONVEX32      implementation-defined
 *      #define PROCESSOR_CONVEX64      implementation-defined
 *      #define PROCESSOR_EPIPHANY      implementation-defined
 *      #define PROCESSOR_EPIPHANY32    implementation-defined
 *      #define PROCESSOR_EPIPHANY64    implementation-defined
 *      #define PROCESSOR_HPPA          implementation-defined
 *      #define PROCESSOR_HPPA32        implementation-defined
 *      #define PROCESSOR_HPPA64        implementation-defined
 *      #define PROCESSOR_68K           implementation-defined
 *      #define PROCESSOR_68K32         implementation-defined
 *      #define PROCESSOR_MIPS          implementation-defined
 *      #define PROCESSOR_MIPS32        implementation-defined
 *      #define PROCESSOR_MIPS64        implementation-defined
 *      #define PROCESSOR_SUPERH        implementation-defined
 *      #define PROCESSOR_SUPERH32      implementation-defined
 *      #define PROCESSOR_SUPERH64      implementation-defined
 *      #define PROCESSOR_SYSTEMZ       implementation-defined
 *      #define PROCESSOR_SYSTEMZ32     implementation-defined
 *      #define PROCESSOR_SYSTEMZ64     implementation-defined
 *      #define PROCESSOR_TAHOE         implementation-defined
 *      #define PROCESSOR_TAHOE32       implementation-defined
 *      #define PROCESSOR_VAX           implementation-defined
 *      #define PROCESSOR_VAX32         implementation-defined
 */

#pragma once

// MACROS
// ------

// ITANIUM
#if !defined (PROCESSOR_DETECTED)
#   if defined(__ia64) || defined(__itanium__) || defined(_M_IA64)
#      define PROCESSOR_ITANIUM
#      define PROCESSOR_ITANIUM64
#      define PROCESSOR_DETECTED
#   endif
#endif

// POWERPC
#if !defined (PROCESSOR_DETECTED)
#   if defined(ppc) || defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
#      define PROCESSOR_POWERPC
#      define PROCESSOR_DETECTED
#       if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(__64BIT__) || defined(_LP64) || defined(__LP64__)
#           define PROCESSOR_POWERPC64
#       else
#           define PROCESSOR_POWERPC32
#       endif
#   endif
#endif

// SPARC
#if !defined (PROCESSOR_DETECTED)
#   if defined(__sparc)
#       define PROCESSOR_SPARC
#       define PROCESSOR_DETECTED
#       if defined(__sparc_v9__) || defined(__sparcv9) || defined(__arch64__)
#          define PROCESSOR_SPARC64
#       else
#          define PROCESSOR_SPARC32
#       endif
#   endif
#endif


// X86
#if !defined (PROCESSOR_DETECTED)
#   if defined(__x86_64__) || defined(_M_X64)
#       define PROCESSOR_X86
#       define PROCESSOR_X8664
#       define PROCESSOR_DETECTED
#   elif defined(__i386) || defined(_M_IX86)
#       define PROCESSOR_X86
#       define PROCESSOR_X8632
#       define PROCESSOR_DETECTED
#   endif
#endif

// DEC ALPHA
#if !defined (PROCESSOR_DETECTED)
#   if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#       define PROCESSOR_ALPHA
#       define PROCESSOR_ALPHA64
#       define PROCESSOR_DETECTED
#   endif
#endif

// ARM
#if !defined (PROCESSOR_DETECTED)
#   if defined(__arm__) || defined(__thumb__) || defined(__arm) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__aarch64__)
#       define PROCESSOR_ARM
#       if defined(__aarch64__)
#          define PROCESSOR_ARM64
#       else
#          define PROCESSOR_ARM32
#       endif
#       define PROCESSOR_DETECTED
#   endif
#endif

// BLACKFIN
#if !defined (PROCESSOR_DETECTED)
#   if defined(__bfin) || defined(__BFIN__)
#       define PROCESSOR_BLACKFIN
#       define PROCESSOR_BLACKFIN32
#       define PROCESSOR_DETECTED
#   endif
#endif

// CONVEX
#if !defined (PROCESSOR_DETECTED)
#   if defined(__bfin) || defined(__BFIN__)
#       define PROCESSOR_CONVEX
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define PROCESSOR_CONVEX32
#       elif SYSTEM_ARCHITECTURE == 64
#          define PROCESSOR_CONVEX64
#       endif
#   endif
#endif

// EPIPHANY
#if !defined (PROCESSOR_DETECTED)
#   if defined(__epiphany__)
#       define PROCESSOR_EPIPHANY
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define PROCESSOR_EPIPHANY32
#       elif SYSTEM_ARCHITECTURE == 64
#          define PROCESSOR_EPIPHANY64
#       endif
#   endif
#endif

// HPPA RISC
#if !defined (PROCESSOR_DETECTED)
#   if defined(__hppa__) || defined(__HPPA__) || defined(__hppa)
#       define PROCESSOR_HPPA
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define PROCESSOR_HPPA32
#       elif SYSTEM_ARCHITECTURE == 64
#          define PROCESSOR_HPPA64
#       endif
#   endif
#endif

// MOTOROLA 68K
#if !defined (PROCESSOR_DETECTED)
#   if defined(__m68k__) || defined(M68000) || defined(__MC68K__)
#       define PROCESSOR_68K
#       define PROCESSOR_68K32
#       define PROCESSOR_DETECTED
#   endif
#endif

// MIPS
#if !defined (PROCESSOR_DETECTED)
#   if defined(__mips__) || defined(mips) || defined(__mips) || defined(__MIPS__)
#       define PROCESSOR_MIPS
#       define PROCESSOR_DETECTED
#       include <architecture.h>
#       if SYSTEM_ARCHITECTURE == 32
#           define PROCESSOR_MIPS32
#       elif SYSTEM_ARCHITECTURE == 64
#          define PROCESSOR_MIPS64
#       endif
#   endif
#endif

// SUPERH
#if !defined (PROCESSOR_DETECTED)
#   if defined(__sh__)
#       define PROCESSOR_SUPERH
#       define PROCESSOR_DETECTED
#       if defined(__SH5__)
#           define PROCESSOR_SUPERH64
#       else
#           define PROCESSOR_SUPERH32
#       endif
#   endif
#endif

// SYSTEMZ
#if !defined (PROCESSOR_DETECTED)
#   if defined(__370__) || defined(__THW_370__) || defined(__s390__) || defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#       define PROCESSOR_SYSTEMZ
#       define PROCESSOR_DETECTED
#       if defined(__s390x__) || defined(__zarch__) || defined(__SYSC_ZARCH__)
#           define PROCESSOR_SYSTEMZ64
#       else
#           define PROCESSOR_SYSTEMZ32
#       endif
#   endif
#endif

// TAHOE
#if !defined(PROCESSOR_DETECTED)
#   if defined(tahoe)
#       define PROCESSOR_TAHOE
#       define PROCESSOR_TAHOE32
#       define PROCESSOR_DETECTED
#   endif
#endif

// VAX
#if !defined(PROCESSOR_DETECTED)
#   if defined(__vax__) || defined(vax) || defined(vaxc) || defined(VAXC) || defined(vax11c) || defined(VAX11)
#       define PROCESSOR_VAX
#       define PROCESSOR_VAX32
#       define PROCESSOR_DETECTED
#   endif
#endif
