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
 https://github.com/Breathleas/AES-DES-3DES-MD5-RC4-SHA1-SHA256-BASE64/blob/master/SHA1/sha1.hpp
 */
 #include <string>
 //#include <string.h>
// #include <cstdint>
 #include <iostream>
 #include <fstream>
 //#include <unordered_map>
 #include <sstream>
 #include <iomanip>

#ifndef SHA1_H
#define SHA1_H

<<<<<<< HEAD
#include "project.h"
=======
class SHA1{
>>>>>>> 7cc5a9dd0b331618e56d238c860c9a22f87ac1c2

public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();

private:
    static const unsigned int BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    unsigned long int hash[5];
    std::string buffer;
    unsigned long long transforms;

    void initialize();
    void transform(unsigned long int block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string &buffer, unsigned long int block[BLOCK_BYTES]);
    static void read(std::istream &is, std::string &s, int max);
};

std::string sha1(const std::string &string);

/*
public:
  //same as initialize
  SHA1();
  void update(const char*);
	void final(char*);
  void hash_word(const char*, char*);
  void buffer_to_block(const std::string &, unsigned int );
  // initialize variables
  void initialize();

private:
  //initialize array for hash
  uint32_t hash[5];
  //initialize string for buffer
  std::size_t buffer_size;
  //initialize uint32_t for transforms
  unsigned int transforms;
  char buffer[64];
}; */

#endif
