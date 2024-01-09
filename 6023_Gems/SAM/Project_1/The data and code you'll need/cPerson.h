// A classic type of header guard
#ifndef _cPerson_HG_2020_09_26_
#define _cPerson_HG_2020_09_26_

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

#include <string>

class cPerson
{
public:
	cPerson();		// constructor (c'tor)
	// Note: Think if you want this virtual or not...?
	// if polymorphic (i.e. will inherit), the 
	//	MAKE it a virtual destructor or you'll get a memory leak
	~cPerson();		// destructor (d'tor)

	std::string first;
	std::string middle;
	std::string last;


	// enum inside the class "tightly coupled"
	enum eGenderType
	{
		MALE = 0,
		FEMALE,
		NON_BINARY,
		RATHER_NOT_SAY_UNKNOWN
	};

	eGenderType gender;
	std::string getGenderAsString(void);

	int age;		// default = -1;

	// These are in the format from the US census data: https://catalog.data.gov/dataset/street-names-37fec/resource/d655cc82-c98f-450a-b9bb-63521c870503
	//
	// Something like this: "MISSION BAY BLVD NORTH, MISSION BAY, BLVD, NORTH"
	// would be:
	//	- streetName = "MISSION BAY"
	//  - streetType = "BLVD"
	//  - streetDirection = "NORTH"	    Note: many streets don't have a streetDirection
	//                                        and "street direction" can be stuff like "RAMP", etc. 
	// 
	int streetNumber;		// default = 0 
	std::string streetName;
	std::string streetType;
	std::string streetDirection;

	std::string city;
	std::string province;
	char postalCode[6];		// Canadian postal codes are 6 characters

	unsigned int SIN;	// = 0
	//unsigned int SIN = 0;		// C++ 11

	unsigned int getSnotifyUniqueUserID(void);

private:
	unsigned int m_Snotify_UniqueUserID;
	// 
	static unsigned int m_NEXT_Snotify_UniqueUSerID;
};



#endif
