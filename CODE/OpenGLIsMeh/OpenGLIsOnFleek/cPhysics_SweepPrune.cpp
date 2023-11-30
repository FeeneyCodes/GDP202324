#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>
#include <iterator>

#include "cAABB.h"


struct AABB_Pair
{
	cAABB* pAABB[2];
};

bool AABBLessCompare_minX(cAABB* pA, cAABB* pB)
{
	if (pA->minXYZ.x < pB->minXYZ.x )
	{
		return true;
	}
	return false;
}

class cAABBLessCompare_minX
{
public:
	//  "Functor" or predicate object
	// The "operator()" is because no one uses this
	bool operator() (cAABB* pA, cAABB* pB)
	{
		if (pA->minXYZ.x < pB->minXYZ.x)
		{
			return true;
		}
		return false;
	}
};

struct cAABBEdge
{
	float edgeValue;
	cAABB* theAABB;
};

bool cAABBEdgeLessCompare(cAABBEdge &A, cAABBEdge &B)
{
	if (A.edgeValue < B.edgeValue)
	{
		return true;
	}
	return false;
}




void findOverlaps(std::vector< cAABB* > &vecAABBs, std::vector< AABB_Pair* > vecAABB_Pairs )
{
	// Start with the x axis



	std::vector< cAABBEdge > vecAABBs_X_Sorted;

	// Make a copy of the vector
	for (std::vector< cAABB* >::iterator itAABB = vecAABBs.begin();
		 itAABB != vecAABBs.end(); itAABB++ )
	{
		cAABBEdge curMin;
		curMin.theAABB = *itAABB;
		curMin.edgeValue = (*itAABB)->minXYZ.x;

		vecAABBs_X_Sorted.push_back(curMin);

		cAABBEdge curMax;
		curMax.theAABB = *itAABB;
		curMax.edgeValue = (*itAABB)->maxXYZ.x;

		vecAABBs_X_Sorted.push_back(curMax);

	}

	// pAABB_1, minX
	// pAABB_1, maxX;
	// pAABB_2, minX;
	// pAABB_2, maxX;
	// ... and so on



	std::sort(vecAABBs_X_Sorted.begin(), vecAABBs_X_Sorted.end(), cAABBEdgeLessCompare);

	// Now go through the vector from start to end.
	// If any 2 values aren't the same AABB, you have an overlap
//	for ( 


	// Or you can use a predicate class (or a "functor")
//	std::sort(vecAABBsSorted.begin(), vecAABBsSorted.end(), cAABBLessCompare_minX() );

//	cAABBLessCompare_minX myTesterFunctorThing;
//	std::sort(vecAABBsSorted.begin(), vecAABBsSorted.end(), myTesterFunctorThing);

//	std::vector< int > vecInts;
//	std::sort(vecInts.begin(), vecInts.end());


	return;
}


void TestSort(void)
{
	cAABB* pA = new cAABB();
	cAABB* pB = new cAABB();
	cAABB* pC = new cAABB();

	pA->minXYZ = glm::vec3(-10.0f, 0.0f, 0.0f);
	pA->maxXYZ = glm::vec3( -3.0f, 0.0f, 0.0f);

	pB->minXYZ = glm::vec3( -5.0f, 0.0f, 0.0f);
	pB->maxXYZ = glm::vec3(  5.0f, 0.0f, 0.0f);

	pC->minXYZ = glm::vec3(  8.0f, 0.0f, 0.0f);
	pC->maxXYZ = glm::vec3( 12.0f, 0.0f, 0.0f);


	std::vector< cAABB* > vecAABBs;
	vecAABBs.push_back(pB);
	vecAABBs.push_back(pA);
	vecAABBs.push_back(pC);

	std::vector< AABB_Pair* > vecAABB_Pairs;
	findOverlaps(vecAABBs, vecAABB_Pairs);

	return;
}