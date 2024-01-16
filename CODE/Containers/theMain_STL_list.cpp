#include <list>
#include <iterator>
#include <algorithm>

#include "cPerson.h"
#include "cPersonMaker.h"

#include <iostream>
#include <string>

//bool isItKeenan(cPerson &testPerson)
//{
//	if (testPerson.firstName == "KEENAN" &&
//		testPerson.middleName == "FAE" && 
//		testPerson.lastName == "BERGMAN")
//	{
//		return true;
//	}
//
//	return false;
//}
bool isItKeenan(cPerson& testPerson)
{
	if (testPerson.firstName != "KEENAN") 
	{
		return false;
	}

	if ( testPerson.middleName != "FAE" )
	{
		return false;
	}

	if ( testPerson.lastName != "BERGMAN" )
	{
		return false;
	}
	
	// It must be the one
	return true;
}

// Functor (or functional object)
class cFindPerson
{
public:
	cFindPerson(std::string firstName, std::string middleName, std::string lastName)
	{
		this->m_firstName_tofind = firstName;
		this->m_middleName_tofind = middleName;
		this->m_lastName_tofind = lastName;
	}
	// The key method: note the strange overloading
	bool operator() (cPerson& testPerson)
	{
		if (testPerson.firstName != this->m_firstName_tofind)
		{
			return false;
		}

		if (testPerson.middleName != this->m_middleName_tofind)
		{
			return false;
		}

		if (testPerson.lastName != this->m_lastName_tofind)
		{
			return false;
		}

		// It must be the one
		return true;
	}

private:
	cFindPerson() {};
	std::string m_firstName_tofind;
	std::string m_middleName_tofind;
	std::string m_lastName_tofind;
};

int main()
{
	cPersonMaker PM;
	PM.LoadNameFiles();

//	const unsigned int NUM_PEOPLE = 7'888'000'000;
	const unsigned int NUM_PEOPLE = 15;

	std::list<cPerson> myPeople;

	for (unsigned int count = 0; count < NUM_PEOPLE; count++)
	{
		cPerson newPerson = PM.makePerson();

//		std::cout << pNewPerson.firstName << " " << pNewPerson.age << std::endl;

		myPeople.insert(myPeople.end(), newPerson);
//		myPeople.insert(myPeople.begin(), newPerson);
	}


	// Insert at location #5
	std::list<cPerson>::iterator itPerson = myPeople.begin();
	
	//itPerson++;
	//itPerson++;
	//itPerson++;
	//itPerson++;
	//itPerson++;
	// Or
	std::advance(itPerson, 5);

	cPerson newPerson;
	newPerson.firstName = "Michael";
	newPerson.lastName = "Feeney";
	myPeople.insert(itPerson, newPerson);


	// 
//	itPerson++;
//	myPeople.erase(itPerson);

// THe "index" location [] operator doesn't make any sense for a list
//	for ( unsigned int index = 0; index != myPeople.size(); index++ )
//	{
//		std::cout << myPeople[index]->firstName << std::endl;
//	}

	for (std::list<cPerson>::iterator itPerson = myPeople.begin();
		 itPerson != myPeople.end();
		 itPerson++)
	{
//		cPerson curPerson = *itPerson;

		std::cout 
			<< itPerson->firstName << " " 
			<< itPerson->middleName << " "
			<< itPerson->lastName 
			<< std::endl;

	}


	//std::list<cPerson*> people_pointers;
	//
	//for (unsigned int count = 0; count < NUM_PEOPLE; count++)
	//{
	//	cPerson* pPerson = PM.makePerson_Pointer();
	//	people_pointers.insert(people_pointers.end(), pPerson);
	//}
	//
	//cPerson* pPerson = PM.makePerson_Pointer();
	//std::list<cPerson*>::iterator itSomewhere = people_pointers.begin();
	//std::advance(itSomewhere, 4);
	//people_pointers.insert(itSomewhere, pPerson);
	//
	//std::list<cPerson*>::iterator itKeenan = std::find(myPeople.begin(), myPeople.end(), pPerson);


	// KEENAN FAE BERGMAN
	// find() only works if the operator== works...
//	std::list<cPerson>::iterator itKeenan = std::find(myPeople.begin(), myPeople.end(), ? ? );

	// regular predicate function...
//	std::list<cPerson>::iterator itKeenan = std::find_if(myPeople.begin(), myPeople.end(), isItKeenan);

	// predicate function object ("functor")
	cFindPerson findKeenanObject("KEENAN", "FAE", "BERGMAN");
//	cFindPerson findKeenan;

	std::list<cPerson>::iterator itKeenan = std::find_if(myPeople.begin(), myPeople.end(), findKeenanObject);

	// Usually you'll see this instead...
	std::list<cPerson>::iterator itKeenan = std::find_if(myPeople.begin(), myPeople.end(), cFindPerson("KEENAN", "FAE", "BERGMAN") );


	if (itKeenan == myPeople.end())
	{
		std::cout << "Didn't find them." << std::endl;
	}
	if ( itKeenan != myPeople.end() )
	{
		std::cout 
			<< itPerson->firstName << " " 
			<< itPerson->middleName << " "
			<< itPerson->lastName 
			<< std::endl;
	}

	return 0;
}
