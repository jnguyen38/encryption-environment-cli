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

// std::reverse hardcoded
template <class BidirectionalIterator>
void reverse (BidirectionalIterator first, BidirectionalIterator last) {
  while ((first!=last)&&(first!=--last)) {
    std::iter_swap (first,last);
    ++first;
  }
}

std::string hex2bin(std::string);

std::string bin2hex(std::string);

std::string permute(std::string, int*, int);

#endif
