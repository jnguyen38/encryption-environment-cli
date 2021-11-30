/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the main function
 *                for the final project
 **********************************/

#include "../include/funcproj.h"
#include "../include/MD5.h"

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
  std::ifstream fdata ("userPassTest.txt"); // Running Test File: Change later
  std::string currUser, currPass;
  std::unordered_map<std::string, std::string> dataHash;
  while (fdata >> currUser >> currPass) {
    readData(currUser, currPass, dataHash);
  }

  // Ask for username and check database
  std::string username = getUsername();
  bool correctPass;
  if (checkUser(dataHash, username)) {
    correctPass = (false & askPass(dataHash, username));
  } else {
    correctPass = createPass(dataHash, username);
  }

  // Save the username and password
  if (correctPass)
    saveFile(dataHash, username);
  else
    std::cout << "No new username or password was saved\n";

  return 0;

  std::string str = "notre dame";
  MD5(str);
}
