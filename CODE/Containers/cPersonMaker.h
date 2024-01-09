#pragma once
#include "cPerson.h"

#include <vector>
#include <fstream>



class cPersonMaker
{
public:
	bool LoadNameFiles(void);

	cPerson* makePerson_Pointer(void);
	cPerson makePerson(void);


private:
	bool loadBoyNames(std::string filename);
	bool loadGirlNames(std::string filename);
	bool loadLastNames(std::string filename);
	std::vector<std::string> m_vecBoyNames;
	std::vector<std::string> m_vecGirlNames;
	std::vector<std::string> m_vecLastNames;

};
