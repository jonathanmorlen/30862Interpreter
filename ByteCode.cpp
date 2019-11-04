#include "ByteCode.h"
#include <iostream>
#include "Value.h"

ByteCode::ByteCode(const int c)
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
	const int byteCode = this->value;

	// Halt [00000000]
	if (byteCode == 0)
	{
		std::cout << "Program Counter: " << programCounter << "/" << programMemory.size( ) - 1 << std::endl << std::
				endl;
		std::cout << "Stack Pointer: " << runtimeStackPointer << std::endl;
		std::cout << "Runtime Stack:" << std::endl;
		printStack(runtimeStack);
		std::cout << std::endl << std::endl;
		std::cout << "Frame Pointer Stack Pointer: " << framePointerStackPointer << std::endl;
		std::cout << "Frame Pointer Stack: " << std::endl;
		printStack(framePointerStack);
		std::cout << std::endl;
		exit(EXIT_SUCCESS);
	}

	// jmp [00100100]
	if (byteCode == 36)
	{
		programCounter = runtimeStack[runtimeStackPointer].getInt( );

		runtimeStack.pop_back( );
		runtimeStackPointer--;

		return programCounter;
	}

	// jmpc [00101000]
	if (byteCode == 40)
	{
		if (runtimeStack[runtimeStackPointer - 1].getInt( ) == 1)
		{
			programCounter = runtimeStack[runtimeStackPointer].getInt( );
		}

		runtimeStack.pop_back( );
		runtimeStack.pop_back( );
		runtimeStackPointer -= 2;

		return programCounter;
	}

	// call [00101100]
	if (byteCode == 44)
	{
		// Save runtimeStackPointer minus the number of arguments to
		// function (number at the top of the runtimeStack) being called
		framePointerStack.emplace_back(Value(runtimeStackPointer - runtimeStack[runtimeStackPointer].getInt( ) - 1));
		framePointerStackPointer++;
		
		// Pop off number of arguments, runtimeStackPointer points to address of function to call
		runtimeStack.pop_back();
		runtimeStackPointer--;

		// Address of first instruction of function. Read in as decimal, converted to hex		
		programCounter = runtimeStack[runtimeStackPointer].getInt();
		programCounter = 23;

		runtimeStack.pop_back( );
		runtimeStackPointer--;

		return programCounter;
	}

	// ret [00110000]
	if (byteCode == 48)
	{
		runtimeStackPointer = framePointerStack[framePointerStackPointer].getInt( );

		framePointerStack.pop_back( );
		framePointerStackPointer--;

		programCounter = runtimeStack[runtimeStackPointer].getInt( );

		runtimeStack.pop_back( );
		runtimeStackPointer--;

		return programCounter;
	}

	// pushc [01000100]
	if (byteCode == 68)
	{
		char charValue = programMemory[programCounter + 1];

		runtimeStack.emplace_back(Value(charValue));
		runtimeStackPointer++;

		programCounter += 2;
		return programCounter;
	}

	// pushs [01000101]
	if (byteCode == 69)
	{
		short shortValue;
		unsigned char bytes[] =
		{
			programMemory[programCounter + 1],
			programMemory[programCounter + 2]
		};
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
		unsigned char bytes[] =
		{
			programMemory[programCounter + 1],
			programMemory[programCounter + 2],
			programMemory[programCounter + 3],
			programMemory[programCounter + 4]
		};
		memcpy(&intValue, &bytes, sizeof(intValue));

		runtimeStack.emplace_back(Value(intValue));
		runtimeStackPointer++;

		programCounter += 5;
		return programCounter;
	}

	// pushf [01000111]
	if (byteCode == 71)
	{
		float floatValue;
		unsigned char bytes[] =
		{
			programMemory[programCounter + 1],
			programMemory[programCounter + 2],
			programMemory[programCounter + 3],
			programMemory[programCounter + 4]
		};
		memcpy(&floatValue, &bytes, sizeof(floatValue));

		runtimeStack.emplace_back(Value(floatValue));
		runtimeStackPointer++;

		programCounter += 5;
		return programCounter;
	}

	// pushvc [01001000]
	if (byteCode == 72)
	{
		runtimeStack.emplace_back(Value(runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) +
			                                runtimeStack[runtimeStackPointer].getInt( ) + 1]));
		runtimeStackPointer++;

		return ++programCounter;
	}

	// pushvs [01001001]
	if (byteCode == 73)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) +
			runtimeStack[runtimeStackPointer].getInt( ) + 1];
		return ++programCounter;
	}

	// pushvi [01001010]
	if (byteCode == 74)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) +
			runtimeStack[runtimeStackPointer].getInt( ) + 1];
		return ++programCounter;
	}

	// pushvf [01001011]
	if (byteCode == 75)
	{
		runtimeStack[runtimeStackPointer] = runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) +
			runtimeStack[runtimeStackPointer].getInt( ) + 1];
		return ++programCounter;
	}

	// popm [01001100]
	if (byteCode == 76)
	{
		runtimeStackPointer -= runtimeStack[runtimeStackPointer].getInt( ) + 1;
		return ++programCounter;
	}

	// popa [01001101]
	if (byteCode == 77)
	{
		unsigned int numElementsToKeep = runtimeStack.back().getInt();

		// Pop numElementsToKeep from runtimeStack to framePointerStack to save them
		for(unsigned int i = 0; i < numElementsToKeep; i++)
		{
			framePointerStack.emplace_back(runtimeStack.back());
			runtimeStack.pop_back();
		}

		// Empty runtimeStack
		while(!runtimeStack.empty(  ))
		{
			runtimeStack.pop_back();
		}

		// Restore numElements from framePointerStack to runtimeStack
		for(unsigned int i = 0; i < numElementsToKeep; i++)
		{
			runtimeStack.emplace_back(framePointerStack.back());
			framePointerStack.pop_back();
		}
		return ++programCounter;
	}

	// popv [01010000]
	if (byteCode == 80)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer].
			getInt( ) + 1] = runtimeStack[runtimeStackPointer];
		runtimeStackPointer -= 2;
		return ++programCounter;
	}

	// peekc [01011000]
	if (byteCode == 84)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer - 1].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer]
				   .getInt( ) + 1];
		return ++programCounter;
	}
	// peeks [01011001]
	if (byteCode == 85)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer - 1].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer]
				   .getInt( ) + 1];
		return ++programCounter;
	}

	// peeki [01011010]
	if (byteCode == 86)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer - 1].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer]
				   .getInt( ) + 1];
		return ++programCounter;
	}

	// peekf [01011011]
	if (byteCode == 87)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer - 1].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer]
				   .getInt( ) + 1];
		return ++programCounter;
	}

	// pokec [01100000]
	if (byteCode == 88)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer
					- 1].getInt( ) + 1].getChar( );
		return ++programCounter;
	}

	// pokes [01100001]
	if (byteCode == 89)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer
					- 1].getInt( ) + 1].getShort( );
		return ++programCounter;
	}

	// pokei [01100010]
	if (byteCode == 90)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer
					- 1].getInt( ) + 1].getInt( );
		return ++programCounter;
	}

	// pokef [01100011]
	if (byteCode == 91)
	{
		runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer].
					getInt( ) + 1] =
				runtimeStack[framePointerStack[framePointerStackPointer].getInt( ) + runtimeStack[runtimeStackPointer
					- 1].getInt( ) + 1].getFloat( );
		return ++programCounter;
	}

	// swp [01011110]
	if (byteCode == 94)
	{
		Value temp = Value(runtimeStack[runtimeStackPointer - 1]);
		runtimeStack[runtimeStackPointer - 1] = runtimeStack[runtimeStackPointer];
		runtimeStack[runtimeStackPointer] = temp;
		return ++programCounter;
	}

	// add [01100100]
	if (byteCode == 100)
	{
		if(runtimeStack.back(  ).type == Value::CHAR)
		{
			char operandA = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			char operandB = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();
			
			runtimeStack.emplace_back(Value(operandA + operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::SHORT)
		{
			short operandA = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			short operandB = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA + operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::INT)
		{
			int operandA = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			int operandB = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA + operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::FLOAT)
		{
			float operandA = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			float operandB = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA + operandB));
			runtimeStackPointer++;
		}
		return ++programCounter;
	}

	// sub [01101000]
	if (byteCode == 104)
	{
		if (runtimeStack.back().type == Value::CHAR)
		{
			char operandA = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			char operandB = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB - operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::SHORT)
		{
			short operandA = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			short operandB = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB - operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::INT)
		{
			int operandA = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			int operandB = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB - operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::FLOAT)
		{
			float operandA = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			float operandB = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB - operandA));
			runtimeStackPointer++;
		}
		return ++programCounter;
	}

	// mul [01101100]
	if (byteCode == 108)
	{
		if (runtimeStack.back().type == Value::CHAR)
		{
			char operandA = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			char operandB = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA * operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::SHORT)
		{
			short operandA = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			short operandB = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA * operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::INT)
		{
			int operandA = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			int operandB = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA * operandB));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::FLOAT)
		{
			float operandA = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			float operandB = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandA * operandB));
			runtimeStackPointer++;
		}
		return ++programCounter;
	}

	// div [01110000]
	if (byteCode == 112)
	{
		if (runtimeStack.back().type == Value::CHAR)
		{
			char operandA = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			char operandB = runtimeStack[runtimeStackPointer].getChar();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB / operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::SHORT)
		{
			short operandA = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			short operandB = runtimeStack[runtimeStackPointer].getShort();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB / operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::INT)
		{
			int operandA = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			int operandB = runtimeStack[runtimeStackPointer].getInt();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB / operandA));
			runtimeStackPointer++;
		}
		else if (runtimeStack.back().type == Value::FLOAT)
		{
			float operandA = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			float operandB = runtimeStack[runtimeStackPointer].getFloat();
			runtimeStackPointer--;
			runtimeStack.pop_back();

			runtimeStack.emplace_back(Value(operandB / operandA));
			runtimeStackPointer++;
		}
		return ++programCounter;
	}

	// cmpe [10000100]
	if (byteCode == 132)
	{
		const Value equal = Value(runtimeStack[runtimeStackPointer - 1].getValue( ) == runtimeStack[
			                          runtimeStackPointer].getValue( ));
		runtimeStack.pop_back();
		runtimeStack.pop_back();
		runtimeStack.emplace_back(equal);
		runtimeStackPointer--;
		return ++programCounter;
	}

	// cmplt [10001000]
	if (byteCode == 136)
	{
		const Value lessThan = Value(runtimeStack[runtimeStackPointer - 1].getValue( ) < runtimeStack[
			                             runtimeStackPointer].getValue( ));
		runtimeStack.pop_back();
		runtimeStack.pop_back();
		runtimeStack.emplace_back(lessThan);
		runtimeStackPointer--;
		return ++programCounter;
	}

	// cmpgt [10001100]
	if (byteCode == 140)
	{
		const Value greaterThan = Value(runtimeStack[runtimeStackPointer - 1].getValue( ) > runtimeStack[
			                                runtimeStackPointer].getValue( ));
		runtimeStack.pop_back();
		runtimeStack.pop_back();
		runtimeStack.emplace_back(greaterThan);
		runtimeStackPointer--;
		return ++programCounter;
	}

	// printc [10010000]
	if (byteCode == 144)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getValue( ) << std::endl;
		runtimeStack.pop_back( );
		return ++programCounter;
	}

	// prints [10010001]
	if (byteCode == 145)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getValue( ) << std::endl;
		runtimeStack.pop_back( );
		return ++programCounter;
	}

	// printi [10010010]
	if (byteCode == 146)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getValue( ) << std::endl;
		runtimeStack.pop_back( );
		return ++programCounter;
	}

	// printf [10010011]
	if (byteCode == 147)
	{
		std::cout << runtimeStack[runtimeStackPointer--].getFloat( ) << std::endl;
		runtimeStack.pop_back( );
		return ++programCounter;
	}

	// Error Handling
	std::cerr << "Instruction " << byteCode << " does not exist" << std::endl;
	exit(EXIT_FAILURE);
}

void ByteCode::printStack(std::vector<Value> vec)
{
	std::cout << '[';
	if (!vec.empty( ))
	{
		for (long long i = 0; i < vec.size( ); i++)
		{
			std::cout << vec[i];
			if (i + 1 != vec.size( ))
			{
				std::cout << ", ";
			}
		}
	}
	else
	{
		std::cout << " Empty ";
	}
	std::cout << ']';
}

std::ostream& operator<<(std::ostream& os, const ByteCode& byteCode)
{
	os << " BC(" << byteCode.value << ")";
	return os;
}
