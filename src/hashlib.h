//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Hash functions.
 *
 *  \TODO all of these need to use a secure_string implementation, since
 *  std::string does not clear its buffer.
 */

#pragma once

#include "secure_string.h"
#include <string>
#include <utility>

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

// OBJECTS
// -------


/**
 *  \brief Generic hash context.
 */
struct hash
{
public:
    hash(hash_algorithm algorithm);
    hash(hash_algorithm algorithm, const void* src, size_t srclen);
    hash(hash_algorithm algorithm, const string_view& str);

    hash(const hash&);
    hash& operator=(const hash&);
    hash(hash&&);
    hash& operator=(hash&&);

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    typedef typename std::aligned_storage<sizeof(uintptr_t)>::type memory_type;
    hash_algorithm algorithm;
    memory_type mem;
};


/**
 *  \brief MD2 hash context.
 */
struct md2_hash
{
public:
    md2_hash();
    md2_hash(const void* src, size_t srclen);
    md2_hash(const string_view& str);
    ~md2_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    md2_context* ctx;
};


/**
 *  \brief MD4 hash context.
 */
struct md4_hash
{
public:
    md4_hash();
    md4_hash(const void* src, size_t srclen);
    md4_hash(const string_view& str);
    ~md4_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    md4_context* ctx;
};


/**
 *  \brief MD5 hash context.
 */
struct md5_hash
{
public:
    md5_hash();
    md5_hash(const void* src, size_t srclen);
    md5_hash(const string_view& str);
    ~md5_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    md5_context* ctx;
};


/**
 *  \brief SHA1 hash context.
 */
struct sha1_hash
{
public:
    sha1_hash();
    sha1_hash(const void* src, size_t srclen);
    sha1_hash(const string_view& str);
    ~sha1_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha1_context* ctx;
};


/**
 *  \brief SHA224 hash context.
 */
struct sha2_224_hash
{
public:
    sha2_224_hash();
    sha2_224_hash(const void* src, size_t srclen);
    sha2_224_hash(const string_view& str);
    ~sha2_224_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha2_256_context* ctx;
};


/**
 *  \brief SHA256 hash context.
 */
struct sha2_256_hash
{
public:
    sha2_256_hash();
    sha2_256_hash(const void* src, size_t srclen);
    sha2_256_hash(const string_view& str);
    ~sha2_256_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha2_256_context* ctx;
};


/**
 *  \brief SHA384 hash context.
 */
struct sha2_384_hash
{
public:
    sha2_384_hash();
    sha2_384_hash(const void* src, size_t srclen);
    sha2_384_hash(const string_view& str);
    ~sha2_384_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha2_512_context* ctx;
};


/**
 *  \brief SHA512 hash context.
 */
struct sha2_512_hash
{
public:
    sha2_512_hash();
    sha2_512_hash(const void* src, size_t srclen);
    sha2_512_hash(const string_view& str);
    ~sha2_512_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha2_512_context* ctx;
};


/**
 *  \brief SHA3-224 hash context.
 */
struct sha3_224_hash
{
public:
    sha3_224_hash();
    sha3_224_hash(const void* src, size_t srclen);
    sha3_224_hash(const string_view& str);
    ~sha3_224_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha3_context* ctx;
};


/**
 *  \brief SHA3-256 hash context.
 */
struct sha3_256_hash
{
public:
    sha3_256_hash();
    sha3_256_hash(const void* src, size_t srclen);
    sha3_256_hash(const string_view& str);
    ~sha3_256_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha3_context* ctx;
};


/**
 *  \brief SHA3-384 hash context.
 */
struct sha3_384_hash
{
public:
    sha3_384_hash();
    sha3_384_hash(const void* src, size_t srclen);
    sha3_384_hash(const string_view& str);
    ~sha3_384_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha3_context* ctx;
};


/**
 *  \brief SHA3-512 hash context.
 */
struct sha3_512_hash
{
public:
    sha3_512_hash();
    sha3_512_hash(const void* src, size_t srclen);
    sha3_512_hash(const string_view& str);
    ~sha3_512_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha3_context* ctx;
};


/**
 *  \brief Whirlpool hash context.
 */
struct whirlpool_hash
{
public:
    whirlpool_hash();
    whirlpool_hash(const void* src, size_t srclen);
    whirlpool_hash(const string_view& str);
    ~whirlpool_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    whirlpool_context* ctx;
};
