#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>

#include "cCustomer.h"

#include "cMapV1.h"
#include "cMapV2.h"


int main()
{
	std::string names[100];



	cCustomer michael = cCustomer("Michael", 7, "123 Anywhere", 829383728);

	cMapV2 myHashMap;
	unsigned int index = myHashMap.calcNameHash("Michaelsdfklsdfhksdjfdskj");

	//myCust.StoreAtIndex(173846283, cCustomer("Ali", 37, "4 Street", 173846283));
	//myCust.StoreAtIndex(1674582728, cCustomer("Robin", 17, "NFA", 1674582728));





	return 0;
}


int main__()
{
	cMapV1 myCust;
	myCust.StoreAtIndex(829383728, cCustomer("Michael", 7, "123 Anywhere", 829383728));
	myCust.StoreAtIndex(173846283, cCustomer("Ali", 37, "4 Street", 173846283));
	myCust.StoreAtIndex(1674582728, cCustomer("Robin", 17, "NFA", 1674582728));

	cCustomer foundCust;
	foundCust = myCust.getAtIndex(829383728);





	std::vector<cCustomer> vecCustomers;
	 
	vecCustomers.push_back( cCustomer("Michael", 7, "123 Anywhere", 829383728) );
	vecCustomers.push_back( cCustomer("Ali", 37, "4 Street", 173846283) );
	vecCustomers.push_back( cCustomer("Robin", 17, "NFA", 1674582728) );

	for (std::vector<cCustomer>::iterator itCustomer = vecCustomers.begin();
		 itCustomer != vecCustomers.end();
		 itCustomer++)
	{
//		std::cout << (*itCustomer).name << std::endl;

//		std::cout << itCustomer->name << std::endl;
	}

	// 1. Linear search
	// 2. Make an array where the index is large enough to hold them all
	//		cCustomer allDeCustomers[999999999];
	// 3. Magic container that somehow indexes by whatever (dictionary)

	// 999,999,999
	// 2 - 1 
	// 4 - 2
	// 8 - 3
	// 16 - 4
	// 32 - 5
	// 64 - 6
	// 128 - 7
	// 256 - 8
	// 4 billion - 32 searches
	// 8 b - 33 
	// 16 billion billion - 64

 // std::vector<cCustomer> vecCustomers;		// Always start at 0

	std::map< unsigned long long /*SIN*/, cCustomer> mapCustomers;

//	mapCustomers[0] = cCustomer("Michael", 7, "123 Anywhere", 829383728);
//	mapCustomers[1] = cCustomer("Robin", 17, "NFA", 1674582728);
//	mapCustomers[2] = cCustomer("Ali", 37, "4 Street", 173846283);

	mapCustomers[8296383728] = cCustomer("Michael", 7, "123 Anywhere", 829383728);
	mapCustomers[1674582728] = cCustomer("Robin", 17, "NFA", 1674582728);
	mapCustomers[1738446283] = cCustomer("Ali", 37, "4 Street", 173846283);

//	mapCustomers[8296383728] = cCustomer("Ted", 77, "Hey", 829383728);

	for (std::map< unsigned long long /*SIN*/, cCustomer>::iterator itCustomer = mapCustomers.begin();
		 itCustomer != mapCustomers.end();
		 itCustomer++)
	{
//		std::cout << (*itCustomer).name << std::endl;

//		std::cout << itCustomer->second.name << std::endl;
	}


	std::map< unsigned long long, cCustomer>::iterator itCustomer = mapCustomers.find(1674582728);
//	std::map< unsigned long long, cCustomer>::iterator itCustomer = mapCustomers.find(99);

	if ( itCustomer == mapCustomers.end() )
	{
		std::cout << "Didn't find it" << std::endl;
	}
	if (itCustomer != mapCustomers.end())
	{
		std::cout << itCustomer->second.name << std::endl;
	}


//	cCustomer* pA = new cCustomer();
//	*pA

	std::list<std::string> listName;
	listName.insert(listName.begin(), "Michael");
	listName.insert(listName.begin(), "Mary");
	listName.insert(listName.begin(), "Mike");
	listName.insert(listName.begin(), "Ali");
	listName.insert(listName.begin(), "Robin");

//	for (unsigned int index = 0; index != listName.size(); index++)
//	{
//		std::cout << listName[index] << std::endl;
//	}

	std::list<std::string>::iterator itNames;
	itNames = listName.begin();

	for (; itNames != listName.end(); itNames++)
	{
		std::cout << *itNames << std::endl;
	}

	std::vector<std::string> vecNames;

	vecNames.push_back("Michael");		
	vecNames.push_back("Mary");			// <--- Iterator points to here
	vecNames.push_back("Mike");
	vecNames.push_back("Ali");
	vecNames.push_back("Robin");

//	std::vector<int> vecInts;
//	int vecInts[10];
//	vecInts.push_back(20);	// 8 bytes
//	vecInts.push_back(54);


	// "end()"

	for ( unsigned int index = 0; index != vecNames.size(); index++ )
	{
		std::cout << vecNames[index] << std::endl;
	}

	std::vector<std::string>::iterator itVecNames;
	itVecNames = vecNames.begin();

//	for ( ; itVecNames != vecNames.end(); itVecNames++ )

	typedef std::vector<std::string>::iterator vecIt;

	for (vecIt itVecNames = vecNames.begin();
		 itVecNames != vecNames.end(); 
		 itVecNames++ )
	{
		std::cout << *itVecNames << std::endl;
	}

	// Sort assumes that "<" makes sense

	std::sort(vecNames.begin(), vecNames.end());

	for (unsigned int index = 0; index != vecNames.size(); index++)
	{
		std::cout << vecNames[index] << std::endl;
	}

	return 0;
}














void DoIt(void)
{
	// 0101 1100
	//    5    C
	//        92

	const int HAS_A_MESH = 128;		//  01000000
	const int HAS_A_COLOUR = 64;	//  00100000

	int has_shape = 1;
//	has_shape << 4;
	
	void* myItems[8];
	myItems[0] = NULL;
	myItems[1];// = --->a mesh pointer

	int itemIHave = 92;

	// 0101 1100
	// 0100 0000
	// ---------
	// 0100 0000  -- NOT zero so true
	if (itemIHave & HAS_A_MESH) 
	{
		// I've got a mesh
	}


}