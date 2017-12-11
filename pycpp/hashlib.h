//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash functions.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/secure/string.h>
#include <pycpp/string/string.h>
#include <functional>
#include <string>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct md2_context;
struct md4_context;
struct md5_context;
struct sha1_context;
struct sha3_context;
struct sha2_256_context;
struct sha2_512_context;
struct whirlpool_context;

// ENUMS
// -----

/**
 *  \brief Enumerated algorithms for hashlib.
 */
enum hash_algorithm
{
    md2_hash_algorithm = 0,
    md4_hash_algorithm,
    md5_hash_algorithm,
    sha1_hash_algorithm,
    sha2_224_hash_algorithm,
    sha2_256_hash_algorithm,
    sha2_384_hash_algorithm,
    sha2_512_hash_algorithm,
    sha3_224_hash_algorithm,
    sha3_256_hash_algorithm,
    sha3_384_hash_algorithm,
    sha3_512_hash_algorithm,
    whirlpool_hash_algorithm,
};


// MACROS
// ------


/**
 *  \brief Macro to declare a specialized hash context.
 *
 *  \param          Name of the hash object.
 *  \cx             Name of the context object.
 */
#define SPECIALIZED_HASH(name, cx)                                      \
    struct name##_hash                                                  \
    {                                                                   \
    public:                                                             \
        name##_hash();                                                  \
        name##_hash(const void* src, size_t srclen);                    \
        name##_hash(const string_wrapper& str);                         \
        ~name##_hash();                                                 \
                                                                        \
        void update(const void* src, size_t srclen);                    \
        void update(const string_wrapper& str);                         \
        void digest(void*& dst, size_t dstlen) const;                   \
        void hexdigest(void*& dst, size_t dstlen) const;                \
        secure_string digest() const;                                   \
        secure_string hexdigest() const;                                \
                                                                        \
    private:                                                            \
        cx##_context* ctx;                                              \
    }

// FUNCTIONS
// ---------

/**
 *  \brief Update hash from buffer.
 */
void hash_update(void* ctx, const void* src, size_t srclen, void (*cb)(void*, const void*, size_t));

/**
 *  \brief Get digest from context.
 */
void hash_digest(void* ctx, void*& dst, size_t dstlen, size_t hashlen, void (*cb)(void*, void*));

/**
 *  \brief Get digest from context.
 */
secure_string hash_digest(void* ctx, size_t hashlen, void (*cb)(void*, void*));

/**
 *  \brief Get hexdigest from context.
 */
void hash_hexdigest(void* ctx, void*& dst, size_t dstlen, size_t hashlen, void (*cb)(void*, void*));

/**
 *  \brief Get hexdigest from context.
 */
secure_string hash_hexdigest(void* ctx, size_t hashlen, void (*cb)(void*, void*));


// OBJECTS
// -------


/**
 *  \brief Generic hash context.
 */
struct cryptographic_hash
{
public:
    cryptographic_hash(hash_algorithm algorithm);
    cryptographic_hash(hash_algorithm algorithm, const void* src, size_t srclen);
    cryptographic_hash(hash_algorithm algorithm, const string_wrapper& str);
    ~cryptographic_hash();

    cryptographic_hash(const cryptographic_hash&);
    cryptographic_hash& operator=(const cryptographic_hash&);
    cryptographic_hash(cryptographic_hash&&);
    cryptographic_hash& operator=(cryptographic_hash&&);

    void update(const void* src, size_t srclen);
    void update(const string_wrapper& str);
    void digest(void*& dst, size_t dstlen) const;
    void hexdigest(void*& dst, size_t dstlen) const;
    secure_string digest() const;
    secure_string hexdigest() const;

private:
    typedef typename std::aligned_storage<sizeof(uintptr_t)>::type memory_type;
    hash_algorithm algorithm;
    memory_type mem;
};


SPECIALIZED_HASH(md2, md2);
SPECIALIZED_HASH(md4, md4);
SPECIALIZED_HASH(md5, md5);
SPECIALIZED_HASH(sha1, sha1);
SPECIALIZED_HASH(sha2_224, sha2_256);
SPECIALIZED_HASH(sha2_256, sha2_256);
SPECIALIZED_HASH(sha2_384, sha2_512);
SPECIALIZED_HASH(sha2_512, sha2_512);
SPECIALIZED_HASH(sha3_224, sha3);
SPECIALIZED_HASH(sha3_256, sha3);
SPECIALIZED_HASH(sha3_384, sha3);
SPECIALIZED_HASH(sha3_512, sha3);
SPECIALIZED_HASH(whirlpool, whirlpool);

// CLEANUP
// -------

#undef SPECIALIZED_HASH

PYCPP_END_NAMESPACE
