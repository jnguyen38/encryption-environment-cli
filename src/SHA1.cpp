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
http://www.zedwood.com/article/cpp-sha1-function
*/

#include "../include/SHA1.h"

//funciton to initialize SHA1
void SHA1::initialize(){
  // according to RFC 1321
  // hex numbers that are standard for use for the hashing funciton
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
  initialize(); // initialize hash
}

//update the string that is being hashed
void SHA1::update(const std::string &string)
{
    //create string stream so that we can extract/operate on the string like a stream
    std::istringstream stream_str(string);
    update(stream_str);
}

//now pass stream_str to another update function - use overloading in C++ to do this
void SHA1::update(std::istream &stream_str)
{
    std::string rest_of_buffer;
    //use read string and update for length of string inputted
    SHA1::read(stream_str, rest_of_buffer, BLOCK_BYTES - (int)buffer.size());
    buffer += rest_of_buffer; //add to buffer

    while (stream_str)
    {
        unsigned long int block[BLOCK_INTS];
        //standard buffer for SHA1
        SHA1::buffer_to_block(buffer, block);
        //perform hashing in transform
        transform(block);
        //read again
        SHA1::read(stream_str, buffer, BLOCK_BYTES);
    }
}


/*
 * Add padding and return the message hash.
 */

std::string SHA1::final()
{
    /* Total number of hashed bits */
    unsigned long long total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

    // Padding that occurs in SHA1 by its definition
    buffer += (char) 0x80;
    unsigned long int orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    unsigned long int block[BLOCK_INTS];
    //standard buffer for SHA1
    SHA1::buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(block); //transform number of times based on length of buffer and string
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this unsigned long long into two unsigned long int */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    transform(block);

    // change from hex to string
    std::ostringstream result;
    for (unsigned int i = 0; i < 5; i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (hash[i] & 0xffffffff);
    }

    /* Reset for next run */
    initialize();

    //get final string
    return result.str();
}

/* Help macros */
//these are defined by the SHA1 algorithm
#define SHA1_ROL(value, bits) (((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))))
#define SHA1_BLK(i) (block[i&15] = SHA1_ROL(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
//these are defined by the SHA1 algorithm
#define SHA1_ROUND_0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + SHA1_BLK(i) + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_2(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0x6ed9eba1 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + SHA1_BLK(i) + 0x8f1bbcdc + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_4(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0xca62c1d6 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);

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
    SHA1_ROUND_0( a, b, c, d, e,  0 );
    SHA1_ROUND_0( e, a, b, c, d,  1 );
    SHA1_ROUND_0( d, e, a, b, c,  2 );
    SHA1_ROUND_0( c, d, e, a, b,  3 );
    SHA1_ROUND_0( b, c, d, e, a,  4 );
    SHA1_ROUND_0( a, b, c, d, e,  5 );
    SHA1_ROUND_0( e, a, b, c, d,  6 );
    SHA1_ROUND_0( d, e, a, b, c,  7 );
    SHA1_ROUND_0( c, d, e, a, b,  8 );
    SHA1_ROUND_0( b, c, d, e, a,  9 );
    SHA1_ROUND_0( a, b, c, d, e, 10 );
    SHA1_ROUND_0( e, a, b, c, d, 11 );
    SHA1_ROUND_0( d, e, a, b, c, 12 );
    SHA1_ROUND_0( c, d, e, a, b, 13 );
    SHA1_ROUND_0( b, c, d, e, a, 14 );
    SHA1_ROUND_0( a, b, c, d, e, 15 );
    SHA1_ROUND_1( e, a, b, c, d, 16 );
    SHA1_ROUND_1( d, e, a, b, c, 17 );
    SHA1_ROUND_1( c, d, e, a, b, 18 );
    SHA1_ROUND_1( b, c, d, e, a, 19 );
    SHA1_ROUND_2( a, b, c, d, e, 20 );
    SHA1_ROUND_2( e, a, b, c, d, 21 );
    SHA1_ROUND_2( d, e, a, b, c, 22 );
    SHA1_ROUND_2( c, d, e, a, b, 23 );
    SHA1_ROUND_2( b, c, d, e, a, 24 );
    SHA1_ROUND_2( a, b, c, d, e, 25 );
    SHA1_ROUND_2( e, a, b, c, d, 26 );
    SHA1_ROUND_2( d, e, a, b, c, 27 );
    SHA1_ROUND_2( c, d, e, a, b, 28 );
    SHA1_ROUND_2( b, c, d, e, a, 29 );
    SHA1_ROUND_2( a, b, c, d, e, 30 );
    SHA1_ROUND_2( e, a, b, c, d, 31 );
    SHA1_ROUND_2( d, e, a, b, c, 32 );
    SHA1_ROUND_2( c, d, e, a, b, 33 );
    SHA1_ROUND_2( b, c, d, e, a, 34 );
    SHA1_ROUND_2( a, b, c, d, e, 35 );
    SHA1_ROUND_2( e, a, b, c, d, 36 );
    SHA1_ROUND_2( d, e, a, b, c, 37 );
    SHA1_ROUND_2( c, d, e, a, b, 38 );
    SHA1_ROUND_2( b, c, d, e, a, 39 );
    SHA1_ROUND_3( a, b, c, d, e, 40 );
    SHA1_ROUND_3( e, a, b, c, d, 41 );
    SHA1_ROUND_3( d, e, a, b, c, 42 );
    SHA1_ROUND_3( c, d, e, a, b, 43 );
    SHA1_ROUND_3( b, c, d, e, a, 44 );
    SHA1_ROUND_3( a, b, c, d, e, 45 );
    SHA1_ROUND_3( e, a, b, c, d, 46 );
    SHA1_ROUND_3( d, e, a, b, c, 47 );
    SHA1_ROUND_3( c, d, e, a, b, 48 );
    SHA1_ROUND_3( b, c, d, e, a, 49 );
    SHA1_ROUND_3( a, b, c, d, e, 50 );
    SHA1_ROUND_3( e, a, b, c, d, 51 );
    SHA1_ROUND_3( d, e, a, b, c, 52 );
    SHA1_ROUND_3( c, d, e, a, b, 53 );
    SHA1_ROUND_3( b, c, d, e, a, 54 );
    SHA1_ROUND_3( a, b, c, d, e, 55 );
    SHA1_ROUND_3( e, a, b, c, d, 56 );
    SHA1_ROUND_3( d, e, a, b, c, 57 );
    SHA1_ROUND_3( c, d, e, a, b, 58 );
    SHA1_ROUND_3( b, c, d, e, a, 59 );
    SHA1_ROUND_4( a, b, c, d, e, 60 );
    SHA1_ROUND_4( e, a, b, c, d, 61 );
    SHA1_ROUND_4( d, e, a, b, c, 62 );
    SHA1_ROUND_4( c, d, e, a, b, 63 );
    SHA1_ROUND_4( b, c, d, e, a, 64 );
    SHA1_ROUND_4( a, b, c, d, e, 65 );
    SHA1_ROUND_4( e, a, b, c, d, 66 );
    SHA1_ROUND_4( d, e, a, b, c, 67 );
    SHA1_ROUND_4( c, d, e, a, b, 68 );
    SHA1_ROUND_4( b, c, d, e, a, 69 );
    SHA1_ROUND_4( a, b, c, d, e, 70 );
    SHA1_ROUND_4( e, a, b, c, d, 71 );
    SHA1_ROUND_4( d, e, a, b, c, 72 );
    SHA1_ROUND_4( c, d, e, a, b, 73 );
    SHA1_ROUND_4( b, c, d, e, a, 74 );
    SHA1_ROUND_4( a, b, c, d, e, 75 );
    SHA1_ROUND_4( e, a, b, c, d, 76 );
    SHA1_ROUND_4( d, e, a, b, c, 77 );
    SHA1_ROUND_4( c, d, e, a, b, 78 );
    SHA1_ROUND_4( b, c, d, e, a, 79 );

    /* Add the working vars back into hash[] */
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;

    /* Count the number of transformations */
    transforms++;
}

// strandard buffer to block function for sha1 hashing
void SHA1::buffer_to_block(const std::string &buffer, unsigned long int block[BLOCK_BYTES])
{
    /* Convert the std::string (byte buffer) to a unsigned long int array (MSB) */
    //this will then allow us to use as a string to return the hash
    for (unsigned long int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4*i+3] & 0xff) | ((buffer[4*i+2] & 0xff)<<8)
                   | ((buffer[4*i+1] & 0xff)<<16) | ((buffer[4*i+0] & 0xff)<<24);
    }
}

void SHA1::read(std::istream &stream_str, std::string &string, int max)
{
    char str_buf[max];
    // stream_str.read will extract max characters from the string and stroe it in str_buf
    stream_str.read(str_buf, max);
    //string is assigned the first .gcount number of characters in str_buff so how long the string is
    string.assign(str_buf, stream_str.gcount());
}

//to run sha1, input string, and it outputs the hashed string
std::string sha1(const std::string &string)
{
    SHA1 sha1_hash; //create class
    sha1_hash.update(string); //input a string, and update calls other functions to hash it
    return sha1_hash.final(); // .final converts the hash to a string, so we can return it
}

/* main for debugging
int main(){
  //hash "abc"
  std::cout << "string: \"abc\"" << std::endl;
  std::cout << "hash: " << sha1("abc") << std::endl << std::endl;

  //hash "cat"
  std::cout << "string: \"cat\"" << std::endl;
  std::cout << "hash: " << sha1("cat") << std::endl;
}
*/
