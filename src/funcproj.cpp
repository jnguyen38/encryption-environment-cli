/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the function definitions
 *                for the final project
 **********************************/

#include "../include/funcproj.h"

void getNewline(void) {
  char trash;
  do {
    std::cin >> trash;
  } while (trash != '\n');
}

// Check if command line input is a valid number of inputs
void checkInput(const int argc) {
  if (argc > 2) {
    std::cout << "Error: Invalid number of inputs\n";
    exit(-1);
  }
}

// Check if the file is valid and repeat until the user enters a valid file or q/quit
void checkFile(std::string& filename, std::ifstream& fin) {
  fin.open(filename);
  while (!fin) {
    std::cout << "Error: Invalid file name \"" << filename << "\"\nPlease enter a valid file name ('q' or 'quit' to exit): ";
    std::cin >> filename;
    // getNewline();
    if (filename == "q" || filename == "quit") exit(-1);
    fin.open(filename);
  }
}

// Read the username and password and insert into the hash table
void readData(std::string currUser, std::string currPass, std::unordered_map<std::string, std::string>& database) {
  database.insert({currUser, currPass}); // Should insert the hashed currPass
}

std::string getUsername(void) {
  std::string username;
  std::cout << "Username: ";
  std::cin >> username;
  // getNewline();
  return username;
}

// Check if the given username is in the given hashtable
bool checkUser(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  return (dataHash.find(username) != dataHash.end()) ? true : false;
}

// If the username is found, ask for the corresponding password, only allowing 3 tries
bool askPass(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  unsigned int numTries = 3;
  std::string guess;

  // Initialize guess
  std::cout << "Password: ";
  std::cin >> guess;
  // getNewline();
  // Repeat attempts if necessary, max of 3 attempts
  while (guess != dataHash[username]) { // **FIX: Should compare to hashAlg(guess) later *Idea: create a vector of strings indexed by dif hashes of guess
    if (numTries == 0) {
      std::cout << "No attempts remain\n";
      return false;
    } else {
      std::cout << numTries << " attempts remain\n";
    }
    std::cout << "Password: ";
    std::cin >> guess;
    // getNewline();
    --numTries;
  }

  // Correct guess
  std::cout << "\nWelcome, " << username << "!\n";
  return true;
}

/**FIX: Set new password restrictions (ie 8 characters or more, numbers, special characters)
bool getNewPass() {

}
*/

bool confirmMatch(std::string newPass, std::string confirmPass) {
  int numTries = 3;

  // Check if the passwords are the same
  while (newPass != confirmPass) {
    --numTries;
    if (numTries > 0) {
      std::cout << "\nError: The passwords you entered do not match\nPlease try again: ";
      std::cin >> confirmPass;
      // getNewline();
    } else {
      std::cout << "\nYou may have had a typo. If you want to try a new password, type 'stop'\nOtherwise, please try again: ";
      std::cin >> confirmPass;
      // getNewline();
      if (confirmPass == "stop") return true; // Return true to repeat outer loop
    }
  }

  return false; // Return false to break outer loop
}

// If the username is not found, create a new password, confirming it
bool createPass(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  std::string guess;
  std::string newPass;  // Add hashed versions of passwords
  std::string confirmPass;
  std::string option;
  bool repeat = false;

  do {
    // Initialize first password entry
    std::cout << "Please create a password: ";
    std::cin >> newPass;
    // getNewline();

    // Iniitialize confirmation password
    std::cout << "Confirm your password: ";
    std::cin >> confirmPass;
    // getNewline();

    // Give user the option to re-enter their password or quit
    repeat = confirmMatch(newPass, confirmPass);
    if (repeat) {
      std::cout << "If you would like to quit, enter 'q' or 'quit'\Enter any other key to try a new password\n";
      std::cin >> option;
      // getNewline();
      if (option == "q" || option == "quit") return false;
    }
  } while (repeat);

  dataHash.insert({username, newPass}); // **FIX: Should insert the username with the HASHED password
  return true;
}

// Append the database file wiht the given username and password
void saveFile(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  std::ofstream sFile ("userPassTest.txt", std::fstream::app); // **FIX: Running Test File: Change Later // fstream::app appends rather than overwriting
  sFile << username + " " + dataHash[username];
  std::cout << "Your username and password have been saved\n";
}
