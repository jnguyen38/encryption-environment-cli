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
#include <string>
#include <iostream>
#include <fstream>
#include <array>

/*
class SHA256 {

public:
	SHA256();
	void update(const unsigned char * data, size_t length);
	void update(const std::string &data);
	unsigned char * digest();

	static std::string toString(const unsigned char * digest);

private:
	unsigned char  m_data[64];
	unsigned long int m_blocklen;
	unsigned long long m_bitlen;
	unsigned long int m_state[8]; //A, B, C, D, E, F, G, H

	static constexpr std::array<unsigned long int, 64> K = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
		0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
		0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
		0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
		0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
		0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
		0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
		0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
		0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
		0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
		0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};

	static unsigned long int rotr(unsigned long int x, unsigned long int n);
	static unsigned long int choose(unsigned long int e, unsigned long int f, unsigned long int g);
	static unsigned long int majority(unsigned long int a, unsigned long int b, unsigned long int c);
	static unsigned long int sig0(unsigned long int x);
	static unsigned long int sig1(unsigned long int x);
	void transform();
	void pad();
	void revert(unsigned char * hash);
};

#endif

*/
class SHA256
{
protected:
    const static unsigned int sha256_k[];
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

/*
#ifndef SHA256_H
#define SHA256_H

#include "project.h"



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
*/
