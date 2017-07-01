//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Cipher functions.
 */

#pragma once

#include <view/string.h>
#include <string>

// OBJECTS
// -------

/**
 *  \brief AES cipher context.
 */
struct aes_cipher
{
public:
    aes_cipher(int keysize = 256);
    ~aes_cipher();

    void key_setup();
    void encrypt(const void* src, size_t srclen, void* dst, size_t dstlen) const;
    std::string encrypt(const string_view& str) const;
    void decrypt(const void* src, size_t srclen, void* dst, size_t dstlen) const;
    std::string decrypt(const string_view& str) const;

private:
// TODO?
//    aes_context* ctx;
};
