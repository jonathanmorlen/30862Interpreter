#pragma once
#include <ostream>

#include "MemoryObject.h"

class Value : public MemoryObject
{
public:
	// Constructor(s) and Destructor(s)
	Value(char);
	Value(short);
	Value(int);
	Value(float);
	~Value() = default;

	// Variables
	enum valueTag
	{
		CHAR = 1,
		SHORT = 2,
		INT = 3,
		FLOAT = 4
	};

	int value;
	

	// Methods
	char getChar();
	short getShort();
	int getInt();
	float getFloat();
	friend std::ostream& operator<<(std::ostream& os, const Value& value);
	
};