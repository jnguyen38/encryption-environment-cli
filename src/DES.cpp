/**********************************
 * Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
 * E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
 * File Name: funcproj.h
 * Date Created: 11/19/21
 * File Contents: This file contains the DES hash function
 *                for the final project
 **********************************/

#include "../include/DES.h"

// Standard Hex to Binary Conversion
std::string hex2bin(std::string string) {
  std::string bin = "";
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

  for (unsigned int i = 0; i < string.size(); i++) {
      bin += converter[string[i]];
  }
  return bin;
}

// Standard Binary to Hex Conversion
std::string bin2hex(std::string string) {
  std::string hexStr = "", tempBin = "";
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
  for (unsigned int i = 0; i < string.length(); i += 4) {
      tempBin = "";
      tempBin += string[i];
      tempBin += string[i + 1];
      tempBin += string[i + 2];
      tempBin += string[i + 3];
      hexStr += converter[tempBin];
  }
  return hexStr;
}

std::string permute(std::string k, int* arr, int n) {
    std::string per = "";
    for (int i = 0; i < n; i++) {
        per += k[arr[i] - 1];
    }
    return per;
}

std::string shift_left(std::string k, int shifts) {
    std::string s = "";
    for (int i = 0; i < shifts; i++) {
        for (int j = 1; j < 28; j++) {
            s += k[j];
        }
        s += k[0];
        k = s;
        s = "";
    }
    return k;
}

std::string xor_(std::string a, std::string b) {
    std::string ans = "";
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a[i] == b[i]) {
            ans += "0";
        }
        else {
            ans += "1";
        }
    }
    return ans;
}

std::string encrypt(std::string pt, std::vector<std::string> rkb, std::vector<std::string> rk) {
    // Hexadecimal to binary
    pt = hex2bin(pt);

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

    // Initial Permutation
    pt = permute(pt, initial_perm, 64);
    std::cout << "After initial permutation: " << bin2hex(pt) << std::endl;

    // Splitting
    std::string left = pt.substr(0, 32);
    std::string right = pt.substr(32, 32);
    std::cout << "After splitting: L0=" << bin2hex(left) << " R0=" << bin2hex(right) << std::endl;

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

    std::cout << std::endl;
    for (int i = 0; i < 16; i++) {
        // Expansion D-box
        std::string right_expanded = permute(right, exp_d, 48);

        // XOR RoundKey[i] and right_expanded
        std::string x = xor_(rkb[i], right_expanded);

        // S-boxes
        std::string op = "";
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
        x = xor_(op, left);

        left = x;

        // Swapper
        if (i != 15) {
            swap(left, right);
        }
        std::cout << "Round " << i + 1 << " " << bin2hex(left) << " "
             << bin2hex(right) << " " << rk[i] << std::endl;
    }

    // Combination
    std::string combine = left + right;

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

    // Final Permutation
    std::string cipher = bin2hex(permute(combine, final_perm, 64));
    return cipher;
}

int main() {
    // pt is plain text
    std::string pt, key;
    /*std::cout<<"Enter plain text(in hexadecimal): ";
    std::cin>>pt;
    std::cout<<"Enter key(in hexadecimal): ";
    std::cin>>key;*/

    pt = "123456ABCD132536";
    key = "AABB09182736CCD";
    // Key Generation

    // Hex to binary
    key = hex2bin(key);

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

    // getting 56 bit key from 64 bit using the parity bits
    key = permute(key, keyp, 56); // key without parity

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

    // Splitting
    std::string left = key.substr(0, 28);
    std::string right = key.substr(28, 28);

    std::vector<std::string> rkb; // rkb for RoundKeys in binary
    std::vector<std::string> rk; // rk for RoundKeys in hexadecimal
    for (int i = 0; i < 16; i++) {
        // Shifting
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);

        // Combining
        std::string combine = left + right;

        // Key Compression
        std::string RoundKey = permute(combine, key_comp, 48);

        rkb.push_back(RoundKey);
        rk.push_back(bin2hex(RoundKey));
    }

    std::cout << "\nEncryption:\n\n";
    std::string cipher = encrypt(pt, rkb, rk);
    std::cout << "\nCipher Text: " << cipher << std::endl;

    std::cout << "\nDecryption\n\n";
    reverse(rkb.begin(), rkb.end());
    reverse(rk.begin(), rk.end());
    std::string text = encrypt(cipher, rkb, rk);
    std::cout << "\nPlain Text: " << text << std::endl;
}
