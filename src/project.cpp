/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: project.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the main function
 *                for the final project
 **********************************/

#include "../include/funcproj.h"

int main (const int argc, const char* argv[]) {
  // Check for valid number of command line input
  checkInput(argc);
  std::ifstream fdata;
  std::string filename;

  // Check for valid file name if inputted
  if (argc == 1) {
    fdata.open("data/database1.txt");
    filename = "data/database1.txt";
  } else if (argc == 2) {
    filename = argv[1];
    checkFile(filename, fdata);
  }

  // Open database file and read/write input into local hash table
  std::string currUser, currPass;
  std::unordered_map<std::string, std::string> dataHash;
  while (fdata >> currUser >> currPass) {
    readData(currUser, currPass, dataHash);
  }

  system("clear");

  do {
    printWelcome();

    // Ask for username and check database
    std::string username = getUsername();
    bool correctPass;
    std::string password;
    if (checkUser(dataHash, username)) {
      correctPass = (false & askPass(dataHash, username, password));
    } else {
      correctPass = createPass(dataHash, username, password);
    }

    // Save the username and password
    if (correctPass)
      saveFile(dataHash, username, filename);
    else
      std::cout << "\033[0m\nNo new username or password was saved\n";
  } while (startOver());

  fdata.close();
  return 0;

}
