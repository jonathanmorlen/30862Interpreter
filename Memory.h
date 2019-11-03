#pragma once
#include <vector>
#include "MemoryObject.h"
#include "ByteCode.h"
class Program
{
public:
	// Constructor(s) and Destructor(s)
	Program() = default;
	virtual ~Program() = default;

	// Variables
	static std::vector<char> programMemory;
	static int programCounter;
	static int runtimeStackPointer;
	static std::vector<MemoryObject> runtimeStack;
	static int framePointerStackPointer;
	static std::vector<MemoryObject> framePointerStack;

	// Methods
	static ByteCode getByteCode(int programCounter);
};