/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA1.h
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-1 header file
 *                for the final project
 **********************************/
 /*
 This code is adapted from github library, which can be found at link
 http://www.zedwood.com/article/cpp-sha1-function
 */
#ifndef SHA1_H
#define SHA1_H
#include "project.h"

#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>
#include <cstring>
#include <string>
#include <fstream>

class SHA1{


public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();

private:
    // two variables, type static so the static functions can access these elements
    static const unsigned int BLOCK_INTS = 16;  // number of 32bit integers per SHA1 block
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    unsigned long int hash[5];
    std::string buffer;
    unsigned long long transforms;

    void initialize();
    void transform(unsigned long int block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string &buffer, unsigned long int block[BLOCK_BYTES]);
    static void read(std::istream &is, std::string &s, int max);
};

// initialize variables for displaying steps
const bool sha1_show_block_state_add_1 = 1;
const bool sha1_show_distance_from_512bit = 1;
const bool sha1_show_padding_results = 1;
const bool sha1_show_Wt = 1;

//function that allows user to input string and returns final hashed string
std::string sha1(std::string string);

// Converts the ASCII string to a binary representation.
std::vector<unsigned long> sha1_convert_to_binary(const std::string);

// Pads the messages to make sure they are a multiple of 512 bits.
std::vector<unsigned long> sha1_pad_to_512bits(const std::vector<unsigned long>);

// Changes the n 8 bit segments representing every ASCII character to 32 bit words.
std::vector<unsigned long> sha1_resize_block(std::vector<unsigned long>);

//display W(t) values
void show_Wt_values(const std::vector<unsigned long>);

// display current block
void sha1_cout_block_state(std::vector<unsigned long>);

// show as hex
std::string sha1_show_as_hex(unsigned long);

// show as binary
std::string sha1_show_as_binary(unsigned long);

#endif
