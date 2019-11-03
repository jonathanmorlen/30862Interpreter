#include "Value.h"

Value::Value(char c) : MemoryObject(c)
{
	value = c;
	valueTag type = CHAR;
}
Value::Value(short c) : MemoryObject(c)
{
	value = c;
	valueTag type = SHORT;
}
Value::Value(int c) : MemoryObject(c)
{
	value = c;
	valueTag type = INT;
}
Value::Value(float c) : MemoryObject(c)
{
	value = c;
	valueTag type = FLOAT;
}

char Value::getChar()
{
	return 0;
}

short Value::getShort()
{
	return 0;
}

int Value::getInt()
{
	return 0;
}

float Value::getFloat()
{
	return 0.0f;
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
	os << "V(" << value.value << ")";
	return os;
}