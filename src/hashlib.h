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

struct md5_context;
struct sha1_context;

// OBJECTS
// -------

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
