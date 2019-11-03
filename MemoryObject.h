#pragma once
#include <ostream>

class MemoryObject
{
public:
	// Constructor(s) and Destructor(s)
	MemoryObject(int);
	virtual ~MemoryObject() = default;
	
	// Variables
	char value;
	
	// Methods
	friend std::ostream & operator<<(std::ostream &os, const MemoryObject &memObj);
	
};
