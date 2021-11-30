/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA256.h
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-256 header file
 *                for the final project
 **********************************/
 /*#ifndef SHA256_H
 #define SHA256_H
 #include <std::string>

 class SHA256
 {
 protected:
     typedef unsigned char uint8;
     typedef unsigned int uint32;
     typedef unsigned long long uint64;

     const static uint32 sha256_k[];
     static const unsigned int SHA224_256_BLOCK_SIZE = (512/8);
 public:
     void init();
     void update(const unsigned char *message, unsigned int len);
     void final(unsigned char *digest);
     static const unsigned int DIGEST_SIZE = ( 256 / 8);

 protected:
     void transform(const unsigned char *message, unsigned int block_nb);
     unsigned int m_tot_len;
     unsigned int m_len;
     unsigned char m_block[2*SHA224_256_BLOCK_SIZE];
     uint32 m_h[8];
 };

 std::std::string sha256(std::std::string input);

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
     *((str) + 3) = (uint8) ((x)      );       \
     *((str) + 2) = (uint8) ((x) >>  8);       \
     *((str) + 1) = (uint8) ((x) >> 16);       \
     *((str) + 0) = (uint8) ((x) >> 24);       \
 }
 #define SHA2_PACK32(str, x)                   \
 {                                             \
     *(x) =   ((uint32) *((str) + 3)      )    \
            | ((uint32) *((str) + 2) <<  8)    \
            | ((uint32) *((str) + 1) << 16)    \
            | ((uint32) *((str) + 0) << 24);   \
 }
 #endif
*/
#ifndef SHA256_H
#define SHA256_H

//#include "../include/funcproj.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>

#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define BSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

std::vector<unsigned long> convert_to_binary(const std::string);

std::vector<unsigned long> pad_to_512_bits(const std::vector<unsigned long>);

std::vector<unsigned long> resize_block(std::vector<unsigned long>);

std::string compute_hash(const std::vector<unsigned long>);

std::string show_as_hex(unsigned long);

void cout_block_state(std::vector<unsigned long>);

std::string show_as_binary(unsigned long);

const bool show_block_state_add_1 = false;

const bool show_distance_from_512bit = false;

const bool show_padding_results = true;

const bool show_working_vars_for_t = false;

const bool show_T1_calculation = false;

const bool show_T2_calculation = false;

const bool show_hash_segments = false;

const bool show_Wt = false;

#endif
