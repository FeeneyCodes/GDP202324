#pragma once

#include <string>

class cCustomer
{
public:
	cCustomer() {
		this->SIN = 0;
		this->name = "INVALID";
		this->age = -1;
		this->address = "INVALID";
	};
	cCustomer(std::string theName, int theAge, std::string theAddress,
			  unsigned int theSIN)
	{
		this->SIN = theSIN;
		this->name = theName;
		this->age = theAge;
		this->address = theAddress;
	}
	unsigned int SIN;
	std::string name;
	int age;
	std::string address;
};