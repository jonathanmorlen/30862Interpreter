#include "Value.h"

Value::Value(const char c)
{
	charValue = c;
	shortValue = 0;
	intValue = 0;
	floatValue = 0;
	type = CHAR;
}

Value::Value(const short c)
{
	charValue = 0;
	shortValue = c;
	intValue = 0;
	floatValue = 0;
	type = SHORT;
}

Value::Value(const int c)
{
	charValue = 0;
	shortValue = 0;
	intValue = c;
	floatValue = 0;
	type = INT;
}

Value::Value(const float c)
{
	charValue = 0;
	shortValue = 0;
	intValue = 0;
	floatValue = c;
	type = FLOAT;
}

char Value::getChar()
{
	return charValue;
}

short Value::getShort()
{
	return shortValue;
}

int Value::getInt()
{
	return intValue;
}

float Value::getFloat()
{
	return floatValue;
}

// For use when using value to hold addresses just in case
int Value::getValue()
{
	int basicValue = 0;
	if (this->type == CHAR)
	{
		basicValue = static_cast<int>(this->getChar( ));
	}
	else if (this->type == SHORT)
	{
		basicValue = static_cast<int>(this->getShort( ));
	}
	else if (this->type == INT)
	{
		basicValue = this->getInt( );
	}
	else if (this->type == SHORT)
	{
		basicValue = static_cast<int>(this->getFloat( ));
	}
	return basicValue;
}

std::ostream& operator<<(std::ostream& os, Value& value)
{
	os << value.getValue( );
	return os;
}
