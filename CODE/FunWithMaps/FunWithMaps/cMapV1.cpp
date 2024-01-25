#include "cMapV1.h"

void cMapV1::StoreAtIndex(unsigned long long index, cCustomer newCustomer)
{
	sData* pNode = this->m_pFindCustomer(index);
	if (pNode == NULL)
	{
		sData newNode;
		newNode.theCust = newCustomer;
		newNode.index = index;
		this->vecData.push_back(newNode);
		return;
	}
	// Update it
	pNode->theCust = newCustomer;
	return;
}

// what happens if the customer isn't there? Who knows...?
cCustomer cMapV1::getAtIndex(unsigned long long index)
{
	sData* pNode = this->m_pFindCustomer(index);
	if ( pNode != NULL )
	{
		return pNode->theCust;
	}

	// Return what??? 
	// A bogus customer maybe?
	return cCustomer();
}


cMapV1::sData* cMapV1::m_pFindCustomer(unsigned long long index)
{
	for ( int dataIndex = 0; dataIndex != this->vecData.size(); dataIndex++ )
	{
		// Is this it? 
		if (this->vecData[dataIndex].index == index )
		{
			// Yup

			return &(this->vecData[dataIndex]);
		}
	}
	return NULL;
}

