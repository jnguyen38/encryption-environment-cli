/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA1.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-1 hash function
 *                for the final project
 **********************************/
/*
This code is adapted from github library, which can be found at link
https://github.com/Breathleas/AES-DES-3DES-MD5-RC4-SHA1-SHA256-BASE64/blob/master/SHA1/sha1.cpp
*/

#include "../include/SHA1.h"

const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
const size_t BLOCK_BYTES = BLOCK_INTS * 4;

//funciton to initialize SHA1
void SHA1::initialize(){
  // according to RFC 1321
  hash[0] = 0x67452301;
  hash[1] = 0xefcdab89;
  hash[2] = 0x98badcfe;
  hash[3] = 0x10325476;
  hash[4] = 0xc3d2e1f0;

  //initialize numbers/reset values
  buffer = "";
  transforms = 0;
}

//initializes SHA1
SHA1::SHA1() : buffer(""), transforms(0) {
  initialize();
}

void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


void SHA1::update(std::istream &is)
{
    std::string rest_of_buffer;
    read(is, rest_of_buffer, BLOCK_BYTES - (int)buffer.size());
    buffer += rest_of_buffer;

    while (is)
    {
        unsigned long int block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(block);
        read(is, buffer, BLOCK_BYTES);
    }
}


/*
 * Add padding and return the message hash.
 */

std::string SHA1::final()
{
    /* Total number of hashed bits */
    unsigned long long total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += (char) 0x80;
    unsigned long int orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    unsigned long int block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(block);
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this unsigned long long into two unsigned long int */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    transform(block);

    /* Hex std::string */
    std::ostringstream result;
    for (unsigned int i = 0; i < 5; i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (hash[i] & 0xffffffff);
    }

    /* Reset for next run */
    initialize();

    return result.str();
}

/*
void SHA1::update(const char* s)
{
	while (true)
	{
		strncpy(&buffer[buffer_size], s, BLOCK_BYTES - buffer_size);
		buffer_size = strnlen(buffer, BLOCK_BYTES);
		if (buffer_size != BLOCK_BYTES)
			return;
		unsigned long int block[BLOCK_INTS];
		SHA1::buffer_to_block(buffer, block);
		transform(hash, block, transforms);
		buffer_size = 0;
		s += BLOCK_BYTES;
	}
}

static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7',
						'8', '9' ,'a', 'b', 'c', 'd', 'e', 'f' };

void uint_to_hex_str(unsigned long int num, char* buff)
{
	for (int32_t k = 0; k < 8; k++)
	{
		// get the equivalent hex digit
		buff[k] = hex[num & 0xF];
		num >>= 4;
	}

	// since we get the digits in the wrong order reverse the digits in the buffer

	for (int32_t k = 0; k < 4; k++)
	{
		// xor swapping
		buff[k] ^= buff[8 - k - 1];
		buff[8 - k - 1] ^= buff[k];
		buff[k] ^= buff[8 - k - 1];
	}
}

// add padding and return the message
void SHA1::final(char* result)
{
	// total number of hashed bits
	unsigned long long_t total_bits = (transforms * BLOCK_BYTES + buffer_size) * 8;

	// padding
	buffer[buffer_size] = char(0x80);
	buffer_size++;
	size_t orig_size = buffer_size;
	while (buffer_size < BLOCK_BYTES)
	{
		buffer[buffer_size] = (char)0x00;
		buffer_size++;
	}

	unsigned long int block[BLOCK_INTS];
	SHA1::buffer_to_block(buffer, block);

	if (orig_size > BLOCK_BYTES - 8)
	{
		transform(hash, block, transforms);
		for (size_t i = 0; i < BLOCK_INTS - 2; i++)
			block[i] = 0;
	}

	// append total_bits, split this unsigned long long_t into two unsigned long int
	block[BLOCK_INTS - 1] = unsigned long int(total_bits);
	block[BLOCK_INTS - 2] = (total_bits >> 32);
	transform(hash, block, transforms);

	const size_t total_length = 8 * sizeof(hash) / sizeof(hash[0]);
	for (size_t i = 0; i < total_length; i++)
		result[i] = '0';

	// hex string
	for (size_t i = 0; i < sizeof(hash) / sizeof(hash[0]); i++)
	{
		char hex_str[8];
		uint_to_hex_str(hash[i], hex_str);
		strncpy(&result[i * 8], hex_str, 8);
	}

	result[total_length] = 0; // null terminate

	// Reset for next run
	SHA1::initialize();
} */

/*
void SHA1::hash_word(const char* str, char* result)
{
	SHA1 sha_hash;
	sha_hash.update(str);
	sha_hash.final(result);
}*/

static unsigned long int rol(const unsigned long int value, const size_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


static unsigned long int blk(const unsigned long int block[BLOCK_INTS], const size_t i)
{
    return rol(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i], 1);
}

/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

//definiton of rotation 0
static void R0(const unsigned long int block[BLOCK_INTS], const unsigned long int v, unsigned long int &w, const unsigned long int x, const unsigned long int y, unsigned long int &z, const size_t i)
{
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}

//definiton of rotation 1
static void R1(unsigned long int block[BLOCK_INTS], const unsigned long int v, unsigned long int &w, const unsigned long int x, const unsigned long int y, unsigned long int &z, const size_t i)
{
    block[i] = blk(block, i);
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}

//definiton of rotation 2
static void R2(unsigned long int block[BLOCK_INTS], const unsigned long int v, unsigned long int &w, const unsigned long int x, const unsigned long int y, unsigned long int &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0x6ed9eba1 + rol(v, 5);
    w = rol(w, 30);
}

//definiton of rotation 3
static void R3(unsigned long int block[BLOCK_INTS], const unsigned long int v, unsigned long int &w, const unsigned long int x, const unsigned long int y, unsigned long int &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (((w|x)&y)|(w&x)) + block[i] + 0x8f1bbcdc + rol(v, 5);
    w = rol(w, 30);
}

//definiton of rotation 4
static void R4(unsigned long int block[BLOCK_INTS], const unsigned long int v, unsigned long int &w, const unsigned long int x, const unsigned long int y, unsigned long int &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0xca62c1d6 + rol(v, 5);
    w = rol(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

//applies all the rotation functions on a, b, c, d, e
//for sha1 have to define as unsigned long int
void SHA1::transform(unsigned long int block[BLOCK_BYTES])
{
    /* Copy hash[] to working vars */
    unsigned long int a = hash[0];
    unsigned long int b = hash[1];
    unsigned long int c = hash[2];
    unsigned long int d = hash[3];
    unsigned long int e = hash[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    //this is defined in several examples
    R0(block, a, b, c, d, e,  0);
    R0(block, e, a, b, c, d,  1);
    R0(block, d, e, a, b, c,  2);
    R0(block, c, d, e, a, b,  3);
    R0(block, b, c, d, e, a,  4);
    R0(block, a, b, c, d, e,  5);
    R0(block, e, a, b, c, d,  6);
    R0(block, d, e, a, b, c,  7);
    R0(block, c, d, e, a, b,  8);
    R0(block, b, c, d, e, a,  9);
    R0(block, a, b, c, d, e, 10);
    R0(block, e, a, b, c, d, 11);
    R0(block, d, e, a, b, c, 12);
    R0(block, c, d, e, a, b, 13);
    R0(block, b, c, d, e, a, 14);
    R0(block, a, b, c, d, e, 15);
    R1(block, e, a, b, c, d,  0);
    R1(block, d, e, a, b, c,  1);
    R1(block, c, d, e, a, b,  2);
    R1(block, b, c, d, e, a,  3);
    R2(block, a, b, c, d, e,  4);
    R2(block, e, a, b, c, d,  5);
    R2(block, d, e, a, b, c,  6);
    R2(block, c, d, e, a, b,  7);
    R2(block, b, c, d, e, a,  8);
    R2(block, a, b, c, d, e,  9);
    R2(block, e, a, b, c, d, 10);
    R2(block, d, e, a, b, c, 11);
    R2(block, c, d, e, a, b, 12);
    R2(block, b, c, d, e, a, 13);
    R2(block, a, b, c, d, e, 14);
    R2(block, e, a, b, c, d, 15);
    R2(block, d, e, a, b, c,  0);
    R2(block, c, d, e, a, b,  1);
    R2(block, b, c, d, e, a,  2);
    R2(block, a, b, c, d, e,  3);
    R2(block, e, a, b, c, d,  4);
    R2(block, d, e, a, b, c,  5);
    R2(block, c, d, e, a, b,  6);
    R2(block, b, c, d, e, a,  7);
    R3(block, a, b, c, d, e,  8);
    R3(block, e, a, b, c, d,  9);
    R3(block, d, e, a, b, c, 10);
    R3(block, c, d, e, a, b, 11);
    R3(block, b, c, d, e, a, 12);
    R3(block, a, b, c, d, e, 13);
    R3(block, e, a, b, c, d, 14);
    R3(block, d, e, a, b, c, 15);
    R3(block, c, d, e, a, b,  0);
    R3(block, b, c, d, e, a,  1);
    R3(block, a, b, c, d, e,  2);
    R3(block, e, a, b, c, d,  3);
    R3(block, d, e, a, b, c,  4);
    R3(block, c, d, e, a, b,  5);
    R3(block, b, c, d, e, a,  6);
    R3(block, a, b, c, d, e,  7);
    R3(block, e, a, b, c, d,  8);
    R3(block, d, e, a, b, c,  9);
    R3(block, c, d, e, a, b, 10);
    R3(block, b, c, d, e, a, 11);
    R4(block, a, b, c, d, e, 12);
    R4(block, e, a, b, c, d, 13);
    R4(block, d, e, a, b, c, 14);
    R4(block, c, d, e, a, b, 15);
    R4(block, b, c, d, e, a,  0);
    R4(block, a, b, c, d, e,  1);
    R4(block, e, a, b, c, d,  2);
    R4(block, d, e, a, b, c,  3);
    R4(block, c, d, e, a, b,  4);
    R4(block, b, c, d, e, a,  5);
    R4(block, a, b, c, d, e,  6);
    R4(block, e, a, b, c, d,  7);
    R4(block, d, e, a, b, c,  8);
    R4(block, c, d, e, a, b,  9);
    R4(block, b, c, d, e, a, 10);
    R4(block, a, b, c, d, e, 11);
    R4(block, e, a, b, c, d, 12);
    R4(block, d, e, a, b, c, 13);
    R4(block, c, d, e, a, b, 14);
    R4(block, b, c, d, e, a, 15);

    /* Add the working vars back into hash[] */
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;

    /* Count the number of transformations */
    transforms++;
}


void SHA1::buffer_to_block(const std::string &buffer, unsigned long int block[BLOCK_BYTES])
{
    /* Convert the std::string (byte buffer) to a unsigned long int array (MSB) */
    for (unsigned long int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4*i+3] & 0xff)
                   | (buffer[4*i+2] & 0xff)<<8
                   | (buffer[4*i+1] & 0xff)<<16
                   | (buffer[4*i+0] & 0xff)<<24;
    }
}

void SHA1::read(std::istream &is, std::string &s, int max)
{
    char sbuf[max];
    is.read(sbuf, max);
    s.assign(sbuf, is.gcount());
}

//to run sha1, input string, and it outputs the hashed string
std::string sha1_hash(const std::string &string)
{
    SHA1 sha1_hash;
    sha1_hash.update(string);
    return sha1_hash.final();
}
