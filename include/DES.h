/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the DES header file
 *                for the final project
 **********************************/

#ifndef DES_H
#define DES_H

#include "project.h"
#define KEY "0123456789ABCDEF"

// std::reverse hardcoded
template <class BidirectionalIterator>
void reverse (BidirectionalIterator first, BidirectionalIterator last) {
  while ((first!=last)&&(first!=--last)) {
    std::iter_swap (first,last);
    ++first;
  }
}

std::string ascii2hex(std::string);

std::string pad(std::string);

std::vector<std::string> separate(std::string);

std::string hex2bin(std::string);

std::string bin2hex(std::string);

std::vector<std::string> getKeys(void);

std::string permute(std::string, int*, int);

std::string shift_left(std::string, int);

std::string binxor(std::string, std::string);

std::vector<std::string> pt_transfer(std::string);

std::string DES(std::string pt);

#endif
