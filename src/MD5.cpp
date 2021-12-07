/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the MD5 hash function
 *                for the final project
 **********************************/

#include "../include/MD5.h"

// These vars will contain the hash and are stored globally for fast access
uint32_t MD5h0, MD5h1, MD5h2, MD5h3;

// C method of computing MD5 - used in C++ implementation
void C_MD5(uint8_t *initial_msg, size_t initial_len) {

    system("clear");
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating

    // r specifies the per-round shift amounts
 
    const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // Use binary integer part of the sines of integers (in radians) as constants// Initialize variables:
    const uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    // initial buffer values
    MD5h0 = 0x67452301;
    MD5h1 = 0xefcdab89;
    MD5h2 = 0x98badcfe;
    MD5h3 = 0x10325476;
    
    // print password and initial bitlength
    
    /* ***** 
    Step 1: Pad string to length congruent to 448 (mod 512)
    ***** */

    printf("\033[0;31m\t\t***** Step 1 *****\n\033[0m");
    printf("Pad input string to bitlength congruent to 448 (mod 512)\n\n");
    printf("Password initial bitlength: %lu bits\n", 8 * initial_len);

    // Pre-processing: adding a single 1 bit
    //append "1" bit to message    
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */
 
    // Pre-processing: padding with zeros
    //append "0" bit until message length in bit ≡ 448 (mod 512)
    //append length mod (2 pow 64) to message
 
    size_t new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

    // print step of appending bits until length congruent to 448 (mod 512)
    printf("Now, append bits until message bitlength is congruent to 448 (mod 512):\n");
    printf("\tBits added: %lu\n", (long unsigned int) (8 * (new_len - initial_len)));
    printf("Password's new bitlength after appending 0's: %lu bits\n\n", 8 * new_len);

    /* ***** 
    Step 2: allocate 64 more bits for padding 
    ***** */

    printf("\033[0;31m\t\t***** Step 2 *****\n\033[0m");
    printf("Now, 64 more bits are allocated to achieve bitlength multiple of 512.\n");
    printf("\tBits added: 64\n");

    msg = static_cast<uint8_t *>(malloc(new_len + 64));
    printf("Password's new bitlength after appending 0's: %lu bits\n\n", 8 * new_len + 64);
    // memcpy message from initial_msg to initial_len
    memcpy(msg, initial_msg, initial_len);
    
    // write the "1" bit
    msg[initial_len] = 128; 

    
    // append length in bits at end of the buffer
    uint32_t bits_len = 8 * (uint32_t)initial_len;
    memcpy(msg + new_len, &bits_len, 4);           
 
    /* *****
    Step 3: Process the message in successive 512-bit chunks:
    ***** */

    printf("\033[0;31m\t\t***** Step 3 *****\n\033[0m");
    printf("Break password into successive 512-bit chunks - repeated for each such chunk\n");
    //for each 512-bit chunk of message:
    size_t offset;
    for (offset = 0; offset < new_len; offset += (512/8)) {
        // print buffer values (changes to buffer values only happen for chunks greater than 512 bits)
        printf("\nPadded buffer values for 512-bit chunk #%lu:\n\tMD5h0: 0x%8.8x MD5h1: 0x%8.8x MD5h2: 0x%8.8x MD5h3: 0x%8.8x\n\n", 
                ((offset + 1) / (512/8)) + 1, MD5h0, MD5h1, MD5h2, MD5h3);
        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *) (msg + offset);
 
        // Initialize hash value for this chunk:
        uint32_t a = MD5h0;
        uint32_t b = MD5h1;
        uint32_t c = MD5h2;
        uint32_t d = MD5h3;

        /* ***** 
        Step 4 - Apply auxiliary functions
        ***** */

        printf("\033[0;31m\t\t\t***** Step 4 ***** (chunk #%lu)\n\033[0m", ((offset + 1) / (512/8)) + 1);
        printf("\t\tApply auxiliary functions and rotations to each chunk\n");
        for (uint32_t i = 0; i < 64; i++) {      
            // apply auxiliary functions 
            uint32_t f, g;

            if (i < 16) {
                if (i % 16 == 0)
                    printf("\tApplying auxiliary function F:\n");
                f = (b & c) | ((~b) & d);   // Function F
                g = i;                          // predefined method to get element
            } else if (i < 32) {
                if (i % 16 == 0)
                    printf("\tApplying auxiliary function G:\n");
                f = (d & b) | ((~d) & c);   // Function G
                g = (5*i + 1) % 16;             // predefined method to get element
            } else if (i < 48) {
                if (i % 16 == 0)
                    printf("\tApplying auxiliary function H:\n");
                f = b ^ c ^ d;              // Function H
                g = (3*i + 5) % 16;           // predefined method to get element
            } else {
                if (i % 16 == 0)
                    printf("\tApplying auxiliary function I:\n");
                f = c ^ (b | (~d));         // Function I
                g = (7*i) % 16;               // predefined method to get element
            }
            uint32_t temp = d;
            d = c;
            c = b;
            // apply left rotation given parameters specified by a, f, k[i], w[g], and r[i]
            printf("\t\trotate left(%x + %x + %x + %x, %d)\n", a, f, k[i], w[g], r[i]);
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }
 
        // update buffer values hash to result so far
        MD5h0 += a;
        MD5h1 += b;
        MD5h2 += c;
        MD5h3 += d;

    }
    
    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
    uint8_t *p;
    /* *****
    Step 5 - Concatenate final buffer values to achieve final MD5 hash
    ***** */
   printf("\n\033[0;31m\t\t***** Step 5 *****\n\033[0m");
   printf("Concatenate final buffer blocks to achieve final MD5 hash\n\n");
    printf("32 bit block 1: ");
    p=(uint8_t *)&MD5h0;
    printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);

    printf("32 bit block 2: ");
    p=(uint8_t *)&MD5h1;
    printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);

    printf("32 bit block 3: ");
    p=(uint8_t *)&MD5h2;
    printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);

    printf("32 bit block 4: ");
    p=(uint8_t *)&MD5h3;
    printf("%2.2x%2.2x%2.2x%2.2x\n", p[0], p[1], p[2], p[3]);

    // free msg
    free(msg);
 
}
// C++ MD5 implementation which returns std::string MD5 hash of the input std::string
std::string MD5(std::string str) {

    // output string
    std::string out;

    // convert std::string to c string
    const char* msg = str.c_str();

    // get length of string to pass to function
    size_t len = str.length();

    // compute md5 of str
    C_MD5((uint8_t *)msg, len);

    // char digest[16] := MD5h0, append MD5h1, append MD5h2, append MD5h3
    uint8_t *p;

    // allocate digests for each 32 bit (8 byte) block of words
    char* dig0 = (char *)malloc(8);
    char* dig1 = (char *)malloc(8);
    char* dig2 = (char *)malloc(8);
    char* dig3 = (char *)malloc(8);

    // point digest to MD5h0, print hex values (8 characters) to dig0, append to out string, and free dig0
    p = (uint8_t *)&MD5h0;
    sprintf(dig0, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
    out.append(dig0);
    free(dig0);

    // point digest to MD5h1, print hex values (8 characters) to dig1, append to out string, and free dig1
    p = (uint8_t *)&MD5h1;
    sprintf(dig1, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
    out.append(dig1);
    free(dig1);

    // point digest to MD5h2, print hex values (8 characters) to dig2, append to out string, and free dig2 
    p = (uint8_t *)&MD5h2;
    sprintf(dig2, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
    out.append(dig2);
    free(dig2);

    // point digest to MD5h3, print hex values (8 characters) to dig3, append to out string, and free dig3
    p = (uint8_t *)&MD5h3;
    sprintf(dig3, "%2.2x%2.2x%2.2x%2.2x", p[0], p[1], p[2], p[3]);
    out.append(dig3);
    free(dig3);

    std::cout << "Final MD5 hash: " << out << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    // return output string of MD5 hash
    return out;
}