#pragma once

#include "iCommand.h"
#include <string>


class cCommandFactory
{
public:
	cCommandFactory();

	// Passing whatever generic type you need to the 
	//	Abstract Factory...
	iCommand* makeCommand(std::string parameters);
	iCommand* makeCommand(std::string JSON);
	iCommand* makeCommand(void* pOldSchoolRulz);

};
