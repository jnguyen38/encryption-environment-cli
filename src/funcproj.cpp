/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the function definitions
 *                for the final project
 **********************************/

#include "../include/funcproj.h"

void checkInput(const int argc) {
  if (argc > 2) {
    std::cout << "Error: Invalid number of inputs\n";
    exit(-1);
  }
}

void checkFile(std::string& filename, std::ifstream& fin) {
  fin.open(filename);
  while (!fin) {
    std::cout << "Error: Invalid file name \"" << filename << "\"\nPlease enter a valid file name: ";
    std::cin >> filename;
    if (filename == "q" || filename == "quit") exit(-1);
    fin.open(filename);
  }
}
void readData(std::string currUser, std::string currPass, std::unordered_map<std::string, std::string>& database) {
  database.insert({currUser, currPass});
  std::cout << "Username: " << currUser << "\nPassword: " << database[currUser] << std::endl;
}

bool checkUser(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  return (dataHash.find(username) != dataHash.end()) ? true : false;
}

bool askPass(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  unsigned int numTries = 3;
  std::string guess;

  // Initialize guess
  std::cout << "Please enter your password: ";
  std::cin >> guess;

  // Repeat attempts if necessary, max of 3 attempts
  while (guess != dataHash[username]) { // Should compare to hashAlg(guess) later *Idea: create a vector of strings indexed by dif hashes of guess
    if (numTries == 0) {
      std::cout << "No attempts remain\n";
      return false;
    } else {
      std::cout << numTries << " attempts remain\n";
    }
    std::cout << "Please enter your password: ";
    std::cin >> guess;
  }

  // Correct guess
  std::cout << "Welcome, " << username << "!\n";
  return true;
}

bool createPass(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  std::string guess;
  std::string firstPass;
  std::string secondPass;
  bool enterFirst = 1;
  bool enterSecond = 1;
  unsigned int numTries = 3;

  // Initialize first password entry
  std:: cout << "Please enter a password: ";
  std::cin >>
}
