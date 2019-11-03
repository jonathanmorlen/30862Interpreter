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
	enum ValueTag
	{
		CHAR = 1,
		SHORT = 2,
		INT = 3,
		FLOAT = 4
	}type;	

	// Methods
	char getChar();
	short getShort();
	int getInt();
	float getFloat();
	int getValue();
	friend std::ostream& operator<<(std::ostream& os, Value& value);

private:
	char charValue;
	short shortValue;
	int intValue;
	float floatValue;
	
};