# FinalProject
Our project objective is to build a Password Protection Simulation Engine that uses various types of Secure Hash Algorithms (SHAs) to store usernames and their respective hashed passwords in a local database. The simulation will replicate the functionality of any ordinary sign-in user interface on account-based websites but will incorporate optional educational steps to guide and teach the user about one-way hash algorithms as a method for secure password storage. As a result, the simulation engine will not only store and act as a password database, but will document and display how widely used hash algorithms –  such as SHA-256 – work step by step.  The user will be able to input a username into the command line in plain text and if the username is in the database, the program will ask for the password through command line input; if not, the user can create and re-confirm their password in the command line. Once they enter a valid password, the user can choose between a variety of hashing algorithms such as SHA-1, SHA-2 family, MD5, DES, etc. to see how they work and how hashing algorithms have evolved over time to become more secure. Our program will output a .txt file which can be converted into a hashmap by our program in order to act as a database for our usernames and passwords that can be saved between program executions. 

Link to explanatory article:
https://docs.google.com/document/d/1aDFlFlzOveRRZa7K_13vF2zfeLzYp3wWw2MwNugrIh8/edit?usp=sharing

## Usage

To use or test this CLI Environment, first clone this repository onto your local machine

```
git clone https://github.com/jnguyen38/encryption-environment-cli
```

Then, `cd` into the environment and run the Makefile command

```
cd encryption-environement-cli
make project
```

Finally, run the project executable with

```
./exe/project
```
