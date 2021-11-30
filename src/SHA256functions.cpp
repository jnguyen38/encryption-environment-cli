/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA256functions.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-256 hash function functions
 *                for the final project
 **********************************/

#include "../include/SHA256.h"

// This function resizes the blocks from 64 8 bit sections to 16 32 bit
// sections. The password length should be no greater than 55 characters
// (Passwords should not be longer than this)
std::vector<unsigned long> resize_block (std::vector <unsigned long> input) {
  std::vector<unsigned long> output(16);

  // loop through the 64 sections by 4 steps and merge those 4 sections to
  // create 32 bit sections
  for (int i = 0; i < 64; i = i + 4) {
    // make a 32 bit section
    std::bitset<32> temp(0);

    // shift the blocks and OR them with the original to combine them
    temp = (unsigned long) input[i] << 24;
    temp |= (unsigned long) input[i+1] << 16;
    temp |= (unsigned long) input[i+2] << 8;
    temp |= (unsigned long) input[i+3];

    // put the new 32-bit word in the proper array location
    output[i/4] = temp.to_ulong();
  }

  return output;
}

// This function shows the current contents of all the blocks in binary
void cout_block_state (std::vector<unsigned long> block)
{
	std::cout << "---- Current State of block ----\n";
	for (unsigned int i = 0; i < block.size(); i++)
	{
		std::cout << "block[" << i << "] binary: " << show_as_binary(block[i])
			<< "     hex y: 0x" << show_as_hex(block[i]) << std::endl;
	}
}

// This function shows the contents of the (32-bit or less) block in hex
std::string show_as_hex (unsigned long input) {
  std::bitset<32> bs(input);
  long unsigned usign = bs.to_ulong();

  std::stringstream sstream;
  sstream << std::hex << std::setw(8) << std::setfill('0') << usign;
  std::string temp;
  sstream >> temp;

  return temp;
}

// This function shows the contents of the (32-bit or less) block in binary
std::string show_as_binary (unsigned long input) {
  std::bitset<8> bs(input);
  return bs.to_string();
}

// This function takes a std::string and converts all characters to their ASCII
// binary equivalents
std::vector<unsigned long> convert_to_binary (const std::string input) {
  std::vector<unsigned long> block;

  // for each character, convert its ASCII representation to show_as_binary
  for (unsigned int i = 0; i < input.size(); ++i) {
    // temp to store 8-bit binary representation of character
    std::bitset<8> temp(input.c_str()[i]);

    // add this to the block
    block.push_back(temp.to_ulong());
  }

  return block;
}

// This function pads the ASCII values in binary so that there is a total of
// 512 bits. (Takes the password, adds 1, and then adds 0's so that the total
// length is 448 bits long, with 64 bits left to tell how long the original
// password was)
std::vector<unsigned long> pad_to_512_bits (std::vector <unsigned long> block) {
  long unsigned int length = block.size() * 8;

  // num of 0's is 448 - length - 1
  long unsigned int zeros = 447 - length;

  if(show_block_state_add_1)
		cout_block_state(block);

  // add an 8-bit block with the first bit set to 1
  unsigned long one = 0x80;
  block.push_back(one);

  if(show_block_state_add_1)
		cout_block_state(block);

  // subtract 7 from zeros because 7 zeros were just added
  zeros = zeros - 7;

  // Find how far away we are from a 512-bit message
	if (show_distance_from_512bit)
	{
		std::cout << "length: " << length << std::endl;
		std::cout << "zeros: " << zeros + 7 << std::endl;
	}

	if (show_distance_from_512bit)
		std::cout << "adding " << zeros / 8 << " empty eight bit blocks\n";

  // add 8-bit blocks containing zeros
  for (unsigned int i = 0; i < length/8; ++i) {
    block.push_back(0x00000000);
  }

  // add length in the binary form of 8 8-bit blocks
  std::bitset<64> block_64_bit(length);
  if (show_distance_from_512bit)
		std::cout << "length in a 64 bit binary block: \n\t" << block_64_bit << std::endl;

  // split the 64-bit block into 8-bit sections
  std::string block_64_bit_string = block_64_bit.to_string();

  // push the first block into the 56th position
  std::bitset<8> temp_string(block_64_bit_string.substr(0,8));
  block.push_back(temp_string.to_ulong());

  // push the rest of the 8-bit blocks
  for (int i = 8; i < 63; i = i + 8) {
    std::bitset<8> temp_string_2(block_64_bit_string.substr(i,8));
    block.push_back(temp_string_2.to_ulong());
  }

  // display
	if (show_padding_results)
	{
		std::cout << "Current 512 bit pre-processed hash in binary: \n";
			for(unsigned int i = 0; i < block.size(); i=i+4)
				std::cout << i << ": " << show_as_binary(block[i]) << "\t"
				     << i + 1 << ": " << show_as_binary(block[i+1]) << "\t"
				     << i + 2 << ": " << show_as_binary(block[i+2]) << "\t"
				     << i + 3 << ": " << show_as_binary(block[i+3]) << std::endl;

		std::cout << "Current 512 bit pre-processed hash in hex: \n";
		for(unsigned int i = 0; i < block.size(); i=i+4)
			std::cout << i << ": " << "0x" + show_as_hex(block[i]) << "\t"
			     << i + 1 << ": " << "0x" + show_as_hex(block[i+1]) << "\t"
			     << i + 2 << ": " << "0x" + show_as_hex(block[i+2]) << "\t"
			     << i + 3 << ": " << "0x" + show_as_hex(block[i+3]) << std::endl;
	}
	return block;
}

// This function takes the 512-bit padded password and returns a std::string
// representing the hash.
std::string compute_hash (const std::vector<unsigned long> block) {
  // these words represent the first 32-bits of the fractional parts of the
  // cube roots of the first 64 prime numbers
  unsigned long k[64] = {
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,
		0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,
		0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,
		0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,
		0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,
		0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};

  // these initial hash values are the first 32-bits of the fractional parts
  // of the square roots of the first 8 prime numbers
  unsigned long static H0 = 0x6a09e667;
	unsigned long static H1 = 0xbb67ae85;
	unsigned long static H2 = 0x3c6ef372;
	unsigned long static H3 = 0xa54ff53a;
	unsigned long static H4 = 0x510e527f;
	unsigned long static H5 = 0x9b05688c;
	unsigned long static H6 = 0x1f83d9ab;
	unsigned long static H7 = 0x5be0cd19;

  unsigned long W[64];

  for(int t = 0; t <= 15; t++)
	{
		W[t] = block[t] & 0xFFFFFFFF;

		if (show_Wt)
			std::cout << "W[" << t << "]: 0x" << show_as_hex(W[t]) << std::endl;
	}

  for(int t = 16; t <= 63; t++)
	{
		W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];

		// ensures we are still dealing with 32 bit numbers
		W[t] = W[t] & 0xFFFFFFFF;

		if (show_Wt)
			std::cout << "W[" << t << "]: " << W[t];
	}

  unsigned long temp1;
	unsigned long temp2;
	unsigned long a = H0;
	unsigned long b = H1;
	unsigned long c = H2;
	unsigned long d = H3;
	unsigned long e = H4;
	unsigned long f = H5;
	unsigned long g = H6;
	unsigned long h = H7;

  if(show_working_vars_for_t)
		std::cout << "         A        B        C        D        "
		     << "E        F        G        H        T1       T2\n";

  for( int t = 0; t < 64; t++) {
    temp1 = h + EP1(e) + CH(e,f,g) + k[t] + W[t];

    if ((t == 20) & show_T1_calculation) {
      std::cout << "h: 0x" << h << "  dec:" << h << "  sign:" << (int)h << std::endl;
      std::cout << "EP1(e): 0x" << EP1(e) << "  dec:" << EP1(e) << "  sign:" << (int)EP1(e) << std::endl;
      std::cout << "CH(e,f,g): 0x" << CH(e,f,g) << "  dec:" << CH(e,f,g) << "  sign:" << (int)CH(e,f,g) << std::endl;
      std::cout << "k[t]: 0x" << k[t] << "  dec:" << k[t] << "  sign:" << (int)k[t] << std::endl;
      std::cout << "W[t]: 0x" << W[t] << "  dec:" << W[t] << "  sign:" << (int)W[t] << std::endl;
      std::cout << "temp1: 0x" << temp1 << "  dec:" << temp1 << "  sign:" << (int)temp1 << std::endl;
    }

    temp2 = EP0(a) + MAJ(a,b,c);

    if ((t == 20) & show_T2_calculation) {
      std::cout << "a: 0x" << a << "  dec:" << a << "  sign:" << (int)a << std::endl;
			std::cout << "b: 0x" << b << "  dec:" << b << "  sign:" << (int)b << std::endl;
			std::cout << "c: 0x" << c << "  dec:" << c << "  sign:" << (int)c << std::endl;
			std::cout << "EP0(a): 0x" << EP0(a) << "  dec:" << EP0(a) << "  sign:" << (int)EP0(a) << std::endl;
			std::cout << "MAJ(a,b,c): 0x" << MAJ(a,b,c) << "  dec:" << MAJ(a,b,c) << "  sign:" << (int)MAJ(a,b,c) << std::endl;
			std::cout << "temp2: 0x" << temp2 << "  dec:" << temp2 << "  sign:" << (int)temp2 << std::endl;
    }

		h = g;
		g = f;
		f = e;
		e = (d + temp1) & 0xFFFFFFFF; // ensures that we are still using 32 bits
		d = c;
		c = b;
		b = a;
		a = (temp1 + temp2) & 0xFFFFFFFF; // ensures that we are still using 32 bits

    if (show_working_vars_for_t) {
			std::cout << "t= " << t << " ";
			std::cout << show_as_hex (a) << " " << show_as_hex (b) << " "
			     << show_as_hex (c) << " " << show_as_hex (d) << " "
			     << show_as_hex (e) << " " << show_as_hex (f) << " "
			     << show_as_hex (g) << " " << show_as_hex (h) << " "
			     << std::endl;
    }
  }

    // show the contents of each hash function
    if(show_hash_segments) {
      std::cout << "H0: " << show_as_hex (H0) << " + " << show_as_hex (a)
			 << " " << show_as_hex (H0 + a) << std::endl;
		  std::cout << "H1: " << show_as_hex (H1) << " + " << show_as_hex (b)
			 << " " << show_as_hex (H1 + b) << std::endl;
		  std::cout << "H2: " << show_as_hex (H2) << " + " << show_as_hex (c)
			 << " " << show_as_hex (H2 + c) << std::endl;
		  std::cout << "H3: " << show_as_hex (H3) << " + " << show_as_hex (d)
			 << " " << show_as_hex (H3 + d) << std::endl;
		  std::cout << "H4: " << show_as_hex (H4) << " + " << show_as_hex (e)
			 << " " << show_as_hex (H4 + e) << std::endl;
		  std::cout << "H5: " << show_as_hex (H5) << " + " << show_as_hex (f)
			 << " " << show_as_hex (H5 + f) << std::endl;
		  std::cout << "H6: " << show_as_hex (H6) << " + " << show_as_hex (g)
			 << " " << show_as_hex (H6 + g) << std::endl;
		  std::cout << "H7: " << show_as_hex (H7) << " + " << show_as_hex (h)
			 << " " << show_as_hex (H7 + h) << std::endl;
    }

    // ensures we are still working with only 32-bit variables
    H0 = (H0 + a) & 0xFFFFFFFF;
	  H1 = (H1 + b) & 0xFFFFFFFF;
	  H2 = (H2 + c) & 0xFFFFFFFF;
	  H3 = (H3 + d) & 0xFFFFFFFF;
	  H4 = (H4 + e) & 0xFFFFFFFF;
	  H5 = (H5 + f) & 0xFFFFFFFF;
	  H6 = (H6 + g) & 0xFFFFFFFF;
	  H7 = (H7 + h) & 0xFFFFFFFF;

    // append the hash segments together to get the 256-bit hash
    return show_as_hex(H0) + show_as_hex(H1) + show_as_hex(H2) +
		   show_as_hex(H3) + show_as_hex(H4) + show_as_hex(H5) +
		   show_as_hex(H6) + show_as_hex(H7);
}
