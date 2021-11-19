# Authors: Catherine Pardi, Andrea Turner, Ryan O'Halloran, and Jonathan Nguyen
# E-mails: cpardi@nd.edu, aturne22@nd.edu, rohallo2@nd.edu, and jnguyen5@nd.edu
#
# This is the Makefile for the final project

# g++ is for the compiler for C++
PP := g++

# CFLAGS are the compiler flages for when we compile C code in this course
FLAGS := -O2 -g -Wall -Wextra -Wconversion -Wshadow -Werror -lm
CXXFLAGS := -m64 -std=c++11 -Weffc++ $(FLAGS)

# Folder Variables
SRC := src
INC := include
OBJ := objects
EXE := exe

# Initialize the folders for the objects and executables
initialize:
	mkdir $(OBJ) $(EXE)

# Create the hash objects
$(OBJ)/SHA256.o: $(SRC)/SHA256.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/SHA256.cpp -o $(OBJ)/SHA256.o

$(OBJ)/SHA1.o: $(SRC)/SHA1.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/SHA1.cpp -o $(OBJ)/SHA1.o

$(OBJ)/MD5.o: $(SRC)/MD5.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/MD5.cpp -o $(OBJ)/MD5.o

$(OBJ)/DES.o: $(SRC)/DES.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/DES.cpp -o $(OBJ)/DES.o

$(OBJ)/funcproj.o: $(SRC)/funcproj.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/funcproj.cpp -o $(OBJ)/funcproj.o

SHA256_objs := $(OBJ)/SHA256.o $(SRC)/SHA256.cpp
SHA1_objs := $(OBJ)/SHA1.o $(SRC)/SHA1.cpp
MD5_objs := $(OBJ)/MD5.o $(SRC)/MD5.cpp
DES_objs := $(OBJ)/DES.o $(SRC)/DES.cpp
PROJ_objs := $(SHA256_objs) $(SHA1_objs) $(MD5_objs) $(DES_objs) $(OBJ)/funcproj.o $(SRC)/project.cpp

# Create the project executable
project: $(PROJ_objs)
	$(PP) $(CXXFLAGS) $(PROJ_objs) -o $(EXE)/project

# Make clean
clean:
	rm -rf $(OBJ)/* $(EXE)/*
