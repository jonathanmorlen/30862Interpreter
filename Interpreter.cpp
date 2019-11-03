// Interpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>

#include "MemoryObject.h"
#include "ByteCode.h"
#include "Value.h"

int main(const int argc, char* argv[])
{
	// Get filename from command line
	if(argc == 1)
	{
		std::cout << "Must pass a program filename to the interpreter." << std::endl;
		exit(EXIT_FAILURE);
	}
	char* inputFilename = argv[1];
	
	// Read input file
	std::ifstream inputFile;
	inputFile.open(inputFilename);

	// Error handling
	if(!inputFile)
	{
		std::cerr << "Cannot open file: " << inputFilename << std::endl;
		exit(EXIT_FAILURE);
	}
	char byte;

	// Read bytes into memory
	std::vector<unsigned char> programMemory;
	while (inputFile.read(&byte, 1))
	{
		programMemory.push_back(byte);
	}
	inputFile.close();

	// Initialize ByteCode variables and data structures to be used in execute()
	ByteCode::framePointerStackPointer = -1;
	ByteCode::runtimeStackPointer = -1;
	ByteCode::runtimeStack = std::vector<Value>();
	ByteCode::framePointerStack = std::vector<Value>();
	ByteCode::programMemory = programMemory;
	
	// Run the program
	int programCounter = 0;
	while(true)
	{
		programCounter = ByteCode(programMemory[programCounter]).execute(programCounter);
	}	
}