/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: SHA1.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the SHA-1 hash function
 *                for the final project
 **********************************/

 #include "../include/SHA1.h"

//function to call to compute full hash with steps, inputs string and returns hashed string
std::string sha1(std::string string) {
  // This will hold all the blocks.
  std::vector<unsigned long> block1;
  std::cout << "\033[0m";

  // First convert this to a vector of strings representing 8 bit variables.
  block1 = convert_to_binary(string);

  // Pad it so that the message will be a full 512 bits long.
  block1 = pad_to_512bits(block1);

  // Combine the seperate 8 bit sections into single 32 bit sections.
  block1 = resize_block(block1);

  //display values for W(t), when the block is broken down to 16, 32 bit parts
  show_Wt_values(block1);

  SHA1 sha1_hash; //create class
  sha1_hash.update(string); //input a string, and update calls other functions to hash it

  // .final converts the hash to a string, so we can return it
  std::string finalHash = sha1_hash.final(); //set variable to final hash string

  //display final hash string
  std::cout << "\n\n*****Step 7*****\n";
  std::cout << "Concatenate the hash values. Final hash:" << std::endl;
  std::cout << finalHash << "\n" << std::endl;

  //return final string
  return finalHash;
}

// Shows the current contents of all the blocks in binary
// Input : Vector of the current blocks.
// Output : Contents of each block in binary and hex.
void cout_block_state(std::vector<unsigned long> block)
{
	 //display current blocks
   std::cout << "---- Current State of block ----" << std::endl;
	 for (unsigned int i = 0; i < block.size(); i++)
	 {
		  std::cout << "block[" << i << "] binary: " << show_as_binary(block[i])
			<< "\thex: 0x" << show_as_hex(block[i]) << std::endl;
	  }
}

// Shows the current contents of the block in binary.
// Input : A 32 or less bit block
// Output : Contents of the block in binary as a string.
std::string show_as_binary(unsigned long input)
{
  // take string and convert to binary
  std::bitset<8> bs(input);
  return bs.to_string();
}

// Shows the current contents of the block in hex.
// Input : A 32 or less bit block
// Output : Contents of the block in hex as a string.
std::string show_as_hex(unsigned long input)
{
	 //use bitset
	std::bitset<32> bs(input);
	long unsigned n = bs.to_ulong();

	//create string stream to convert every character to hex
	std::stringstream sstream;
	sstream << std::hex << std::setw(8) << std::setfill('0') << n;
	std::string temp;
	sstream >> temp;

	return temp;
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
    std::cout << "*****Step 1*****" << std::endl;
    std::cout << "Convert entered password to binary.\n" << std::endl;
		cout_block_state(block);

	unsigned long t1 = 0x80;
	block.push_back(t1);

	//display step 2
	if(show_block_state_add_1)
    std::cout << "\n\n*****Step 2*****" << std::endl;
    std::cout << "Add another 8 bit block with the first bit set to 1\n" << std::endl;
		cout_block_state(block);

	// 7 zero's added, so subtract 7 from k.
	k = k - 7;

	// Find how far away from a 512 bit message
	if (show_distance_from_512bit)
	{
		std::cout << "\nLength of password in bits: " << l << std::endl;
		std::cout << "Number of zeroes to be added: " << k + 7 << std::endl;
	}

	if (show_distance_from_512bit)
		std::cout << "Adding " << k / 8 << " empty eight bit blocks!" << std::endl;

	// Add 8 bit blocks containing zero's
	for(unsigned int i = 0; i < k / 8; i++)
		block.push_back(0x00000000);

	// Add l in the binary form of eight 8 bit blocks.
	std::bitset<64> big_64bit_blob(l);
	if (show_distance_from_512bit)
		std::cout << "Length in a 64 bit binary chunk: \n" << big_64bit_blob << std::endl;

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

	//display step 3
	if (show_padding_results)
	{
    std::cout << "\n\n*****Step 3*****" << std::endl;
    std::cout << "Pad with 0's to 512 bits, with the last 64 bits representing the length of the original password.\n" << std::endl;
		std::cout << "Current 512 bit pre-processed hash in binary:" << std::endl;
			for(unsigned int i = 0; i < block.size(); i=i+4)
				std::cout << i << ": " << show_as_binary(block[i]) << "\t"
				     << i + 1 << ": " << show_as_binary(block[i+1]) << "\t"
				     << i + 2 << ": " << show_as_binary(block[i+2]) << "\t"
				     << i + 3 << ": " << show_as_binary(block[i+3]) << std::endl;
    std::cout << "\n";
		std::cout << "Current 512 bit pre-processed hash in hex:" << std::endl;
		for(unsigned int i = 0; i < block.size(); i=i+4)
			std::cout << i << ": " << "0x" + show_as_hex(block[i]) << "\t"
			     << i + 1 << ": " << "0x" + show_as_hex(block[i+1]) << "\t"
			     << i + 2 << ": " << "0x" + show_as_hex(block[i+2]) << "\t"
			     << i + 3 << ": " << "0x" + show_as_hex(block[i+3]) << std::endl;
	}
	return block;
}

// Changes the n 8 bit segments representing every ASCII character to 32 bit words.
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

//display W(t) values
void show_Wt_values(const std::vector<unsigned long> block)
{
  //initialize values
  unsigned long W[16];

	std::cout << "\nDisplay values for W(t)" << std::endl;
  for(int t = 0; t <= 15; t++)
  {
	 W[t] = block[t] & 0xFFFFFFFF; //perform mask to separate block
	  	 //print out values
	 if (show_Wt)
		 std::cout << "W[" << t << "]: 0x" << show_as_hex(W[t]) << std::endl;
  }
}


////////////////////////////COMPUTE HASH/////////////////
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


  // * Add padding and return the message hash.

std::string SHA1::final()
{
     // Total number of hashed bits
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

     // Append total_bits, split this unsigned long long into two unsigned long int
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

     // Reset for next run
     initialize();

     //get final string
     return result.str();
}

// * Help macros *
//these are defined by the SHA1 algorithm
#define SHA1_ROL(value, bits) (((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))))
#define SHA1_BLK(i) (block[i&15] = SHA1_ROL(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))

// * (R0+R1), R2, R3, R4 are the different operations used in SHA1 *
//these are defined by the SHA1 algorithm
#define SHA1_ROUND_0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + SHA1_BLK(i) + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_2(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0x6ed9eba1 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + SHA1_BLK(i) + 0x8f1bbcdc + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_ROUND_4(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0xca62c1d6 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);

  // *Hash a single 512-bit block. This is the core of the algorithm.

//applies all the rotation functions on a, b, c, d, e
//for sha1 have to define as unsigned long int
void SHA1::transform(unsigned long int block[BLOCK_BYTES])
{
     // Copy hash[] to working vars
		 //initialize
		unsigned long a = hash[0];
		unsigned long b = hash[1];
		unsigned long c = hash[2];
		unsigned long d = hash[3];
		unsigned long e = hash[4];

		//show initial values
		std::cout << "\n\n*****Step 4*****" << std::endl;
		std::cout << "Break down the input to 5 parts, a, b, c, d, e" << std::endl;
		std::cout << "Initialize variables to buffer values, which are predetermined by SHA-1" << std::endl;
		std::cout << "Values initially: " << std::endl;
		std::cout << "a: 0x" << show_as_hex(a) << std::endl;
		std::cout << "b: 0x" << show_as_hex(b) << std::endl;
		std::cout << "c: 0x" << show_as_hex(c) << std::endl;
		std::cout << "d: 0x" << show_as_hex(d) << std::endl;
		std::cout << "e: 0x" << show_as_hex(e) << std::endl;


		std::cout << "\n\n*****Step 5*****" << std::endl;
		std::cout << "Perform 80 rounds of function rotations on the 5 parts" << std::endl;
		// 4 rounds of 20 operations each. Loop unrolled.
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

		std::cout << "\nValues after first 20 iterations: " <<std::endl;
		std::cout << "a: 0x" << show_as_hex(a) << std::endl;
		std::cout << "b: 0x" << show_as_hex(b) << std::endl;
		std::cout << "c: 0x" << show_as_hex(c) << std::endl;
		std::cout << "d: 0x" << show_as_hex(d) << std::endl;
		std::cout << "e: 0x" << show_as_hex(e) << std::endl;

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

		std::cout << "\nValues after 40 iterations: " <<std::endl;
		std::cout << "a: 0x" << show_as_hex(a) << std::endl;
		std::cout << "b: 0x" << show_as_hex(b) << std::endl;
		std::cout << "c: 0x" << show_as_hex(c) << std::endl;
		std::cout << "d: 0x" << show_as_hex(d) << std::endl;
		std::cout << "e: 0x" << show_as_hex(e) << std::endl;

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

		std::cout << "\nValues after 60 iterations: " <<std::endl;
		std::cout << "a: 0x" << show_as_hex(a) << std::endl;
		std::cout << "b: 0x" << show_as_hex(b) << std::endl;
		std::cout << "c: 0x" << show_as_hex(c) << std::endl;
		std::cout << "d: 0x" << show_as_hex(d) << std::endl;
		std::cout << "e: 0x" << show_as_hex(e) << std::endl;

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

		std::cout << "\nValues after all 80 iterations: " <<std::endl;
		std::cout << "a: 0x" << show_as_hex(a) << std::endl;
		std::cout << "b: 0x" << show_as_hex(b) << std::endl;
		std::cout << "c: 0x" << show_as_hex(c) << std::endl;
		std::cout << "d: 0x" << show_as_hex(d) << std::endl;
		std::cout << "e: 0x" << show_as_hex(e) << std::endl;

		// Add the working vars back into hash[]
		hash[0] += a;
		hash[1] += b;
		hash[2] += c;
		hash[3] += d;
		hash[4] += e;

		//display step 6
		std::cout << "\n\n*****Step 6*****" << std::endl;
		std::cout << "Add these values of a, b, c, d, e back to the initial hash buffers" << std::endl;
		std::cout << "The values of hash are as follows:" <<std::endl;
		std::cout << "hash[0]: 0x" << show_as_hex(hash[0]) << std::endl;
		std::cout << "hash[1]: 0x" << show_as_hex(hash[1]) << std::endl;
		std::cout << "hash[2]: 0x" << show_as_hex(hash[2]) << std::endl;
		std::cout << "hash[3]: 0x" << show_as_hex(hash[3]) << std::endl;
		std::cout << "hash[4]: 0x" << show_as_hex(hash[4]) << std::endl;

     // Count the number of transformations
     transforms++;
}

// strandard buffer to block function for sha1 hashing
void SHA1::buffer_to_block(const std::string &buffer, unsigned long int block[BLOCK_BYTES])
{
     // Convert the std::string (byte buffer) to a unsigned long int array (MSB)
     //this will then allow us to use as a string to return the hash
     for (unsigned long int i = 0; i < BLOCK_INTS; i++)
     {
         block[i] = (buffer[4*i+3] & 0xff) | ((buffer[4*i+2] & 0xff)<<8)
                    | ((buffer[4*i+1] & 0xff)<<16) | ((buffer[4*i+0] & 0xff)<<24);
     }
}

//read function for string to string stream
void SHA1::read(std::istream &stream_str, std::string &string, int max)
{
     char str_buf[max];
     // stream_str.read will extract max characters from the string and stroe it in str_buf
     stream_str.read(str_buf, max);
     //string is assigned the first .gcount number of characters in str_buff so how long the string is
     string.assign(str_buf, stream_str.gcount());
}

/*main for debugging
int main(){
	//hash "abc"
	std::cout << "string: \"abc\"" << std::endl;
	std::cout << "hash: " << sha1("abc") << std::endl << std::endl;

	//hash "cat"
	std::cout << "string: \"Password!!12\"" << std::endl;
	std::cout << "hash: " << sha1("Password!!12") << std::endl << std::endl;
}
*/
