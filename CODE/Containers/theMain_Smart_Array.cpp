#include "cPerson.h"
#include "cPersonMaker.h"

#include <iostream>
#include <string>

#include "cSmartArray.h"
#include "tSmartArray.h"		// Templated version



int main()
{
	cPersonMaker PM;

	PM.LoadNameFiles();

	const unsigned int NUM_PEOPLE = 7'888'000'000;
//	const unsigned int NUM_PEOPLE = 15;

	cSmartArray myPeople;

	for (unsigned int count = 0; count < NUM_PEOPLE; count++)
	{
		cPerson pNewPerson = PM.makePerson();

//		std::cout << pNewPerson.firstName << " " << pNewPerson.age << std::endl;

		myPeople.insertAtEnd(pNewPerson);

	}


	for ( unsigned int index = 0; index != myPeople.getSize(); index++ )
	{
//		std::cout << ::g_vecPeople[index]->firstName << " " 
//				  << ::g_vecPeople[index]->middle << std::endl;

//		cPerson curPerson = myPeople.getAtIndex(index);
		
		std::cout 
			<< myPeople.getAtIndex(index).firstName << " " 
			<< myPeople.getAtIndex(index).middleName << " "
			<< myPeople.getAtIndex(index).lastName << " "
			<< myPeople.getAtIndex(index).age 
			<< std::endl;
	}



	return 0;
}




int main_TEMPLATE_VERSION()
{
	cPersonMaker PM;

	PM.LoadNameFiles();

//	const unsigned int NUM_PEOPLE = 7'888'000'000;
	const unsigned int NUM_PEOPLE = 150;

	tSmartArray<cPerson> myPeople;
//	tSmartArray<cPerson*> myPeople;
//	tSmartArray<std::string> myPeople;

	for (unsigned int count = 0; count < NUM_PEOPLE; count++)
	{
		cPerson pNewPerson = PM.makePerson();

//		std::cout << pNewPerson.firstName << " " << pNewPerson.age << std::endl;

		myPeople.insertAtEnd(pNewPerson);

	}


	for (unsigned int index = 0; index != myPeople.getSize(); index++)
	{
//		std::cout << ::g_vecPeople[index]->firstName << " " 
//				  << ::g_vecPeople[index]->middle << std::endl;

//		cPerson curPerson = myPeople.getAtIndex(index);

		std::cout
			<< myPeople.getAtIndex(index).firstName << " "
			<< myPeople.getAtIndex(index).middleName << " "
			<< myPeople.getAtIndex(index).lastName << " "
			<< myPeople.getAtIndex(index).age
			<< std::endl;
	}



	return 0;
}