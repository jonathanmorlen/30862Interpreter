#include "Memory.h"

ByteCode Program::getByteCode(int programCounter)
{
	return ByteCode(programMemory[programCounter]);
}

