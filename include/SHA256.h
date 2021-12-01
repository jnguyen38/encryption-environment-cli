/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA256.h
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-256 header file
 *                for the final project
 **********************************/
#ifndef SHA256_H
#define SHA256_H
#include "project.h"

class SHA256
{
protected:
    // round constants (k)
    const static unsigned int sha256_k[];
    // block size is 64 bits
    static const unsigned int SHA224_256_BLOCK_SIZE = (512/8);

public:
    void init();
    void update(const unsigned char *message, unsigned int len);
    void final(unsigned char *digest);
    // digest size is 32 bits
    static const unsigned int DIGEST_SIZE = ( 256 / 8);

protected:
    void transform(const unsigned char *message, unsigned int block_nb);
    unsigned int m_tot_len;
    unsigned int m_len;
    unsigned char m_block[2*SHA224_256_BLOCK_SIZE];
    unsigned long int m_h[8];
};

std::string sha256(std::string input);

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                 \
{                                             \
    *((str) + 3) = (unsigned char) ((x)      );       \
    *((str) + 2) = (unsigned char) ((x) >>  8);       \
    *((str) + 1) = (unsigned char) ((x) >> 16);       \
    *((str) + 0) = (unsigned char) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)                   \
{                                             \
    *(x) =   ((unsigned int) *((str) + 3)      )    \
           | ((unsigned int) *((str) + 2) <<  8)    \
           | ((unsigned int) *((str) + 1) << 16)    \
           | ((unsigned int) *((str) + 0) << 24);   \
}
#endif
