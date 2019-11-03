// Interpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>

#include "MemoryObject.h"
#include "ByteCode.h"
#include "Value.h"

constexpr auto inputFilename = "input.txt";

// This is a test
// Testing

int main()
{
	// Initialize counters and pointers
	int programCounter = 0;
	int runtimeStackPointer = -1;
	int framePointerStackPointer = -1;

	// Initialize stacks
	std::vector<Value> runtimeStack;
	std::vector<Value> framePointerStack;
	std::vector<unsigned char> programMemory;

	// Read input file
	std::ifstream inputFile;
	inputFile.open(inputFilename);

	// Error handling
	if(!inputFile)
	{
		std::cerr << "Cannot open file: " << inputFilename << std::endl;
		exit(1);
	}
	char byte;
	while (inputFile.read(&byte, 1))
	{
		programMemory.push_back(byte);
	}
	inputFile.close();

	// Initialize ByteCode variables to be used in execute()
	ByteCode::framePointerStackPointer = framePointerStackPointer;
	ByteCode::runtimeStackPointer = runtimeStackPointer;
	ByteCode::runtimeStack = runtimeStack;
	ByteCode::framePointerStack = framePointerStack;
	ByteCode::programMemory = programMemory;
	
	// Run the program
	bool continueExecution = true;
	while(continueExecution)
	{
		programCounter = ByteCode(programMemory[programCounter]).execute(programCounter);
	}	
}