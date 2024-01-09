#include "cSmartArray.h"

unsigned int cSmartArray::getSize(void)
{
	return this->m_NextInsertIndexLocation;
}

void cSmartArray::insertAtEnd(cPerson thePerson)
{
	// Add person to the array
	this->m_theData[this->m_NextInsertIndexLocation] = thePerson;

	// Move to the next location
	this->m_NextInsertIndexLocation++;

	return;
}

// What if this index is invalid?? (i.e. what do we return?)
cPerson cSmartArray::getAtIndex(unsigned int index)
{
	// We might want to see if this is out of bounds or something...
	// (and decide what to do if it is out of bounds)
	return this->m_theData[index];
}