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

 #include <string>
 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <iomanip>

#ifndef SHA1_H
#define SHA1_H

#include "project.h"

class SHA1{


public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();

private:
    // two variables, type static so the static functions can access these elements
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

//function that allows user to input string and returns final hashed string
std::string sha1(const std::string &string);

#endif
