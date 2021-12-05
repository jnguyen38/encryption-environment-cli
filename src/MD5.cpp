/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the MD5 hash function
 *                for the final project
 **********************************/

#include "../include/MD5.h"
// creates unsigned char* message digest to be used in MD5 function
unsigned char *getMd5Hash(unsigned char *data, unsigned long dataLen, int *mdLen) {
    // declare initial messsage digest variable and set to NULL
    unsigned char *md = NULL;

    // declare initial message digest context variable and set to NULL
    EVP_MD_CTX *ctx = NULL;

    // create and declare the message digest type as md5
    const EVP_MD *mdType = EVP_md5();

    // retrieve length of message digest from md5 type (128 bits for MD5)
    *mdLen = EVP_MD_size(mdType);

    // allocate memory for message digest
    md = (unsigned char *) malloc(*mdLen);

    // assign context
    ctx = EVP_MD_CTX_create();

    // initialize context variable
    EVP_MD_CTX_init(ctx);

    // sets up digest context with ctx
    EVP_DigestInit_ex(ctx, mdType, NULL);

    // hash dataLen number of bytes at &data in digest context ctx
    EVP_DigestUpdate(ctx, data, dataLen);

    // retrieve digest value from ctx and place in md
    EVP_DigestFinal_ex(ctx, md, NULL);

    // cleanup and destroy digest ontext ctx
    EVP_MD_CTX_cleanup(ctx);
    EVP_MD_CTX_destroy(ctx);

    // return message digest as ungined char*
    return md;
}

// MD5 hash of std::string input
std::string MD5(std::string inpStr) {
    // message digest length variable pre-set to 0
    int mdLen = 0;

    // create char* array with base address of inpStr
    char* char_array = &inpStr[0];

    // create unsigned char* array from char_array (to be inputted to getMd5Hash)
    unsigned char* chArr = reinterpret_cast<unsigned char*>(char_array);

    // get actual hash of the input string
    unsigned char* md = getMd5Hash((unsigned char*) chArr, inpStr.length(), &mdLen);

    // print hex address of message digest (the actual MD5 hash) with width of 2 at a time
  /*  for (long unsigned int i = 0; i < (long unsigned int)mdLen; ++i) {
        printf("%02x", md[i]);
    }

    std::cout << std::endl;*/
  std::string sName(reinterpret_cast<char*>(md));

	// free md5
	free(md);

  return sName;
}
