#pragma once


//mapCustomers[8296383728] = cCustomer("Michael", 7, "123 Anywhere", 829383728);

#include "cCustomer.h"

#include <vector>

class cMapV1
{
public:
	cMapV1() {};

	// Will overwrite if already there
	void StoreAtIndex(unsigned long long index, cCustomer newCustomer);
	// what happens if the customer isn't there? Who knows...?
	cCustomer getAtIndex(unsigned long long index);

private:
	struct sData
	{
		cCustomer theCust;
		unsigned long long index;
	};
	std::vector< sData > vecData;

	// returns null if not found
	sData* m_pFindCustomer(unsigned long long index);

};
