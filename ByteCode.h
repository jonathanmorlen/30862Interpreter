#pragma once
#include <ostream>
#include <vector>

#include "MemoryObject.h"
#include "Value.h"

class ByteCode: public MemoryObject
{
public:
	// Constructor(s) and Destructor(s)
	ByteCode(int);
	virtual ~ByteCode() = default;

	// Variables
	static int programCounter;
	static int framePointerStackPointer;
	static int runtimeStackPointer;

	static std::vector<Value> runtimeStack;
	static std::vector<Value> framePointerStack;
	static std::vector<unsigned char> programMemory;
	
	// Methods
	int execute(int);
	friend std::ostream& operator<<(std::ostream& os, const ByteCode& byteCode);
	
private:
	static void printStack(std::vector<Value> vec);
};
