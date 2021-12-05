/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.cpp
 * Date Created: 11/19/21
 * File Contents: This file contains the function definitions
 *                for the final project
 **********************************/

 /**************************************************
  General File, Terminal, and Data Settings/Handling
  **************************************************/

#include "../include/funcproj.h"

void echo(int op) {
  struct termios echoInfo;

  tcgetattr(fileno(stdin), &echoInfo);

  if (op) {
    echoInfo.c_lflag |= ICANON;
    echoInfo.c_lflag |= ECHO;
  } else {
    echoInfo.c_lflag &= ~ICANON;
    echoInfo.c_lflag &= ~ECHO;
  }
  tcsetattr(fileno(stdin), TCSANOW, &echoInfo);
}

void printWelcome(void) {
  std::cout << "\033[1;36m\n\t╔═════════════════════════════════════════╗\n"
            <<             "\t   WELCOME   TO   THE  HOUSE   OF   HASH   \n"
            <<             "\t╚═════════════════════════════════════════╝\033[0m\n\n";
}

// This function handles erroneous input with multiple spacebar delimiters
//  in stdin input and trashes chars until it finds a newline
// NOTE: Use after std::cin >> string_name;
// NOTE: Do NOT use after getHiddenPass(); - This will create an unending loop
void getNewline(void) {
  int trash;
  do {
    trash = getchar();
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
    std::cout << "\033[1;91mError:\033[0m Invalid file name \"" << filename << "\"\nPlease enter a valid file name ('q' or 'quit' to exit): ";
    std::cin >> filename;
    getNewline();
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
  std::cout << "Please enter your credentials\n";
  std::cout << "Username: \033[0;36m";
  std::cin >> username;
  getNewline();
  return username;
}

// Check if the given username is in the given hashtable
bool checkUser(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  return (dataHash.find(username) != dataHash.end()) ? true : false;
}

/**********************************
 Hash Function and Tutorial Options
 **********************************/

int getHashType(void) {
  int type;
  // Show Hash Functions Menu
  std::cout << "\033[0m\nList of Available Hash Functions:\n"
    << "\t(1)\t\033[0;36mSHA-256\033[0m - One of the most secure hashing functions and most widely used today\n"
    << "\t(2)\t\033[0;36mSHA-1\033[0m   - A precursor to SHA-256, SHA-1 is compromised against more advanced attacks"
    <<                  "\n\t \t          but is still by some today\n"
    << "\t(3)\t\033[0;36mMD5\033[0m     - MD5 is not considered cryptographically secure anymore but is still used by some today\n"
    << "\t(4)\t\033[0;36mDES\033[0m     - Labeled cryptographically obsolete and broken in 2005"
    <<                  "\n\t \t          by the National Institute of Standards and Technology\n"
    << "\t(5)\t\033[0;36mShifter\033[0m - Not secure at all, just a function that shifts a string to the left by a character\n"
    <<"\nSelect a hash function to encrypt your password: \033[0;36m";

    // Validate menu option
    fscanf(stdin, "%d", &type);
    getNewline();
    while (type > 5 || type < 1) {
      std::cout << "\033[1;91mError:\033[0m Invalid selection. Please choose an option from 1 - 5: \033[0;36m";
      fscanf(stdin, "%d", &type);
      getNewline();
    }
  return type;
}

std::vector<std::string> createPassVect(std::string plainPass) {
  std::vector<std::string> passVect {sha256(plainPass), sha1(plainPass), MD5(plainPass), DES(plainPass), shift_left(plainPass, 1)};
  return passVect;
}

bool tryHashes(std::string password, std::vector<std::string> guessHashes) {
  for (unsigned int hash_iter = 0; hash_iter < guessHashes.size(); ++hash_iter)
    if (password == guessHashes[hash_iter]) return false;
  return true;
}

/*************************************************
 Password Creation, Validation, and Error Handling
 *************************************************/

std::string getHiddenPass(void) {
  std::string newPass;

  // Turn off terminal stdin echo and enter noncanonical/raw mode (immediate input)
  echo(0);

  // Initialize char_in
  int char_in = getchar();
  while (char_in != '\n') {
    // Check to see if input is a backspace
    if (char_in != 127) {
      // Mask newPass input as * in terminal
      std::cout << '*';
      newPass += (char)char_in;
    } else if (newPass.length() > 0) {
      // Pop the last element of newPass and overwrite mask * with ' '
      newPass.pop_back();
      std::cout << '\b' << ' ' << '\b';
      //if (newPass.length() > 1)
    }
    char_in = getchar();
  }

  // Turn on stdin in echo and enter canonical/cooked mode (delimiter enabled input)
  echo(1);
  std::cout << std::endl;
  return newPass;
}

// If the username is found, ask for the corresponding password, only allowing 3 tries
bool askPass(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  unsigned int numTries = 2;
  std::string guess;
  std::vector<std::string> guessHashes;

  // Initialize guess
  std::cout << "\033[0mPassword: \033[0;36m";
  guess = getHiddenPass();
  guessHashes = createPassVect(guess);

  // Repeat attempts if necessary, max of 3 attempts
  while (tryHashes(dataHash[username], guessHashes)) { // **FIX: Should compare to hashAlg(guess) later *Idea: create a vector of strings indexed by dif hashes of guess
    if (numTries == 0) {
      std::cout << "\033[1;31mError: \033[0mNo attempts remain\nAborting Program\n";
      return false;
    } else {
      std::cout << numTries << "\033[0m attempts remain\n";
    }
    std::cout << "\033[0mPassword: \033[0;36m";
    guess = getHiddenPass();
    guessHashes = createPassVect(guess);
    --numTries;
  }

  // Correct guess
  std::cout << "\033[0m\nWelcome, \033[0;36m" << username << "\033[0m!\n";
  return true;
}

// Check if string contains any invalid characters
bool checkInvalidChar(std::string word) {
  for (unsigned int i = 0; i < word.length(); ++i) {
    if (word[i] < 33 || word[i] > 126) {
      return true;
    }
  }
  return false;
}

// Check if string contains at least three of the four: lowercase letters,
// uppercase letters, numbers, special characters
bool checkContainCapitalNumSpecial(std::string word) {
  int numCats = 0;
  // check for lowercase letters
  for (unsigned int i = 0; i < word.length(); ++i) {
    if (word[i] > 96 && word[i] < 123) {
      numCats++;
      break;
    }
  }
  // check for uppercase letters
  for (unsigned int i = 0; i < word.length(); ++i) {
    if (word[i] > 64 && word[i] < 91) {
      numCats++;
      break;
    }
  }
  // check for numbers
  for (unsigned int i = 0; i < word.length(); ++i) {
    if (word[i] > 47 && word[i] < 58) {
      numCats++;
      break;
    }
  }
  // check for special characters
  for (unsigned int i = 0; i < word.length(); ++i) {
    if ((word[i] > 32 && word[i] < 48) || (word[i] > 57 && word[i] < 65) ||
    (word[i] > 90 && word[i] < 97) || (word[i] > 122 && word[i] < 127)) {
      numCats++;
      break;
    }
  }
  if (numCats > 3) {
    return true;
  }
  else {
    return false;
  }
}

// Set new password restrictions
// must be between 8 and 16 characters
// can't contain any invalid characters
// must contain 3 of the 4: lowercase letters, uppercase letters, numbers,
// or special characters
bool checkNewPass(std::string newPass) {
  if (newPass.length() < 8 || newPass.length() > 16 || checkInvalidChar(newPass) || !checkContainCapitalNumSpecial(newPass)) return false;
  else return true;
}

bool confirmMatch(std::string newPass, std::string confirmPass) {
  int numTries = 3;

  // Check if the passwords are the same
  while (newPass != confirmPass) {
    --numTries;
    if (numTries > 0) {
      std::cout << "\n\033[0mError: The passwords you entered do not match\nPlease try again: \033[0;36m";
      confirmPass = getHiddenPass();
    } else {
      std::cout << "\n\033[0mYou may have had a typo. Please enter a new password\n";
      return true; // Return true to repeat outer loop
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
  bool valid = false;

  do {
    // Initialize first password entry
    std::cout << "\033[0mPlease create a password: \033[0;36m";
    newPass = getHiddenPass();

    // Check if new password is valid
    while (!valid) {
      valid = checkNewPass(newPass);
      if (!valid) {
        std::cout << "\033[1;91mError: \033[mYour password must have"
          << "\n\t(1) 8 - 16 characters,"
          << "\n\t(2) an uppercase letter,"
          << "\n\t(3) a lowercase letter,"
          << "\n\t(4) a number,"
          << "\n\t(5) and a special character (i.e. @, #, $, etc.)"
          << "\n\tIf you would like to quit, enter 'q' or 'quit'\n"
          << "\nPlease choose a new password: \033[0;36m";
          newPass = getHiddenPass();

        // Check quit signal
        if (newPass == "q" || newPass == "quit") {
          std::cout << "\033[0mQuitting Program\n";
          return false;
        }
      }
    }

    // Iniitialize confirmation password
    std::cout << "\033[0mConfirm your password: \033[0;36m";
    confirmPass = getHiddenPass();

    // Give user the option to re-enter their password or quit
    repeat = confirmMatch(newPass, confirmPass);
    if (repeat)
      std::cout << "\033[0mIf you would like to quit, enter 'q' or 'quit' as a new password\n\033[0;36m";
  } while (repeat);

  // Create vector of hashed passwords and get hash type
  std::vector<std::string> passVect = createPassVect(newPass);
  int hashType = getHashType() - 1;

  dataHash.insert({username, passVect[hashType]}); // **FIX: Should insert the username with the HASHED password
  return true;
}

// Append the database file wiht the given username and password
void saveFile(std::unordered_map<std::string, std::string>& dataHash, std::string username) {
  std::ofstream sFile ("data/userPassTest.txt", std::fstream::app); // **FIX: Running Test File: Change Later // fstream::app appends rather than overwriting
  sFile << "\n" << username + " " + dataHash[username];
  std::cout << "\033[0mYour username and password have been saved\n";
}
