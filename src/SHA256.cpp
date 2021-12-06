/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA256.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-256 hash function main
 *                for the final project
 **********************************/

 #include <iostream>
 #include <sstream>
 #include <bitset>
 #include <vector>
 #include <iomanip>
 #include <cstring>

 #include "../include/SHA256.h"

 string sha256 (string message) {
   // This will hold all the blocks.
  std::vector<unsigned long> block;

  // First convert this to a vector of strings representing 8 bit variables.
  block = convert_to_binary(message);

  // Pad it so that the message will be a full 512 bits long.
  block = pad_to_512bits(block);

  // Combine the seperate 8 bit sections into single 32 bit sections.
  block = resize_block(block);

  // This is what does the actual hashing.
  std::string hash = compute_hash(block);

  return hash;
 }

 /* for testing purposes only
 int main(int argc, char* argv[])
 {
 	std::string message = "";

 	switch (argc) {
 	    case 1:
 	      std::cout << "No input string found, running test using abc." << std::endl;
 			  message = "abc";
 	      break;
 	    case 2:
 	    	if (strlen(argv[1]) > 55)
 	    	{
 	    		std::cout << "Your string is over 55 characters long, please use a"
 	    			 << " shorter message!" << std::endl;
 	    		return 0;
 	    	}
 	        message = argv[1];
 	        break;
 	    default:
 	        std::cout << "Too many things in the command line." << std::endl;
 	        exit(-1);
 	        break;
 	}

    // This will hold all the blocks.
   std::vector<unsigned long> block;

   // First convert this to a vector of strings representing 8 bit variables.
   block = convert_to_binary(message);

   // Pad it so that the message will be a full 512 bits long.
   block = pad_to_512bits(block);

   // Combine the seperate 8 bit sections into single 32 bit sections.
   block = resize_block(block);

   // This is what does the actual hashing.
   std::string hash = compute_hash(block);

 	 std::cout << hash << std::endl;

 	 return 0;
 }*/

 // Resize the blocks from 64 8 bit sections to 16 32 bit sections.
 // Input : Vector of individual 8 bit ascii values
 // Output : Vector of 32 bit words which are combined ascii values.
 // The message length should be no more than 55 characters (passwords can be no
 // longer than 16 characters)
 std::vector<unsigned long> resize_block(std::vector<unsigned long> input)
 {
 	std::vector<unsigned long> output(16);

 	// Loop through the 64 sections by 4 steps and merge those 4 sections.
 	for(int i = 0; i < 64; i = i + 4)
 	{
 		// Make a big 32 bit section
 		std::bitset<32> temp(0);

 		// Shift the blocks to their assigned spots and OR them with the original
 		// to combine them.
 		temp = (unsigned long) input[i] << 24;
 		temp |= (unsigned long) input[i + 1] << 16;
 		temp |= (unsigned long) input[i + 2] << 8;
 		temp |= (unsigned long) input[i + 3];

 		// Puts the new 32 bit word into the correct output array location.
 		output[i/4] = temp.to_ulong();
 	}

 	return output;
 }

 // Shows the current contents of all the blocks in binary
 // Input : Vector of the current blocks.
 // Output : Contents of each block in binary and hex.
 void cout_block_state(std::vector<unsigned long> block)
 {
   std::cout << "---- Current State of block ----" << std::endl;
 	 for (unsigned int i = 0; i < block.size(); i++)
 	 {
 		  std::cout << "block[" << i << "] binary: " << show_as_binary(block[i])
 			<< "     hex y: 0x" << show_as_hex(block[i]) << std::endl;
 	  }
  }

 // Shows the current contents of the block in hex.
 // Input : A 32 or less bit block
 // Output : Contents of the block in hex as a string.
 std::string show_as_hex(unsigned long input)
 {
 	std::bitset<32> bs(input);
 	long unsigned n = bs.to_ulong();

 	std::stringstream sstream;
 	sstream << std::hex << std::setw(8) << std::setfill('0') << n;
 	std::string temp;
 	sstream >> temp;

 	return temp;
 }

 // Shows the current contents of the block in binary.
 // Input : A 32 or less bit block
 // Output : Contents of the block in binary as a string.
 std::string show_as_binary(unsigned long input)
 {
 	std::bitset<8> bs(input);
 	return bs.to_string();
 }

 // Takes the string and convers all characters to their ASCII Binary equivilents.
 // Input : A string of any length
 // Output : A vector consisting of one 8 bit value per ASCII character.
 std::vector<unsigned long> convert_to_binary(const std::string input)
 {
 	// make a vector to hold all the ASCII character values.
 	std::vector<unsigned long> block;

 	// For each character, convert the ASCII chararcter to its binary
 	// representation.
 	for (unsigned int i = 0; i < input.size(); ++i)
 	{
 		// Make a temporary variable called B to store the 8 bit pattern
 		// for the ASCII value.
 		std::bitset<8> b(input.c_str()[i]);

 		// Add that 8 bit pattern into the block.
 		block.push_back(b.to_ulong());
 	}

 	return block;
 }

 // Takes the vector of ASCII values in binary and pad it so that there will be a
 // total of 512 bits.
 // Padding specifically consists of having your message, adding 1, and adding 0's to
 // it so you will have the current length be 448 bits long, and then adding 64 bits
 // which say how long the original message was, giving you a total of 512 bits.
 // Input : The message in the form of a vector containing 8 bit binary ASCII values.
 // Output : A padded vector consisting of one 8 bit value per ASCII character. */
 std::vector<unsigned long> pad_to_512bits(std::vector<unsigned long> block)
 {
 	long unsigned int l = block.size() * 8;

 	// Equation for padding is l + 1 + k = 448 mod 512
 	// Simplified to: l + 1 + k = 448
 	//		  448 - 1 - l = k
 	//		  447 - l = k
 	// l = length of message in bits
 	// k = how much zero's to add so new message will be a multiple of 512.
 	long unsigned int k = 447 - l;

 	// First add in another 8 bit block with the first bit set to 1
 	if(show_block_state_add_1)
 		cout_block_state(block);

 	unsigned long t1 = 0x80;
 	block.push_back(t1);

 	if(show_block_state_add_1)
 		cout_block_state(block);

 	// 7 zero's added, so subtract 7 from k.
 	k = k - 7;

 	// Find how far away from a 512 bit message
 	if (show_distance_from_512bit)
 	{
 		std::cout << "l: " << l << std::endl;
 		std::cout << "k: " << k + 7 << std::endl;
 	}

 	if (show_distance_from_512bit)
 		std::cout << "adding " << k / 8 << " empty eight bit blocks!" << std::endl;

 	// Add 8 bit blocks containing zero's
 	for(unsigned int i = 0; i < k / 8; i++)
 		block.push_back(0x00000000);

 	// Add l in the binary form of eight 8 bit blocks.
 	std::bitset<64> big_64bit_blob(l);
 	if (show_distance_from_512bit)
 		std::cout << "l in a 64 bit binary chunk: \n\t" << big_64bit_blob << std::endl;

 	// Split up that 64 bit blob into 8 bit sections.
 	std::string big_64bit_string = big_64bit_blob.to_string();

 	// Push the first block into the 56th position.
 	std::bitset<8> temp_string_holder1(big_64bit_string.substr(0,8));
 	block.push_back(temp_string_holder1.to_ulong());

 	// Push all the rest of the 8 bit blocks in.
 	for(int i = 8; i < 63; i=i+8)
 	{
 		std::bitset<8> temp_string_holder2(big_64bit_string.substr(i,8));
 		block.push_back(temp_string_holder2.to_ulong());
 	}

 	if (show_padding_results)
 	{
 		std::cout << "Current 512 bit pre-processed hash in binary:" << std::endl;
 			for(unsigned int i = 0; i < block.size(); i=i+4)
 				std::cout << i << ": " << show_as_binary(block[i]) << "     "
 				     << i + 1 << ": " << show_as_binary(block[i+1]) << "     "
 				     << i + 2 << ": " << show_as_binary(block[i+2]) << "     "
 				     << i + 3 << ": " << show_as_binary(block[i+3]) << std::endl;

 		std::cout << "Current 512 bit pre-processed hash in hex:" << std::endl;
 		for(unsigned int i = 0; i < block.size(); i=i+4)
 			std::cout << i << ": " << "0x" + show_as_hex(block[i]) << "     "
 			     << i + 1 << ": " << "0x" + show_as_hex(block[i+1]) << "     "
 			     << i + 2 << ": " << "0x" + show_as_hex(block[i+2]) << "     "
 			     << i + 3 << ": " << "0x" + show_as_hex(block[i+3]) << std::endl;
 	}
 	return block;
 }

 // Input : The 512 bit padded message as a vector containing 8 bit binary ASCII values.
 // Output : A string representing the hash.
 std::string compute_hash(const std::vector<unsigned long> block)
 {
 	// These words represent the first 32
 	// bits of the fractional parts of the cube roots of the first sixty-
 	// four prime numbers.
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

 	// Initial Hash Values, first thirty-two bits of the fractional parts of
 	// the square roots of the first eight prime numbers.
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

 		// Have to make sure numbers are still 32 bits
 		W[t] = W[t] & 0xFFFFFFFF;

 		if (show_Wt)
 			std::cout << "W[" << t << "]: " << W[t] << std::endl;
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
 		     << "E        F        G        H        T1       T2" << std::endl;

 	for( int t = 0; t < 64; t++)
 	{
 		temp1 = h + EP1(e) + CH(e,f,g) + k[t] + W[t];
 		if ((t == 20) & show_T1_calculation)
 		{
 			std::cout << "h: 0x" << std::hex << h << "  dec:" << std::dec << h
 			     << "  sign:" << std::dec << (int)h << std::endl;
 			std::cout << "EP1(e): 0x" << std::hex << EP1(e) << "  dec:"
 			     << std::dec << EP1(e) << "  sign:" << std::dec << (int)EP1(e)
 			     << std::endl;
 			std::cout << "CH(e,f,g): 0x" << std::hex << CH(e,f,g) << "  dec:"
 			     << std::dec << CH(e,f,g) << "  sign:" << std::dec
 			     << (int)CH(e,f,g) << std::endl;
 			std::cout << "k[t]: 0x" << std::hex << k[t] << "  dec:" << std::dec
 			     << k[t] << "  sign:" << std::dec << (int)k[t] << std::endl;
 			std::cout << "W[t]: 0x" << std::hex << W[t] << "  dec:" << std::dec
 			     << W[t] << "  sign:" << std::dec << (int)W[t] << std::endl;
 			std::cout << "temp1: 0x" << std::hex << temp1 << "  dec:" << std::dec
 			     << temp1 << "  sign:" << std::dec << (int)temp1 << std::endl;
 		}

 		temp2 = EP0(a) + MAJ(a,b,c);

 		// Shows the variables and operations for getting T2.
 		if ((t == 20) & show_T2_calculation)
 		{
 			std::cout << "a: 0x" << std::hex << a << "  dec:" << std::dec << a
 			     << "  sign:" << std::dec << (int)a << std::endl;
 			std::cout << "b: 0x" << std::hex << b << "  dec:" << std::dec << b
 			     << "  sign:" << std::dec << (int)b << std::endl;
 			std::cout << "c: 0x" << std::hex << c << "  dec:" << std::dec << c
 			     << "  sign:" << std::dec << (int)c << std::endl;
 			std::cout << "EP0(a): 0x" << std::hex << EP0(a) << "  dec:"
 			     << std::dec << EP0(a) << "  sign:" << std::dec << (int)EP0(a)
 			     << std::endl;
 			std::cout << "MAJ(a,b,c): 0x" << std::hex << MAJ(a,b,c) << "  dec:"
 			     << std::dec << MAJ(a,b,c) << "  sign:" << std::dec
 			     << (int)MAJ(a,b,c) << std::endl;
 			std::cout << "temp2: 0x" << std::hex << temp2 << "  dec:" << std::dec
 			     << temp2 << "  sign:" << std::dec << (int)temp2 << std::endl;
 		}

 		h = g;
 		g = f;
 		f = e;
 		e = (d + temp1) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.
 		d = c;
 		c = b;
 		b = a;
 		a = (temp1 + temp2) & 0xFFFFFFFF; // Makes sure that we are still using 32 bits.

 		// Shows the contents of each working variable for the turn T.
 		if (show_working_vars_for_t)
 		{
 			std::cout << "t= " << t << " " << std::endl;
 			std::cout << show_as_hex (a) << " " << show_as_hex (b) << " "
 			     << show_as_hex (c) << " " << show_as_hex (d) << " "
 			     << show_as_hex (e) << " " << show_as_hex (f) << " "
 			     << show_as_hex (g) << " " << show_as_hex (h) << " "
 			     << std::endl;
 		}
 	}

 	// Shows the contents of each hash segment.
 	if(show_hash_segments)
 	{
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

 	// Add up all the working variables to each hash and make sure we are still
 	// working with solely 32 bit variables.
 	H0 = (H0 + a) & 0xFFFFFFFF;
 	H1 = (H1 + b) & 0xFFFFFFFF;
 	H2 = (H2 + c) & 0xFFFFFFFF;
 	H3 = (H3 + d) & 0xFFFFFFFF;
 	H4 = (H4 + e) & 0xFFFFFFFF;
 	H5 = (H5 + f) & 0xFFFFFFFF;
 	H6 = (H6 + g) & 0xFFFFFFFF;
 	H7 = (H7 + h) & 0xFFFFFFFF;

 	// Append the hash segments together one after the other to get the full
 	// 256 bit hash.
 	return show_as_hex(H0) + show_as_hex(H1) + show_as_hex(H2) +
 		   show_as_hex(H3) + show_as_hex(H4) + show_as_hex(H5) +
 		   show_as_hex(H6) + show_as_hex(H7);
}
