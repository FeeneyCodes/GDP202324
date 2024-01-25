#pragma once

#include <string>
#include "cCustomer.h"

class cMapV2
{
public:
	cMapV2()
	{
		this->pThePeople = new sData[ARRAY_SIZE];
	}

	void addCustomer(std::string index, cCustomer newCustomer)
	{
		unsigned int index = this->calcNameHash(index);
		this->pThePeople[index] = newCustomer;
	}

	unsigned int calcNameHash(std::string theName)
	{
		unsigned int index = 0;
		for ( char curChar : theName )
		{
			unsigned int charASCII = curChar;
			index += charASCII;
		}
		index %= cMapV2::ARRAY_SIZE;
		return index;
	}

private:
	// We are going to store these people
	//	 by 1st name. 
	struct sData
	{
		cCustomer theCust;
		std::string firstName;
	};

	const unsigned int ARRAY_SIZE = 10'000;
	sData* pThePeople = NULL;
};

