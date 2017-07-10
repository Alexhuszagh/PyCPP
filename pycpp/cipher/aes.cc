//  :copyright: (c) 2012-2015 Brad Conte.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] https://github.com/B-Con/crypto-algorithms
 */

#include <pycpp/cipher.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/**
 *  Performs the action of generating the keys that will be used in
 *  every round of encryption. "key" is the user-supplied input key,
 *  "w" is the output key schedule, "keysize" is the length in bits of
 *  "key", must be 128, 192, or 256.
 */
static void aes_key_setup(const uint8_t* key, uint32_t* w, int keysize)
{
    // TODO: restore...
//    int Nb=4,Nr,Nk,idx;
//    WORD temp,Rcon[]={0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,
//                      0x40000000,0x80000000,0x1b000000,0x36000000,0x6c000000,0xd8000000,
//                      0xab000000,0x4d000000,0x9a000000};
//
//    switch (keysize) {
//        case 128: Nr = 10; Nk = 4; break;
//        case 192: Nr = 12; Nk = 6; break;
//        case 256: Nr = 14; Nk = 8; break;
//        default: return;
//    }
//
//    for (idx=0; idx < Nk; ++idx) {
//        w[idx] = ((key[4 * idx]) << 24) | ((key[4 * idx + 1]) << 16) |
//                   ((key[4 * idx + 2]) << 8) | ((key[4 * idx + 3]));
//    }
//
//    for (idx = Nk; idx < Nb * (Nr+1); ++idx) {
//        temp = w[idx - 1];
//        if ((idx % Nk) == 0)
//            temp = SubWord(KE_ROTWORD(temp)) ^ Rcon[(idx-1)/Nk];
//        else if (Nk > 6 && (idx % Nk) == 4)
//            temp = SubWord(temp);
//        w[idx] = w[idx-Nk] ^ temp;
//    }
}

PYCPP_END_NAMESPACE
