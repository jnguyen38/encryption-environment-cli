# Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
# E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
#
# This is the Makefile for the final project

# g++ is for the compiler for C++
PP := g++

# CFLAGS are the compiler flages for when we compile C code in this course
FLAGS := -O2 -g -Wall -Wextra  -Wshadow -Werror -lm
CXXFLAGS := -m64 -std=c++11 -Weffc++ -lcrypto $(FLAGS)

# Folder Variables
SRC := src
INC := include
OBJ := objects
EXE := exe

SHA256_objs := $(OBJ)/SHA256.o
SHA1_objs := $(OBJ)/SHA1.o
MD5_objs := $(OBJ)/MD5.o
DES_objs := $(OBJ)/DES.o
PROJ_objs := $(SHA256_objs) $(SHA1_objs) $(MD5_objs) $(DES_objs) $(OBJ)/funcproj.o $(SRC)/project.cpp

# Create the project executable
project: $(PROJ_objs)
	$(PP) $(CXXFLAGS) $(PROJ_objs) -o $(EXE)/project

# Create the hash objects
$(OBJ)/SHA256.o: $(SRC)/SHA256functions.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/SHA256functions.cpp -o $(OBJ)/SHA256.o

$(OBJ)/SHA1.o: $(SRC)/SHA1.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/SHA1.cpp -o $(OBJ)/SHA1.o

$(OBJ)/MD5.o: $(SRC)/MD5.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/MD5.cpp -o $(OBJ)/MD5.o

$(OBJ)/DES.o: $(SRC)/DES.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/DES.cpp -o $(OBJ)/DES.o

$(OBJ)/funcproj.o: $(SRC)/funcproj.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/funcproj.cpp -o $(OBJ)/funcproj.o

# for debugging sha1
sha1: $(SRC)/SHA1.cpp
	$(PP) $(CXXFLAGS) $(SRC)/SHA1.cpp -o $(EXE)/SHA1

# for debugging sha256
sha256: $(SRC)/SHA256.cpp
		$(PP) $(CXXFLAGS) $(SRC)/SHA256.cpp -o $(EXE)/SHA256

# for debugging des
DES: $(SRC)/DES.cpp
	$(PP) $(CXXFLAGS) $(SRC)/DES.cpp -o $(EXE)/DES

# Initialize the folders for the objects and executables
initialize:
	mkdir $(OBJ) $(EXE)

# Make clean
clean:
	rm -rf $(OBJ)/* $(EXE)/*
