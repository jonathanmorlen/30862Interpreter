#pragma once
#include <ostream>

class MemoryObject
{
public:
	// Constructor(s) and Destructor(s)
	explicit MemoryObject(int val = 0);
	virtual ~MemoryObject() = default;

	// Variables
	int value;

	// Methods
	friend std::ostream& operator<<(std::ostream& os, const MemoryObject& memObj);
};
