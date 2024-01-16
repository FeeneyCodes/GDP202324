
#include "cPerson.h"
#include "cPersonMaker.h"

#include <iostream>
#include <string>

#include "cLinkedList.h"

void PrintPerson( const cPerson &thePerson )
{
	std::cout
		<< thePerson.firstName << " "
		<< thePerson.middleName << " "
		<< thePerson.lastName << " "
		<< "age: "  << thePerson.age 
		<< std::endl;
	return;
}


int main()
{
	cPersonMaker PM;
	PM.LoadNameFiles();

//	const unsigned int NUM_PEOPLE = 7'888'000'000;
	const unsigned int NUM_PEOPLE = 15;

	cLinkedList myPeople;

	//cPerson newPerson = PM.makePerson();
	//myPeople.insertAtCurrent(newPerson);

	//cPerson currentPerson = myPeople.getAtCurrent();



	for (unsigned int count = 0; count < NUM_PEOPLE; count++)
	{
		cPerson newPerson = PM.makePerson();

		myPeople.insertAtCurrent(newPerson);
	}

	// Move to the head node
	myPeople.moveToHead();
	PrintPerson( myPeople.getAtCurrent() );

	myPeople.moveToTail();
	PrintPerson( myPeople.getAtCurrent() );


	do 
	{

		PrintPerson( myPeople.getAtCurrent() );

	} while (myPeople.moveNext());



	return 0;
}
