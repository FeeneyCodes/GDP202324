#pragma once

#pragma once

#include "cPerson.h"

template <class T>
class tSmartArray
{
public:
	tSmartArray()
	{
		//	static const unsigned int INITIAL_ARRAY_SIZE = 10;
		// 	cPerson* m_theData = NULL;// = new cPerson[INITIAL_ARRAY_SIZE];

//		this->m_theData = new cPerson[INITIAL_ARRAY_SIZE];
		this->m_theData = new T[INITIAL_ARRAY_SIZE];

		// Note size of array
		this->m_CurrentSizeOfArray = INITIAL_ARRAY_SIZE;
	}

	~tSmartArray()
	{
		// Get rid of the array
		delete[] this->m_theData;
	}


	unsigned int getSize(void)
	{
		return this->m_NextInsertIndexLocation;
	}

	unsigned int getMaxCapacity(void)
	{
		return this->m_CurrentSizeOfArray;
	}


//	void insertAtEnd(cPerson thePerson)
	void insertAtEnd(T thePerson)
	{
	// Do we have space for this entry
		if (this->m_NextInsertIndexLocation == this->m_CurrentSizeOfArray)
		{
			// Nope
//			std::cout << "Not big enough to add. Current size = " << this->m_CurrentSizeOfArray << ". Resizing." << std::endl;
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

	// What if this index is invalid?? (i.e. what do we return?)
//	cPerson getAtIndex(unsigned int index)
	T getAtIndex(unsigned int index)
	{
		// We might want to see if this is out of bounds or something...
		// (and decide what to do if it is out of bounds)
		return this->m_theData[index];
	}

	// You could also do something like this if you'd like:
	// (if there are no exceptions, for instance)
	// Returns true if the index is valid 
	//   or false if invalid (i.e. ignore the foundPerson)
//	bool getAtIndex(unsigned int index, cPerson& foundPerson);


private:


//	static const unsigned int MAX_SIZE = 10;
//	// Array that holds the people
//	cPerson m_theData[MAX_SIZE];

	static const unsigned int INITIAL_ARRAY_SIZE = 10;
//	cPerson* m_theData = NULL;// = new cPerson[INITIAL_ARRAY_SIZE];
	T* m_theData = NULL;// = new cPerson[INITIAL_ARRAY_SIZE];

	unsigned int m_NextInsertIndexLocation = 0;
	// How big is the array now
	unsigned int m_CurrentSizeOfArray;	// Starts at INITIAL_ARRAY_SIZE


	void m_GrowArray(void)
	{
	// 1. Determine new size for the new array
	// Make it 25% larger
		unsigned int newArraySize = this->m_CurrentSizeOfArray + (this->m_CurrentSizeOfArray / 4);

		// 2. Create a new array
//		cPerson* pTheNewArray = new cPerson[newArraySize];
		T* pTheNewArray = new T[newArraySize];

		// 3. Copy the data from the old to new array
		for (unsigned int index = 0; index != this->m_CurrentSizeOfArray; index++)
		{
			pTheNewArray[index] = this->m_theData[index];
		}

		// 4. Delete the old array
		delete[] this->m_theData;

		// 5. Re-assign the pointers and update the counters, etc.
		this->m_theData = pTheNewArray;
		this->m_CurrentSizeOfArray = newArraySize;

		return;
	}

};


