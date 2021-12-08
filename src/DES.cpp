/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the DES hash function
 *                for the final project
 **********************************/

#include "../include/DES.h"

// Print Step Number for the tutorial
void step(int stepNum) {
  std::cout << "\n\033[2;36mSTEP " << stepNum << "\033[0m\n";
}

// Standard ASCII to Hex Conversion
std::string ascii2hex(std::string asciistr) {
  std::stringstream hexstream;
  std::string hexstr;
  long unsigned int len = asciistr.length();

  // Convert string to char array and define start and end pointers
  char asciichars[len + 1];
  char *start = asciichars, *end = asciichars + len;
  strcpy(asciichars, asciistr.c_str());

  // Stream unsigned value at start iterator using stream manipulators
  while (start < end) {
    hexstream << std::uppercase << std::hex << unsigned(*start);
    start += 1;
  }

  // Stream hexstream into hexstr
  hexstream >> hexstr;

  return hexstr;
}

// Standard Hex to Binary Conversion
std::string hex2bin(std::string hexstr) {
  std::string binstr = "";
  std::unordered_map<char, std::string> converter;

  converter['0'] = "0000";
  converter['1'] = "0001";
  converter['2'] = "0010";
  converter['3'] = "0011";
  converter['4'] = "0100";
  converter['5'] = "0101";
  converter['6'] = "0110";
  converter['7'] = "0111";
  converter['8'] = "1000";
  converter['9'] = "1001";
  converter['A'] = "1010";
  converter['B'] = "1011";
  converter['C'] = "1100";
  converter['D'] = "1101";
  converter['E'] = "1110";
  converter['F'] = "1111";

  for (unsigned int i = 0; i < hexstr.size(); i++) {
    binstr += converter[hexstr[i]];
  }
  return binstr;
}

// Standard Binary to Hex Conversion
std::string bin2hex(std::string binstr) {
  std::string hexstr = "", tempBin = "";
  std::unordered_map<std::string, std::string> converter;

  converter["0000"] = "0";
  converter["0001"] = "1";
  converter["0010"] = "2";
  converter["0011"] = "3";
  converter["0100"] = "4";
  converter["0101"] = "5";
  converter["0110"] = "6";
  converter["0111"] = "7";
  converter["1000"] = "8";
  converter["1001"] = "9";
  converter["1010"] = "A";
  converter["1011"] = "B";
  converter["1100"] = "C";
  converter["1101"] = "D";
  converter["1110"] = "E";
  converter["1111"] = "F";

  // Return the string in hex by creating a temporary four bit binary input for the hashmap
  for (unsigned int i = 0; i < binstr.length(); i += 4) {
    tempBin = "";
    tempBin += binstr[i];
    tempBin += binstr[i + 1];
    tempBin += binstr[i + 2];
    tempBin += binstr[i + 3];
    hexstr += converter[tempBin];
  }
  return hexstr;
}

// Pad 0s to the end of the hex string to meet the 64-bit block requirement
std::string pad(std::string hexstr) {
  long unsigned int len = hexstr.length(), size = 64, numpads = (len % size == 0) ? 0 : size - (len % size);

  for (long unsigned int zeropad = 0; zeropad < numpads; ++zeropad)
    hexstr += '0';

  return hexstr;
}

std::vector<std::string> separate(std::string binstr) {
    std::vector<std::string> separatedstr;
    long unsigned int len = binstr.length(), size = 64, numblocks = len / size, start;

    for (long unsigned int block = 0; block < numblocks; ++block) {
      start = block * size;
      separatedstr.push_back(binstr.substr(start, size));
    }

    return separatedstr;
}

// Permutes a string based on a given permutation array
std::string permute(std::string str, int* permArr, int len) {
  std::string per = "";
  for (int i = 0; i < len; i++)
    per += str[permArr[i] - 1];
  return per;
}

// Shifts a 28 bit long string to the left cyclically by 'shifts' amount of times
std::string shift_left(std::string str, int shifts) {
  std::string temp = "";
  for (int i = 0; i < shifts; i++) {
    for (unsigned int j = 1; j < str.length(); j++)
      temp += str[j];
    temp += str[0];
    str = temp;
    temp = "";
  }
  return str;
}

// Takes in two BINARY strings and xors them, returning the result
std::string binxor(std::string str1, std::string str2) {
  std::string out = "";
  for (unsigned int i = 0; i < str1.size(); i++)
    out += (str1[i] == str2[i]) ? "0" : "1";
  return out;
}

std::vector<std::string> pt_transfer(std::string pt) {
  std::vector<std::string> pt_vect = separate(pad(hex2bin(ascii2hex(pt))));
  return pt_vect;
}

std::vector<std::string> getKeys(void) {
  std::string key, left, right, combine, RoundKey, cipher;
  std::vector<std::string> pt_vect, pt_vect_test, rkb;

  // Number of bit shifts
  int shift_table[16] = {
    1, 1, 2, 2,
    2, 2, 2, 2,
    1, 2, 2, 2,
    2, 2, 2, 1
  };

  // Key- Compression Table
  int key_comp[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
  };


  // Parity bit drop table
  int keyp[56] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4
	};

  key = KEY;

  // Hex to binary
  key = hex2bin(key);

  // getting 56 bit key from 64 bit using the parity bits
  key = permute(key, keyp, 56); // key without parity

  // Splitting
  left = key.substr(0, 28);
  right = key.substr(28, 28);

  for (int i = 0; i < 16; i++) {
    // Shifting
    left = shift_left(left, shift_table[i]);
    right = shift_left(right, shift_table[i]);

    // Combining
    combine = left + right;

    // Key Compression
    RoundKey = permute(combine, key_comp, 48);

    rkb.push_back(RoundKey);
  }
  return rkb;
}

std::string DES(std::string pt) {
  // Tutorial Output
  std::cout << "\033[2;31mGENERAL INFO\n\033[0m"
    << "The Data Encryption Standard (DES) Algorithm is a block-cipher encryption algroithm\n"
    << "that was developed by IBM in the 1970's. \"Block-Cipher\" means that the algorithm\n"
    << "takes \"blocks\" of 64 bit binary chunks and encrypts one chunk at a time. \"Encryption\"\n"
    << "means that the algorithm can be reversed given a key that is entered at the time of\n"
    << "encryption. For the sake of simplicity, the key in this algorithm is pre-set to the hex\n"
    << "string \"\033[0;36m0123456789ABCDEF\033[0m\". After a computer was able to crack the DES\n"
    << "encryption in 1997 via a brute force attack, the algorithm receded from popular use in\n"
    << "industry. In 2005, it was finally declared \"obsolete and cryptographically broken\" by the\n"
    << "National Institute of Standards and Technology (NIST).\n";

  step(1);
  std::cout << "For the first step in DES hashing, we take your password \"\033[0;36m"
    << pt << "\033[0m\" and convert it to binary:\n\t"
    << hex2bin(ascii2hex(pt));

  step(2);
  std::cout << "Next, since ";

  // Hexadecimal to binary
  std::vector<std::string> pt_vect = pt_transfer(pt), rkb, rk;
  std::string cipher = "", combine, left, right, right_expanded, x, op;

  // Initial Permutation Table (Standard IP Table Values for DES found online)
  int initial_perm[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
  };

  // Expansion D-box Table (Standard D-box Table Values for DES found online)
  int exp_d[48] = {
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
  };

  // Substitution Value Table (Standard S-box Values for DES found online)
  int s[8][4][16] = {
    // Substitution box 1
    { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
      0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
      4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
      15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
    // Substitution box 2
    { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
      3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
      0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
      13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
    // Substitution box 3
    { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
      13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
      13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
      1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
    // Substitution box 4
    { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
      13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
      10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
      3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
    // Substitution box 5
    { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
      14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
      4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
      11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
    // Substitution box 6
    { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
      10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
      9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
      4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
    // Substitution box 7
    { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
      13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
      1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
      6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
    // Substitution box 8
    { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
      1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
      7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
      2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
  };

  // Straight Permutation Table
  int per[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
  };

  // Final Permutation Table
  int final_perm[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
  };

  rkb = getKeys();
  for (int key_iter = 0; key_iter < 16; ++key_iter) {
    rk.push_back(bin2hex(rkb[key_iter]));
  }

  for (unsigned int pt_iter = 0; pt_iter < pt_vect.size(); ++pt_iter) {
    //std::cout << "\n\t\033[1;96mBLOCK " << pt_iter + 1 << " ENCRYPTION\n\033[0m]";

    // Initial Permutation
    pt = permute(pt_vect[pt_iter], initial_perm, 64);
    //std::cout << "\nAfter initial permutation: " << bin2hex(pt) << std::endl;

    // Splitting
    left = pt.substr(0, 32);
    right = pt.substr(32, 32);
    //std::cout << "After splitting: L0= " << bin2hex(left) << " R0= " << bin2hex(right) << std::endl;
    //std::cout << std::endl;
    for (int i = 0; i < 16; i++) {
      // Expansion D-box
      right_expanded = permute(right, exp_d, 48);

      // XOR RoundKey[i] and right_expanded
      x = binxor(rkb[i], right_expanded);

      // S-boxes
      op = "";
      for (int j = 0; j < 8; j++) {
        int row = 2 * int(x[j * 6] - '0') + int(x[j * 6 + 5] - '0');
        int col = 8 * int(x[j * 6 + 1] - '0') + 4 * int(x[j * 6 + 2] - '0') + 2 * int(x[j * 6 + 3] - '0') + int(x[j * 6 + 4] - '0');
        int val = s[j][row][col];
        op += char(val / 8 + '0');
        val = val % 8;
        op += char(val / 4 + '0');
        val = val % 4;
        op += char(val / 2 + '0');
        val = val % 2;
        op += char(val + '0');
      }

      // Straight D-box
      op = permute(op, per, 32);

      // XOR left and op
      x = binxor(op, left);
      left = x;

      // Swapper if not at the last key
      if (i != 15)
        swap(left, right);

      //std::cout << "Round " << i + 1 << " " << bin2hex(left) << " " << bin2hex(right) << " " << rk[i] << std::endl;
    }

    // Combination of left and right halves
    combine = left + right;

    // Concatentation and Final Permutation
    cipher += bin2hex(permute(combine, final_perm, 64));
  }

  return cipher;
}

/*
int main() {
  // pt is plain text
  std::string pt, cipher;

  std::cout << "Enter a password: ";
  std::cin >> pt;

  std::cout << "\nEncryption:\n";
  cipher = DES(pt);

  std::cout << "\nCipher Text: " << cipher << std::endl;
}
*/
