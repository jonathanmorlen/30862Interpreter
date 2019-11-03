#include "MemoryObject.h"
#include <iostream>

MemoryObject::MemoryObject(int c)
{
	value = c;
}

std::ostream& operator<<(std::ostream& os, const MemoryObject& memObj)
{
	os << " MO(" << memObj.value << ")";
	return os;
}
