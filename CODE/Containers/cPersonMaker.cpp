#include "cPersonMaker.h"


bool cPersonMaker::LoadNameFiles(void)
{
	this->loadBoyNames("dist.male.first.txt");
	this->loadGirlNames("dist.female.first.txt");
	this->loadLastNames("US_LastNames.txt");

	return true;
}





bool cPersonMaker::loadBoyNames(std::string filename)
{
//	JAMES          3.318  3.318      1
//	JOHN           3.271  6.589      2
//	ROBERT         3.143  9.732      3

	std::ifstream boyNameFile(filename.c_str());
	if (!boyNameFile.is_open())
	{
		return false;
	}

	std::string currentName;

	while (boyNameFile >> currentName)		// JOHN
	{
		this->m_vecBoyNames.push_back(currentName);
		// Discard
		float discardData;
		boyNameFile >> discardData;		// Discard 3.271
		boyNameFile >> discardData;		// Discard 6.589
		boyNameFile >> discardData;		// Discard 2
	}

	return true;
}


bool cPersonMaker::loadGirlNames(std::string filename)
{

	std::ifstream girlNameFile(filename.c_str());
	if (!girlNameFile.is_open())
	{
		return false;
	}

	std::string currentName;

	while (girlNameFile >> currentName)		// JOHN
	{
		this->m_vecGirlNames.push_back(currentName);
		// Discard
		float discardData;
		girlNameFile >> discardData;		// Discard 3.271
		girlNameFile >> discardData;		// Discard 6.589
		girlNameFile >> discardData;		// Discard 2
	}

	return true;
}

bool cPersonMaker::loadLastNames(std::string filename)
{

	std::ifstream lastNameFile(filename.c_str());
	if (!lastNameFile.is_open())
	{
		return false;
	}

	std::string currentName;

	while (lastNameFile >> currentName)		// JOHN
	{
		this->m_vecLastNames.push_back(currentName);
		// Discard
		float discardData;
		lastNameFile >> discardData;		// Discard 3.271
		lastNameFile >> discardData;		// Discard 6.589
		lastNameFile >> discardData;		// Discard 2
	}

	return true;
}

cPerson cPersonMaker::makePerson(void)
{
	cPerson* pPerson = this->makePerson_Pointer();

	cPerson personToReturn = *pPerson;

	delete pPerson;

	return personToReturn;
}



cPerson* cPersonMaker::makePerson_Pointer(void)
{
	cPerson* pPerson = new cPerson();

	if (rand() % 10 > 5)		// 50-50 chance
	{
		// Pick from boys list
		// HACK:
		pPerson->firstName = this->m_vecBoyNames[rand() % (this->m_vecBoyNames.size() - 1)];
	}
	else
	{
		// Pick from girls list
		// HACK:
		pPerson->firstName = this->m_vecGirlNames[rand() % (this->m_vecGirlNames.size() - 1)];
	}

	if (rand() % 10 > 5)		// 50-50 chance
	{
		// Pick from boys list
		// HACK:
		pPerson->middleName = this->m_vecBoyNames[rand() % (this->m_vecBoyNames.size() - 1)];
	}
	else
	{
		// Pick from girls list
		// HACK:
		pPerson->middleName = this->m_vecGirlNames[rand() % (this->m_vecGirlNames.size() - 1)];
	}

	// HACK:
	pPerson->lastName = this->m_vecLastNames[rand() % (this->m_vecLastNames.size() - 1)];

	pPerson->age = rand() % 120;

	return pPerson;
}