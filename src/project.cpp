/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the main function
 *                for the final project
 **********************************/

#include "../include/funcproj.h"

int main (const int argc, const char* argv[]) {
  // Check for valid number of command line input
  checkInput(argc);

  // Check for valid file name if inputted
  if (argc == 2) {
    std::ifstream fin;
    std::string filename = argv[1];
    checkFile(filename, fin);
  }

  // Open database file and read/write input into local hash table
  std::ifstream fdata ("../data/userPassTest.txt"); // Running Test File: Change later
  std::string currUser, currPass;
  std::unordered_map<std::string, std::string> database;
  while (fdata >> currUser >> currPass) {
    readData(currUser, currPass, database);
  }

  return 0;
}
