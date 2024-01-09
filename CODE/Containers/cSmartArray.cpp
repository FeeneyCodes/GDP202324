#include "cSmartArray.h"
#include <iostream>

unsigned int cSmartArray::getSize(void)
{
	return this->m_NextInsertIndexLocation;
}

unsigned int cSmartArray::getMaxCapacity(void)
{
	return this->m_CurrentSizeOfArray;
}


void cSmartArray::insertAtEnd(cPerson thePerson)
{
	// Do we have space for this entry
	if ( this->m_NextInsertIndexLocation == this->m_CurrentSizeOfArray )
	{
		// Nope
		std::cout << "Not big enough to add. Current size = " << this->m_CurrentSizeOfArray << ". Resizing." << std::endl;
		this->m_GrowArray();
		return;
	}

	// At this point, the array will be the right size.

	// Add person to the array
	this->m_theData[this->m_NextInsertIndexLocation] = thePerson;

	// Move to the next location
	this->m_NextInsertIndexLocation++;

	return;
}

void cSmartArray::m_GrowArray(void)
{
	// 1. Determine new size for the new array
	// Make it 25% larger
	unsigned int newArraySize = this->m_CurrentSizeOfArray + (this->m_CurrentSizeOfArray / 4);

	// 2. Create a new array
	cPerson* pTheNewArray = new cPerson[newArraySize];

	// 3. Copy the data from the old to new array
	for ( unsigned int index = 0; index != this->m_CurrentSizeOfArray; index++ )
	{
		pTheNewArray[index] = this->m_theData[index];
	}

	// 4. Delete the old array
	delete [] this->m_theData;

	// 5. Re-assign the pointers and update the counters, etc.
	this->m_theData = pTheNewArray;
	this->m_CurrentSizeOfArray = newArraySize;

	return;
}




// What if this index is invalid?? (i.e. what do we return?)
cPerson cSmartArray::getAtIndex(unsigned int index)
{
	// We might want to see if this is out of bounds or something...
	// (and decide what to do if it is out of bounds)
	return this->m_theData[index];
}


cSmartArray::cSmartArray()
{
//	static const unsigned int INITIAL_ARRAY_SIZE = 10;
// 	cPerson* m_theData = NULL;// = new cPerson[INITIAL_ARRAY_SIZE];

	this->m_theData = new cPerson[INITIAL_ARRAY_SIZE];

	// Note size of array
	this->m_CurrentSizeOfArray = INITIAL_ARRAY_SIZE;
}

cSmartArray::~cSmartArray()
{
	// Get rid of the array
	delete [] this->m_theData;
}
