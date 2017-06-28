//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Hash functions.
 */

#pragma once

#include "string_view.h"
#include <string>

// FORWARD
// -------

struct md2_context;
struct md4_context;
struct md5_context;
struct sha1_context;
struct sha256_context;

// OBJECTS
// -------

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
 *  \brief SHA256 hash context.
 */
struct sha256_hash
{
public:
    sha256_hash();
    sha256_hash(const void* src, size_t srclen);
    sha256_hash(const string_view& str);
    ~sha256_hash();

    void update(const void* src, size_t srclen);
    void update(const string_view& str);
    size_t digest(void* dst, size_t dstlen) const;
    size_t hexdigest(void* dst, size_t dstlen) const;
    std::string digest() const;
    std::string hexdigest() const;

private:
    sha256_context* ctx;
};
