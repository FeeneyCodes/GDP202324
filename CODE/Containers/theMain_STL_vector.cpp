#include "cPerson.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>


std::vector<std::string> g_vecBoyNames;
std::vector<std::string> g_vecGirlNames;
std::vector<std::string> g_vecLastNames;

bool loadBoyNames(std::string filename);
bool loadGirlNames(std::string filename);
bool loadLastNames(std::string filename);

cPerson* makePerson(void);

std::vector<cPerson*> g_vecPeople;
//std::vector<cPerson> g_vecPeoples;

bool cPersonIsLess( cPerson* pA, cPerson* pB )
{
	if (pA ->age < pB->age)
	{
		return true;
	}
	return false;
}


int main()
{
	loadBoyNames("dist.male.first.txt");
	loadGirlNames("dist.female.first.txt");
	loadLastNames("US_LastNames.txt");

//	const unsigned int NUM_PEOPLE = 7'888'000'000;
	const unsigned int NUM_PEOPLE = 10;

//	::g_vecPeople.reserve(100);

	for ( unsigned int count = 0; count < NUM_PEOPLE; count++ )
	{
		cPerson* pNewPerson = makePerson();


		::g_vecPeople.push_back(pNewPerson);
		//...is the same as this:
//		::g_vecPeople.insert(::g_vecPeople.end(), pNewPerson);

		// As slow as you could go
//		::g_vecPeople.insert(::g_vecPeople.begin(), pNewPerson);
	}
	cPerson* pMe = new cPerson();
	pMe->firstName = "Michael";
	pMe->age = 100;

	std::vector< cPerson* >::iterator itInsertMichael = ::g_vecPeople.begin();

	//itInsertMichael++;
	//itInsertMichael++;
	//itInsertMichael++;
	//itInsertMichael++;

	std::advance(itInsertMichael, 4);

	::g_vecPeople.insert(itInsertMichael, pMe);
//	itInsertMichael--;
//	itInsertMichael--;		// STEFFANIE

//	::g_vecPeople.erase(itInsertMichael);


	std::sort( ::g_vecPeople.begin(), ::g_vecPeople.end(), cPersonIsLess );

	
//	std::vector< cPerson* >::iterator itWhereIsMichael 
//			= std::find(::g_vecPeople.begin(), ::g_vecPeople.end(), "Michael");

	//// Find it? 
	//if (itWhereIsMichael == ::g_vecPeople.end())
	//{
	//	std::cout << "Didn't find Michael" << std::endl;
	//}
	//
	//if (itWhereIsMichael != ::g_vecPeople.end())
	//{
	//	std::cout << (*itWhereIsMichael)->firstName << std::endl;
	//}

//	::g_vecPeople.push_back( pMe);

//	for ( unsigned int index = 0; index != ::g_vecPeople.size(); index++ )
//	{
//		std::cout << ::g_vecPeople[index]->firstName << std::endl;
//	}

	for ( std::vector< cPerson* >::iterator itPerson = ::g_vecPeople.begin();
		  itPerson != ::g_vecPeople.end();
		  itPerson++ )
	{
//		std::cout << (*itPerson)->firstName << std::endl;

		cPerson* pCurPerson = *itPerson;

		std::cout << (unsigned long long)pCurPerson << " : " << pCurPerson->firstName << std::endl;

	}


	return 0;
}



bool loadBoyNames(std::string filename)
{
//	JAMES          3.318  3.318      1
//	JOHN           3.271  6.589      2
//	ROBERT         3.143  9.732      3

	std::ifstream boyNameFile(filename.c_str());
	if ( ! boyNameFile.is_open() )
	{
		return false;
	}

	std::string currentName;

	while( boyNameFile >> currentName )		// JOHN
	{
		::g_vecBoyNames.push_back(currentName);
		// Discard
		float discardData;
		boyNameFile >> discardData;		// Discard 3.271
		boyNameFile >> discardData;		// Discard 6.589
		boyNameFile >> discardData;		// Discard 2
	}

	return true;
}


bool loadGirlNames(std::string filename)
{

	std::ifstream girlNameFile(filename.c_str());
	if ( ! girlNameFile.is_open() )
	{
		return false;
	}

	std::string currentName;

	while( girlNameFile >> currentName )		// JOHN
	{
		::g_vecGirlNames.push_back(currentName);
		// Discard
		float discardData;
		girlNameFile >> discardData;		// Discard 3.271
		girlNameFile >> discardData;		// Discard 6.589
		girlNameFile >> discardData;		// Discard 2
	}

	return true;
}

bool loadLastNames(std::string filename)
{

	std::ifstream lastNameFile(filename.c_str());
	if (!lastNameFile.is_open())
	{
		return false;
	}

	std::string currentName;

	while (lastNameFile >> currentName)		// JOHN
	{
		::g_vecLastNames.push_back(currentName);
		// Discard
		float discardData;
		lastNameFile >> discardData;		// Discard 3.271
		lastNameFile >> discardData;		// Discard 6.589
		lastNameFile >> discardData;		// Discard 2
	}

	return true;
}

cPerson* makePerson(void)
{
	cPerson* pPerson = new cPerson();

	if ( rand() % 10 > 5 )		// 50-50 chance
	{
		// Pick from boys list
		// HACK:
		pPerson->firstName = ::g_vecBoyNames[rand() % (::g_vecBoyNames.size() - 1)];
	}
	else
	{
		// Pick from girls list
		// HACK:
		pPerson->firstName = ::g_vecGirlNames[rand() % (::g_vecGirlNames.size() - 1)];
	}

	if ( rand() % 10 > 5 )		// 50-50 chance
	{
		// Pick from boys list
		// HACK:
		pPerson->middleName = ::g_vecBoyNames[rand() % (::g_vecBoyNames.size() - 1)];
	}
	else
	{
		// Pick from girls list
		// HACK:
		pPerson->middleName = ::g_vecGirlNames[rand() % (::g_vecGirlNames.size() - 1)];
	}

	// HACK:
	pPerson->lastName = ::g_vecLastNames[rand() % (::g_vecLastNames.size() - 1)];

	pPerson->age = rand() % 120;

	return pPerson;
}