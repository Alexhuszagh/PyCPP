//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  The following section uses cryptographic random number generators.
 *  Although std::random_device was designed in the C++ standard library
 *  for this reason, if hardware sources are insufficient, it allows
 *  a possibly deterministic fallback, which is true for compilers
 *  such a MinGW. For these reasons, native APIs and /dev/urandom
 *  are used for truly random bytes.
 */

#include <pycpp/os.h>
#include <pycpp/random.h>
#if defined(OS_WINDOWS)
#   include <windows.h>
#   include <wincrypt.h>
#   include <stdexcept>
#else
#   include <fcntl.h>
#   include <unistd.h>
#endif

#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

// MACROS
// ------

#if defined(OS_LINUX)
#   // Check the kernel version. `getrandom` is only Linux 3.17 and above.
#   include <linux/version.h>
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
#       define PYCPP_HAVE_GETRANDOM
#       include <sys/syscall.h>
#       include <linux/random.h>
#   endif
#endif

#if defined(OS_OPENBSD)
#   define PYCPP_HAVE_GETENTROPY
#endif

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

#if defined(OS_WINDOWS)                     // WINDOWS


 /**
  * Wrapper that acquires the context for the crypt library. If the
  * keyset has not been set, it also initializes that.
  */
bool acquire_context(HCRYPTPROV *ctx)
{
    if (!CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
        return CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT | CRYPT_NEWKEYSET);
    }
    return true;
}


size_t sysrandom(void* dst, size_t dstlen)
{
    HCRYPTPROV ctx;
    if (!acquire_context(&ctx)) {
        throw std::runtime_error("Unable to initialize Win32 crypt library.");
    }

    BYTE* buffer = reinterpret_cast<BYTE*>(dst);
    if(!CryptGenRandom(ctx, dstlen, buffer)) {
        throw std::runtime_error("Unable to generate random bytes.");
    }

    if (!CryptReleaseContext(ctx, 0)) {
        throw std::runtime_error("Unable to release Win32 crypt library.");
    }

    return dstlen;
}

#elif defined(PYCPP_HAVE_GETRANDOM)         // GETRANDOM


/**
 *  Generate cryptograhically random bytes on Linux systems. Use
 *  the blocking `getrandom` syscall.
 */
size_t sysrandom(void* dst, size_t dstlen)
{
    int bytes = syscall(SYS_getrandom, dst, dstlen, 0);
    if (bytes != dstlen) {
        throw std::runtime_error("Unable to read N bytes from CSPRNG.");
    }

    return dstlen;
}

#elif defined(PYCPP_HAVE_GETENTROPY)        // GETENTROPY


/**
 *  Generate cryptograhically random bytes on OpenBSD systems. Use
 *  the `getentropy` syscall, since `/dev/urandom` is not secure
 *  or does not exist.
 */
size_t sysrandom(void* dst, size_t dstlen)
{
    int bytes = getentropy(dst, dstlen);
    if (bytes != dstlen) {
        throw std::runtime_error("Unable to read N bytes from CSPRNG.");
    }

    return dstlen;
}


#else                                       // POSIX


/**
 *  Generate cryptograhically random bytes on POSIX-systems. Open and
 *  read from `/dev/urandom`, **not** `/dev/random`.
 *
 *  [reference] http://www.2uo.de/myths-about-urandom/
 */
size_t sysrandom(void* dst, size_t dstlen)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Unable to open /dev/urandom.");
    }
    if (read(fd, dst, dstlen) != dstlen) {
        close(fd);
        throw std::runtime_error("Unable to read N bytes from CSPRNG.");
    }

    close(fd);
    return dstlen;
}

#endif


std::string sysrandom(size_t length)
{
    char* dst = new char[length];
    sysrandom(dst, length);
    std::string output(dst, length);
    delete[] dst;
    return output;
}

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
