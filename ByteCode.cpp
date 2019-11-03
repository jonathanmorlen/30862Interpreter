#include "ByteCode.h"
#include <iostream>
#include "Value.h"

ByteCode::ByteCode(char c) : MemoryObject(c)
{
	value = c;
}

int ByteCode::programCounter;
int ByteCode::framePointerStackPointer;
int ByteCode::runtimeStackPointer;

std::vector<Value> ByteCode::runtimeStack;
std::vector<Value> ByteCode::framePointerStack;
std::vector<unsigned char> ByteCode::programMemory;

int ByteCode::execute(int programCounter)
{
	int byteCode = this -> value;

	// Halt [00000000]
	if (byteCode == 0)
	{
		std::cout << "Program Counter: " << programCounter << std::endl;
		std::cout << "Stack Pointer: " << runtimeStackPointer << std::endl;
		std::cout << "Runtime Stack:" << std::endl;
		printStack(runtimeStack);
		std::cout << std::endl;
		std::cout << "Frame Pointer Stack Pointer: " << framePointerStackPointer << std::endl;
		std::cout << "Frame Pointer Stack: " << std::endl;
		printStack(framePointerStack);
		exit(0);
	}

	// jmp [00100100]
	if (byteCode == 36)
	{
		programCounter = runtimeStack[runtimeStackPointer - 1] . getInt( );
		runtimeStackPointer--;
		return programCounter;
	}

	// jmpc [00101000]
	if (byteCode == 40)
	{
		if (runtimeStack[runtimeStackPointer - 1] . getInt( ) == 1)
		{
			programCounter = runtimeStack[runtimeStackPointer] . getInt( );
		}
		runtimeStackPointer -= 2;
		return programCounter;
	}

	// call [00101100]
	if (byteCode == 44)
	{
		framePointerStack[++framePointerStackPointer] = Value(runtimeStackPointer - runtimeStack[runtimeStackPointer].getInt(  ) - 1);
		runtimeStackPointer--;
		programCounter = runtimeStack[runtimeStackPointer--].getInt(  );
		return programCounter;
	}

	// ret [00110000]
	if(byteCode == 48)
	{
		runtimeStackPointer = framePointerStack[framePointerStackPointer--].getInt(  );
		framePointerStack.pop_back();
		programCounter = runtimeStack[runtimeStackPointer--].getInt(  );
	}

	// pushc [01000100]
	if(byteCode == 68)
	{
		runtimeStack.emplace_back(Value(programMemory[programCounter + 1]));
		runtimeStackPointer++;
		programCounter += 2;
		return programCounter;
	}

	// pushs [01000101]
	if(byteCode == 69)
	{
		short shortValue;
		unsigned char bytes[] = { programMemory[programCounter + 1], programMemory[programCounter + 2] };
		memcpy(&shortValue, &bytes, sizeof(shortValue));
		runtimeStack.emplace_back(Value(shortValue));
		runtimeStackPointer++;
		programCounter += 3;
		return programCounter;
	}

	// pushi [01000110]
	if (byteCode == 70)
	{
		int intValue;
		unsigned char bytes[] = { programMemory[programCounter + 1], programMemory[programCounter + 2],
								  programMemory[programCounter + 3], programMemory[programCounter + 4] };
		memcpy(&intValue, &bytes, sizeof(intValue));
		runtimeStack.emplace_back(Value(intValue));
		runtimeStackPointer++;
		programCounter += 5;
		return programCounter;
	}

	// pushf [01000111]
	if (byteCode == 71)
	{
		int floatValue;
		unsigned char bytes[] = { programMemory[programCounter + 1], programMemory[programCounter + 2],
								  programMemory[programCounter + 3], programMemory[programCounter + 4] };
		memcpy(&floatValue, &bytes, sizeof(floatValue));
		runtimeStackPointer++;
		runtimeStack.emplace_back(Value(floatValue));
		programCounter += 5;
		return programCounter;
	}

	// pushvc [01001000]
	if(byteCode == 72)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) +
											runtimeStack[runtimeStackPointer].getInt(  ) + 1];
		return ++programCounter;
	}

	// pushvs [01001001]
	if (byteCode == 73)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) +
			runtimeStack[runtimeStackPointer].getInt(  ) + 1];
		return ++programCounter;
	}

	// pushvi [01001010]
	if (byteCode == 74)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) +
			runtimeStack[runtimeStackPointer].getInt(  ) + 1];
		return ++programCounter;
	}

	// pushvf [01001011]
	if (byteCode == 75)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) +
			runtimeStack[runtimeStackPointer].getInt(  ) + 1];
		return ++programCounter;
	}

	// popm [01001100]
	if (byteCode == 76)
	{
		runtimeStackPointer -= runtimeStack[runtimeStackPointer].getInt(  ) + 1;
		return ++programCounter;
	}

	// popa [01001101]
	if (byteCode == 77)
	{
		//TODO figure it out
		runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) + 1] = runtimeStack[runtimeStackPointer - runtimeStack[runtimeStackPointer].getInt(  )];
		runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) + 2] = runtimeStack[runtimeStackPointer - runtimeStack[runtimeStackPointer].getInt(  ) + 1];
	}
	
	// popv [01010000]
	if(byteCode == 80)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) + runtimeStack[runtimeStackPointer].getInt(  ) + 1] = runtimeStack[runtimeStackPointer];
		runtimeStackPointer -= 2;
		return ++programCounter;
	}

	// peekc [01011000]
	if(byteCode == 84)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt(  ) + 1] = 
			runtimeStack[framePointerStack[framePointerStackPointer].getInt(  ) + runtimeStack[runtimeStackPointer].getInt(  ) + 1];
		return ++programCounter;
	}
	// peeks [01011001]
	if (byteCode == 85)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1];
		return ++programCounter;
	}
	
	// peeki [01011010]
	if (byteCode == 86)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1];
		return ++programCounter;
	}
	
	// peekf [01011011]
	if (byteCode == 87)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1];
		return ++programCounter;
	}

	// pokec [01100000]
	if(byteCode == 88)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1] = 
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1].getChar();
	}

	// pokes [01100001]
	if (byteCode == 88)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1].getShort();
	}

	// pokei [01100010]
	if (byteCode == 88)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1].getInt();
	}

	// pokef [01100011]
	if (byteCode == 88)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer].getInt() + 1] =
			runtimeStack[framePointerStack[framePointerStackPointer].getInt() + runtimeStack[runtimeStackPointer - 1].getInt() + 1].getFloat();
	}

	// swp [01100011]
	if(byteCode == 94)
	{
		Value temp = Value(runtimeStack[runtimeStackPointer - 1]);
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer];
		runtimeStack[runtimeStackPointer] = temp;
	}

	// add [01100100]
	if(byteCode == 100)
	{
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer - 1].getFloat(  ) + runtimeStack[runtimeStackPointer].getInt(  );
	}

	// sub [01101000]
	if (byteCode == 104)
	{
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer - 1].getFloat() - runtimeStack[runtimeStackPointer].getInt();
	}

	// mul [01101100]
	if (byteCode == 108)
	{
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer - 1].getFloat() * runtimeStack[runtimeStackPointer].getInt();
	}

	// div [01110000]
	if (byteCode == 112)
	{
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer - 1].getFloat() / runtimeStack[runtimeStackPointer].getInt();
	}

	// cmpe [10000100]
	if (byteCode == 132)
	{
		const Value equal = Value(runtimeStack[runtimeStackPointer - 1] . getFloat( ) == runtimeStack[
			                          runtimeStackPointer] . getFloat( ));
		runtimeStack[runtimeStackPointer - 1] = equal;
		runtimeStackPointer--;
		return ++programCounter;
	}

	// cmplt [10001000]
	if (byteCode == 136)
	{
		const Value lessThan = Value(runtimeStack[runtimeStackPointer - 1] . getFloat( ) < runtimeStack[
			                             runtimeStackPointer] . getFloat( ));
		runtimeStack[runtimeStackPointer - 1] = lessThan;
		runtimeStackPointer--;
		return ++programCounter;
	}

	// cmpgt [10001100]
	if (byteCode == 140)
	{
		const Value greaterThan = Value(runtimeStack[runtimeStackPointer - 1] . getFloat( ) > runtimeStack[
			                                runtimeStackPointer] . getFloat( ));
		runtimeStack[runtimeStackPointer - 1] = greaterThan;
		runtimeStackPointer--;
		return ++programCounter;
	}

	// printc [10010000]
	if(byteCode == 144)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getChar() << std::endl;
		return ++programCounter;
	}

	// prints [10010001]
	if (byteCode == 145)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getShort() << std::endl;
		return ++programCounter;
	}

	// printi [10010010]
	if (byteCode == 146)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getInt() << std::endl;
		return ++programCounter;
	}

	// printf [10010011]
	if (byteCode == 147)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getFloat() << std::endl;
		return ++programCounter;
	}

	// Error Handling
	std::cerr << "Instruction " << byteCode << " does not exist" << std::endl;
	exit(1);
}

void ByteCode::printStack(std::vector<Value> vec)
{
	std::cout << '[';
	if (!vec . empty( ))
	{
		for (auto i = vec . begin( ); i != vec . end( ); ++i)
		{
			std::cout << *i;
			if (i + 1 != vec . end( ))
			{
				std::cout << ", ";
			}
		}
	}
	else
	{
		std::cout << " Empty " << std::endl;
	}
	std::cout << ']';
}


std::ostream& operator<<(std::ostream& os, const ByteCode& byteCode)
{
	os << " BC(" << byteCode . value << ")";
	return os;
}