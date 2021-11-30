/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the main header file
 *                for the final project
 **********************************/

#ifndef FUNCPROJ_H
#define FUNCPROJ_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <unordered_map>

void getNewline(void);
void checkInput(const int);
void checkFile(std::string&, std::ifstream&);
void readData(std::string, std::string, std::unordered_map<std::string, std::string>&);
std::string getUsername(void);
bool checkUser(std::unordered_map<std::string, std::string>&, std::string);
bool askPass(std::unordered_map<std::string, std::string>&, std::string);
bool confirmMatch(std::string, std::string);
bool createPass(std::unordered_map<std::string, std::string>&, std::string);
void saveFile(std::unordered_map<std::string, std::string>&, std::string);

#endif
