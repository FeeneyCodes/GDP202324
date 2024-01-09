#pragma once

#include "cPerson.h"

class cSmartArray
{
public:
	unsigned int getSize(void);

	void insertAtEnd(cPerson thePerson);

	// What if this index is invalid?? (i.e. what do we return?)
	cPerson getAtIndex(unsigned int index);

	// You could also do something like this if you'd like:
	// (if there are no exceptions, for instance)
	// Returns true if the index is valid 
	//   or false if invalid (i.e. ignore the foundPerson)
	bool getAtIndex(unsigned int index, cPerson &foundPerson);


private:


	static const unsigned int MAX_SIZE = 10;
	// Array that holds the people
	cPerson m_theData[MAX_SIZE];

	unsigned int m_NextInsertIndexLocation = 0;

};

