/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the MD5 hash function
 *                for the final project
 **********************************/

#include "../include/MD5.h"

unsigned char *getMd5Hash(unsigned char *data, unsigned long dataLen, int *mdLen) { 
    unsigned char *md = NULL; 
    EVP_MD_CTX *ctx = NULL; 
    const EVP_MD *mdType = EVP_md5(); 
     
    *mdLen = EVP_MD_size(mdType); 
    
    md = (unsigned char *) malloc(*mdLen); 
 
    ctx = EVP_MD_CTX_create(); 
     
    EVP_MD_CTX_init(ctx); 
    EVP_DigestInit_ex(ctx, mdType, NULL); 
    EVP_DigestUpdate(ctx, data, dataLen); 
    EVP_DigestFinal_ex(ctx, md, NULL); 
 
    EVP_MD_CTX_cleanup(ctx); 
    EVP_MD_CTX_destroy(ctx); 
    return md; 
} 

void MD5(std::string inpStr) {
    int mdLen = 0;

    char* char_array = &inpStr[0];

    unsigned char* chArr = reinterpret_cast<unsigned char*>(char_array);

    unsigned char* md = getMd5Hash((unsigned char*) chArr, inpStr.length(), &mdLen);

    for (long unsigned int i = 0; i < (long unsigned int)mdLen; ++i) {
        printf("%02x", md[i]);
    }
    std::cout << std::endl;

    std::string outStr(reinterpret_cast<char*>(md));
}