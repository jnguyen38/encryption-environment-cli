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

#include "project.h"
#include "DES.h"
#include "SHA1.h"
#include "SHA256.h"
#include "MD5.h"

/**************************************************
 General File, Terminal, and Data Settings/Handling
 **************************************************/

void echo(int);

void printWelcome(void);

void getNewline(void);

void checkInput(const int);

void checkFile(std::string&, std::ifstream&);

void readData(std::string, std::string, std::unordered_map<std::string, std::string>&);

std::string getUsername(void);

bool checkUser(std::unordered_map<std::string, std::string>&, std::string);

/**********************************
 Hash Function and Tutorial Options
 **********************************/

int getHashType(void);

std::string getPassHash(int, std::string);

int determineHash(std::string);

/*************************************************
 Password Creation, Validation, and Error Handling
 *************************************************/

std::string getHiddenPass(void);

bool askPass(std::unordered_map<std::string, std::string>&, std::string, std::string&);

bool checkInvalidChar(std::string);

bool checkContainCapitalNumSpecial(std::string);

bool checkNewPass(std::string);

bool confirmMatch(std::string, std::string);

bool createPass(std::unordered_map<std::string, std::string>&, std::string, std::string&);

void saveFile(std::unordered_map<std::string, std::string>&, std::string, std::string);

bool startOver(void);

#endif
