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

// function that calls all other functions
std::string sha256 (std::string);

// Converts the ASCII string to a binary representation.
std::vector<unsigned long> convert_to_binary(const std::string);

// Pads the messages to make sure they are a multiple of 512 bits.
std::vector<unsigned long> pad_to_512bits(const std::vector<unsigned long>);

// Changes the n 8 bit segments representing every ASCII character to 32 bit words.
std::vector<unsigned long> resize_block(std::vector<unsigned long>);

// The actual hash computing.
std::string compute_hash(const std::vector<unsigned long>);

// show as hex
std::string show_as_hex(unsigned long);

// display current block
void cout_block_state(std::vector<unsigned long>);

// show as binary
std::string show_as_binary(unsigned long);

// initialize variables for displaying steps
const bool show_block_state_add_1 = 1;
const bool show_distance_from_512bit = 1;
const bool show_padding_results = 1;
const bool show_working_vars_for_t = 1;
const bool show_T1_calculation = 1;
const bool show_T2_calculation = 1;
const bool show_hash_segments = 1;
const bool show_Wt = 1;

// define functions
#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define SSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define BSIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

#endif
