//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Generic hash structure.
 */

#include <pycpp/hashlib.h>
#include <pycpp/secure/stdlib.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/string/hex.h>
#include <assert.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


void hash_update(void* ctx, const void* src, size_t srclen, void (*cb)(void*, const void*, size_t)) noexcept
{
    size_t length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        size_t shift = length > 512 ? 512 : length;
        cb(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void hash_digest(void* ctx, void*& dst, size_t dstlen, size_t hashlen, void (*cb)(void*, void*))
{
    if (dstlen < hashlen) {
        throw runtime_error("dstlen not large enough to store hash digest.");
    }

    cb(ctx, dst);
}


secure_string hash_digest(void* ctx, size_t hashlen, void (*cb)(void*, void*))
{
    void* dst = secure_malloc(hashlen);
    void* dst_first = dst;

    try {
        hash_digest(ctx, dst_first, hashlen, hashlen, cb);
        secure_string output((char*)dst, hashlen);

        secure_zero(dst, hashlen);
        secure_free(dst);
        return output;

    } catch (...) {
        secure_free(dst);
        throw;
    }
}


void hash_hexdigest(void* ctx, void*& dst, size_t dstlen, size_t hashlen, void (*cb)(void*, void*))
{
    if (dstlen < 2 * hashlen) {
        throw runtime_error("dstlen not large enough to store hash hexdigest.");
    }

    void* hash = secure_malloc(hashlen);
    void* hash_dst = hash;
    const void* hash_src = hash;

    try {
        hash_digest(ctx, hash_dst, hashlen, hashlen, cb);
        hex_i8(hash_src, hashlen, dst, dstlen);
        secure_free(hash);
    } catch (...) {
        secure_free(hash);
        throw;
    }
}


secure_string hash_hexdigest(void* ctx, size_t hashlen, void (*cb)(void*, void*))
{
    void* dst = secure_malloc(hashlen * 2);
    void* dst_first = dst;

    try {
        hash_hexdigest(ctx, dst_first, hashlen * 2, hashlen, cb);
        secure_string output((char*)dst, hashlen * 2);

        secure_zero(dst, hashlen * 2);
        secure_free(dst);
        return output;

    } catch (...) {
        secure_free(dst);
        throw;
    }
}


/**
 *  \brief Allocate storage for hash.
 */
struct allocate_hash
{
    template <typename Hash>
    void operator()(Hash& hash)
    {
        new (&hash) Hash();
    }
};


/**
 *  \brief Deallocate storage for hash.
 */
struct deallocate_hash
{
    template <typename Hash>
    void operator()(Hash& hash) noexcept
    {
        hash.~Hash();
    }
};


/**
 *  \brief Update hash from storage.
 */
struct update_hash
{
    string_wrapper str;

    template <typename Hash>
    void operator()(Hash& hash) noexcept
    {
        hash.update(str);
    }
};


/**
 *  \brief Get digest via C-string.
 */
struct digest_cstring
{
    void* dst;
    size_t dstlen;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        hash.digest(dst, dstlen);
    }
};


/**
 *  \brief Get hex digest via C-string.
 */
struct hexdigest_cstring
{
    void* dst;
    size_t dstlen;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        hash.hexdigest(dst, dstlen);
    }
};


/**
 *  \brief Get digest via STL.
 */
struct digest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.digest();
    }
};


/**
 *  \brief Get hex digest via STL.
 */
struct hexdigest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.hexdigest();
    }
};


template <typename T, bool IsConst>
using conditional_const_t = conditional_t<
    IsConst,
    const T,
    T
>;


template <typename T, typename Memory>
using hash_type = conditional_const_t<T, is_const<Memory>::value>;


/**
 *  \brief Cast storage to the correct type.
 */
template <typename Memory, typename Function>
static void get_hash(Memory& mem, hash_algorithm algorithm, Function& function)
{
    switch (algorithm) {
        case none_hash_algorithm:
            return;

        case md2_hash_algorithm:
            function(reinterpret_cast<hash_type<md2_hash, Memory>&>(mem));
            break;

        case md4_hash_algorithm:
            function(reinterpret_cast<hash_type<md4_hash, Memory>&>(mem));
            break;

        case md5_hash_algorithm:
            function(reinterpret_cast<hash_type<md5_hash, Memory>&>(mem));
            break;

        case sha1_hash_algorithm:
            function(reinterpret_cast<hash_type<sha1_hash, Memory>&>(mem));
            break;

        case sha2_224_hash_algorithm:
            function(reinterpret_cast<hash_type<sha2_224_hash, Memory>&>(mem));
            break;

        case sha2_256_hash_algorithm:
            function(reinterpret_cast<hash_type<sha2_256_hash, Memory>&>(mem));
            break;

        case sha2_384_hash_algorithm:
            function(reinterpret_cast<hash_type<sha2_384_hash, Memory>&>(mem));
            break;

        case sha2_512_hash_algorithm:
            function(reinterpret_cast<hash_type<sha2_512_hash, Memory>&>(mem));
            break;

        case sha3_224_hash_algorithm:
            function(reinterpret_cast<hash_type<sha3_224_hash, Memory>&>(mem));
            break;

        case sha3_256_hash_algorithm:
            function(reinterpret_cast<hash_type<sha3_256_hash, Memory>&>(mem));
            break;

        case sha3_384_hash_algorithm:
            function(reinterpret_cast<hash_type<sha3_384_hash, Memory>&>(mem));
            break;

        case sha3_512_hash_algorithm:
            function(reinterpret_cast<hash_type<sha3_512_hash, Memory>&>(mem));
            break;

        case whirlpool_hash_algorithm:
            function(reinterpret_cast<hash_type<whirlpool_hash, Memory>&>(mem));
            break;

        default:
            assert(false && "Unrecognized hashing algorithm.");
    }
}

// OBJECTS
// -------


cryptographic_hash::cryptographic_hash(hash_algorithm algorithm):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
}


cryptographic_hash::cryptographic_hash(hash_algorithm algorithm, const void* src, size_t srclen):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
    update(src, srclen);
}


cryptographic_hash::cryptographic_hash(hash_algorithm algorithm, const string_wrapper& str):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
    update(str);
}


cryptographic_hash::~cryptographic_hash() noexcept
{
    deallocate_hash hasher;
    get_hash(mem, algorithm, hasher);
}


cryptographic_hash::cryptographic_hash(cryptographic_hash&& rhs) noexcept
{
    memcpy((void*) &mem, (const void*) &rhs.mem, sizeof(memory_type));
    algorithm = rhs.algorithm;
    rhs.algorithm = none_hash_algorithm;
}


cryptographic_hash& cryptographic_hash::operator=(cryptographic_hash&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


void cryptographic_hash::update(const void* src, size_t srclen) noexcept
{
    update(string_wrapper(reinterpret_cast<const char*>(src), srclen));
}


void cryptographic_hash::update(const string_wrapper& str) noexcept
{
    update_hash functor = {str};
    get_hash(mem, algorithm, functor);
}


void cryptographic_hash::digest(void*& dst, size_t dstlen) const
{
    digest_cstring functor = {dst, dstlen};
    get_hash(mem, algorithm, functor);
}


void cryptographic_hash::hexdigest(void*& dst, size_t dstlen) const
{
    hexdigest_cstring functor = {dst, dstlen};
    get_hash(mem, algorithm, functor);
}


secure_string cryptographic_hash::digest() const
{
    digest_stl functor;
    get_hash(mem, algorithm, functor);
    return move(functor.str);
}


secure_string cryptographic_hash::hexdigest() const
{
    hexdigest_stl functor;
    get_hash(mem, algorithm, functor);
    return move(functor.str);
}


void cryptographic_hash::swap(cryptographic_hash& rhs) noexcept
{
    // swap buffers
    memory_type buffer;
    memcpy((void*) &buffer, (const void*) &rhs.mem, sizeof(memory_type));
    memcpy((void*) &rhs.mem, (const void*) &mem, sizeof(memory_type));
    memcpy((void*) &mem, (const void*) &buffer, sizeof(memory_type));

    using PYCPP_NAMESPACE::swap;
    swap(algorithm, rhs.algorithm);
}

PYCPP_END_NAMESPACE
