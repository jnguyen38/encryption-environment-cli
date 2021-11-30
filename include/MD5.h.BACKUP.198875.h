/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the MD5 header file
 *                for the final project
 **********************************/

#ifndef MD5_H
#define MD5_H

<<<<<<< HEAD
#include "project.h"
=======
#include <openssl/evp.h> 
#include <string>
#include <iostream>
#include <cstdlib>
#include <string.h>
>>>>>>> 7cc5a9dd0b331618e56d238c860c9a22f87ac1c2

unsigned char *getMd5Hash(unsigned char *, unsigned long, int *);
void MD5(std::string inpStr);

#endif
